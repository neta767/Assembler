/**
 * @file second_pass.c
 * @brief This file contains the implementation of the second pass of the assembler.
 * It processes the assembly code, generates machine code, and creates object files.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#include "symbols_list.h"
#include "second_pass.h"
#include "util.h"
#include "validations.h"
#include "code_list.h"
#include "data_list.h"

int code_operand_labels(char *file_am_name, Code *code_head) {
    int error = 0;
    Symbol *operand_label, *label;
    unsigned int word = 0;
    /* Looping through code array */
    while (code_head != NULL) {
        /* Checking if the instruction is of type "direct" */
        if ((code_head->value & BIT_MASK_DIRECT) == BIT_MASK_DIRECT) {
            operand_label = get_operand_label(); /* Getting the next label of type "operand" */
            if (operand_label == NULL) {
                return error; /* Indicates no more labels of type "operand" left */
            }
            if ((label = is_label_defined(operand_label->label)) != NULL) {
                /* Checking if this label was defined */
                word |= (unsigned int) (label->address & MASK_21BIT);
                word <<= BIT_MASK_DIRECT;

                if (label->type == EXTERN) {
                    /* Indicates operand label is of type "extern" */
                    word |= BIT_MASK_EXTERNAL; /* Setting bit 0 for "External" */
                    operand_label->type = EXTERN; /* Updating the address of the label */
                } else {
                    word |= BIT_MASK_RELOCATABLE; /* Setting bit 1 for "Relocatable" */
                    remove_label(operand_label);
                }
                code_head->value = word; /* Updating machine code */
            } else {
                print_error("Unrecognized operand, please check syntax", file_am_name,
                            (int) ((code_head->IC) - IC_INITIAL));
                remove_label(operand_label);
                error = 1; /* Indicates failure */
            }
        }
        if ((code_head->value & BIT_MASK_RELATIVE) == BIT_MASK_RELATIVE) {
            /* Searching for instruction symbol addresses signaled by the first pass */
            operand_label = get_operand_label(); /* Getting the next label of type "operand" */
            if (operand_label == NULL) {
                return error; /* Indicates no more labels of type "operand" left */
            }

            if ((label = is_label_defined(operand_label->label)) != NULL) {
                /* Checking if this label was defined */

                word |= (((label->address) - ((code_head->value) >> FUNCS_POS) + 1) & MASK_21BIT);
                word <<= FUNCS_POS;
                word |= BIT_ABSOLUTE_FLAG;
                code_head->value = word; /* Updating machine code */
            }
            remove_label(operand_label);
        }
        word = 0; /* Resetting word */
        code_head = code_head->next;
    }
    return error;
}

int second_pass(char *file_name, Data *data_head, Code *code_head, const int *IC, const int *DC) {
    char *file_ob_name, *file_ent_name, *file_ext_name;
    int error = 0;

    char *file_am_name = add_extension(file_name, ".am");

    if (code_operand_labels(file_name, code_head) != 0) {
        free_labels();
        return 1; /* Indicates failure */
    }

    /* Scanning the file */
    if (scan_file(file_am_name)) {
        free_labels();
        free_code_list(&code_head);
        free_data_list(&data_head);
        delete_file(file_am_name);
        free(file_am_name);
        return 1; /* Indicates failure */
    }

    /* Getting the object file label */
    file_ob_name = add_extension(file_name, ".ob");

    /* Creating the object file */
    create_ob_file(file_ob_name, code_head, data_head, IC, DC);

    /* Creating "file.ent" if there are "entry" labels */
    if (entry_exist() != 0) {
        file_ent_name = add_extension(file_name, ".ent");
        create_ent_file(file_ent_name);
        free(file_ent_name);
    }
    /* Creating "file.ext" if there are "extern" labels */
    if (extern_exist() != 0) {
        file_ext_name = add_extension(file_name, ".ext");
        create_ext_file(file_ext_name);
        free(file_ext_name);
    }
    free(file_ob_name);
    free(file_am_name);
    return error;
}

/* Function to scan the am file */
int scan_file(char *file_name) {
    char line[MAX_LINE_LENGTH]; /* Buffer for reading lines */
    int line_num = 0; /* Current line number */
    int error = 0; /* Error flag */

    /* Opening the file */
    FILE *file = fopen(file_name, "r");
    if (!file) {
        printf("Error: can't open %s\n", file_name);
        return 1;
    }
    /* Reading line by line */
    while (fgets(line,MAX_LINE_LENGTH, file)) {
        line_num++;
        process_the_line(line_num, file_name, line, &error, file);
    }
    fclose(file);
    return error;
}

int is_entry(char *file_name, int line_num, char *line, int *error, const char *current_word) {
    Symbol *symbol;

    if (get_prompt(current_word) != 2) {
        return 0; /* Scanning line finished */
    }
    /* Checking if there is no label declaration */
    if (*line == NULL_TERMINATOR) {
        print_error("Instructions \".extern\" must have a label declaration", file_name, line_num);
        *error = 1;
        return 0; /* Scanning line finished */
    }
    /* Checking if there is more than one label declaration */
    trim_whitespace(line);
    if (contains_whitespace(line)) {
        print_error("Instructions \".entry\" allow one label declaration at a time", file_name, line_num);
        *error = 1;
        return 0; /* Scanning line finished */
    }
    if (!valid_name(line, line_num, file_name, ENTRY)) {
        *error = 1;
        return 0; /* Scanning line finished */
    }
    symbol = is_symbol_name(line);
    if (symbol) {
        symbol->type = ENTRY; /* Changing the type to "entry" */
        return 0;
    }
    print_error("Label was declared as \".entry\" but was not defined", file_name, line_num);
    return 1;
}


/* Function to process each line of the am file */
void process_the_line(int line_num, char *file_name, char *line, int *error, FILE *file) {
    char *current_word; /* Pointer to the current first word */
    size_t curr_word_len;

    trim_whitespace(line);

    /* Checking if the current line a comment or empty */
    if (line[0] == COMMENT || strlen(line) == 0)
        return; /* Skipping to the next line */

    /* Getting the first word */
    current_word = get_first_word(line);
    if (current_word == NULL) {
        /* Indicates memory allocation failed (all the other allocations were freed inside function) */
        fclose(file);
        free(file_name);
        exit(1); /* Exiting program */
    }
    curr_word_len = strlen(current_word);

    /* Checking for a potential symbol definition */
    if (current_word[curr_word_len - 1] == COLON) {
        free(current_word); /* Freeing memory no longer needed */
        /* Scanning the next word */
        while (*line != NULL_TERMINATOR && !isspace(*line)) /* Skipping the label label */
            line++;
        while (*line != NULL_TERMINATOR && isspace(*line)) /* Setting 'line' to point to the next word */
            line++;
        /* Getting the next word */
        current_word = get_first_word(line);
        curr_word_len = strlen(current_word);
        if (current_word == NULL) {
            /* Indicates memory allocation failed (all the other allocations were freed inside function) */
            fclose(file);
            free(current_word);
            free_labels();
            free(file_name);
            exit(1); /* Exiting program */
        }
    }

    /* Checking for a potential prompt definition */
    if (current_word[0] == DOT) {
        line += curr_word_len; /* Skipping the first word */
        /* Checking for a potential .entry definition */
        is_entry(file_name, line_num, line, error, current_word);
    }
}
