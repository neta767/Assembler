/**
 * @file first_pass.c
 * @brief This file contains the implementation of the first pass of the assembler.
 * It scans the assembly file, processes each line, and updates the data and code lists.
 * It also handles label declarations and checks for errors in the assembly code.
 */

#include "first_pass.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#include "symbols_list.h"
#include "util.h"
#include "validations.h"
#include "code_list.h"
#include "data_list.h"


int first_pass(char *file_name, Data **data_head, Code **code_head, int *IC, int *DC) {
    /* Getting the new file label */
    char *file_am_name = add_extension(file_name, ".am");

    /* Scanning the file */
    if (scan_am_file(file_am_name, data_head, code_head, IC, DC)) {
        free_code_list(code_head);
        free_data_list(data_head);
        free(file_am_name);
        return 1; /* Indicates failure */
    }
    update_data_labels(IC);
    free(file_am_name);
    return 0; /* Indicates success */
}

/* Function to scan the am file */
int scan_am_file(char *file_name, Data **data_head, Code **code_head, int *IC, int *DC) {
    char line[MAX_LINE_LENGTH]; /* Buffer for reading lines */
    int usage = 0; /* usage counter */
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
        process_each_line(code_head, data_head, &usage, IC, DC, line_num, file_name, line, &error, file);
    }
    fclose(file);
    return error;
}

/* Function to process each line of the am file */
void process_each_line(Code **code_head, Data **data_head, int *usage, int *IC, int *DC,
                       int line_num, char *file_name, char *line, int *error, FILE *file) {
    char *current_word; /* Pointer to the current first word */
    char *temp;
    size_t curr_word_len, len;
    char label[MAX_DECLARATION_LENGTH] = {0}; /* Pointer to the label */

    trim_whitespace(line);

    /* Checking if the current line a comment or empty */
    if (line[0] == COMMENT || strlen(line) == 0)
        return; /* Skipping to the next line */

    /* Getting the first word */
    current_word = get_first_word(line);
    if (current_word == NULL) {
        /* Indicates memory allocation failed (all the other allocations were freed inside function) */
        cleanup_and_exit(file, file_name, data_head, code_head, current_word);
        exit(1); /* Exiting program */
    }
    curr_word_len = strlen(current_word);

    /* Checking for a potential symbol definition */
    if (current_word[curr_word_len - 1] == COLON) {
        current_word[curr_word_len - 1] = NULL_TERMINATOR; /* Getting the label without ':' */
        curr_word_len -= 1; /* Getting the label length without ':' */
        if (!valid_name(current_word, line_num, file_name, LABEL)) {
            *error = 1;
            free(current_word);
            return;
        }
        strcpy(label, current_word); /* Copying the label */
        free(current_word); /* Freeing memory no longer needed */
        /* Scanning the next word */
        if (contains_whitespace(line)) {
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
        } else {
            print_error("Invalid label declaration, no value associated with label", file_name, line_num);
            *error = 1;
            return;
        }
    }

    /* Checking for a potential prompt definition */
    if (current_word[0] == DOT) {
        /* Checking for a potential data prompt */
        line += curr_word_len; /* Skipping the first word */
        if (is_data_prompt(data_head, usage, DC, line_num, file_name, file, line, current_word, error, label)) {
            free(current_word);
            return; /* Scanning line finished */
        }
        if (*label) {
            /* label exists but have extra unrecognized command*/
            print_error("Unrecognized command", file_name, line_num);
            free(current_word);
            *error = 1;
            return;
        }
    }

    /* Checking for a potential instruction */
    if (is_instruction(code_head, usage, IC, line, line_num, file, file_name, current_word, error, label)) {
        free(current_word);
        return; /* Scanning line finished */
    }

    /* Checking for a potential prompt definition */
    if (label[0] == '\0') {
        /* Checking for a potential .entry definition */
        if (get_prompt(current_word) == 2) {
            free(current_word);
            return; /* Scanning line finished */
        }
        /* Checking for a potential .extern definition */
        if (is_extern(line_num, file_name, file, line, error, current_word)) {
            free(current_word);
            return; /* Scanning line finished */
        }
    }
    if (strchr(current_word + 1,COLON) != NULL) {
        /* Checking for a failed label declaration (LABEL:add) */
        print_error("Unrecognized command, note that label declarations must have a space after the colon (:)",
                    file_name, line_num);
        *error = 1;
        free(current_word);
        return; /* Scanning line finished */
    }
    while (line && !isspace(*line)) /* Skipping the first word */
        line++;
    while (line && isspace(*line)) /* Skipping whitespace characters */
        line++;
    if (line && *line == COLON) {
        /* Checking for a failed label declaration (LABEL : add...) */
        print_error(
            "Unrecognized command, note that label declarations must have the colon (:) attached to the label name",
            file_name, line_num);
        *error = 1;
        free(current_word);
        return; /* Scanning line finished */
    }
    if (is_symbol_name(current_word) != NULL) {
        /* Checking for a label at the start of the line */
        print_error("Symbol label is not a valid command", file_name, line_num);
        *error = 1;
        free(current_word);
        return; /* Scanning line finished */
    }
    len = strlen(current_word) + TWO; /* +1 for dot, +1 for null-terminator */
    temp = (char *) malloc(len);
    if (temp == NULL) {
        cleanup_and_exit(file, file_name, data_head, code_head, current_word);
        exit(1); /* Exiting program */
    }
    temp[0] = DOT; /* Adding the dot at the beginning */
    strcpy(temp + 1, current_word); /* Copying the original word after the dot */
    if (get_prompt(temp) != -1) {
        print_error("Unrecognized command, note that an prompt must start with a dot (.)", file_name, line_num);
        *error = 1;
        free(temp);
        free(current_word);
        return; /* Scanning line finished */
    }
    print_error("Unrecognized command, please check syntax", file_name, line_num);
    *error = 1;
    free(temp);
    free(current_word);
}

int is_symbol(char *current_word, int line_num, char *file_name, FILE *file, char *line, char *label) {
    size_t curr_word_len = strlen(current_word);
    current_word[curr_word_len - 1] = NULL_TERMINATOR; /* Getting the label without ':' */
    curr_word_len -= 1; /* Getting the label length without ':' */
    if (!valid_name(current_word, line_num, file_name, LABEL))
        return 0;
    strcpy(label, current_word); /* Copying the label */
    free(current_word); /* Freeing memory no longer needed */
    /* Scanning the next word */
    if (contains_whitespace(line)) {
        while (*line != NULL_TERMINATOR && !isspace(*line)) /* Skipping the label label */
            line++;
        while (*line != NULL_TERMINATOR && isspace(*line)) /* Setting 'line' to point to the next word */
            line++;
        /* Getting the next word */
        current_word = get_first_word(line);
        if (current_word == NULL) {
            /* Indicates memory allocation failed (all the other allocations were freed inside function) */
            fclose(file);
            free(current_word);
            free_labels();
            free(file_name);
            exit(1); /* Exiting program */
        }
    } else {
        print_error("Invalid label declaration, no value associated with label", file_name, line_num);
        return 0;
    }
    return 1;
}

void cleanup_and_exit(FILE *file, char *file_name, Data **data_head, Code **code_head, char *current_word) {
    free(current_word);
    fclose(file);
    free_labels();
    free_code_list(code_head);
    free_data_list(data_head);
    free(file_name);
}
