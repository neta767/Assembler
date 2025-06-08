/**
 * @file util.c
 * @brief Utility functions for file handling, string manipulation, and error reporting.
 *
 * This file contains functions to handle file operations, manipulate strings,
 * and report errors in a consistent format. It includes functions to delete files,
 * add extensions to filenames, trim whitespace, check for whitespace in strings,
 * extract numbers from lines, and create output files.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "util.h"
#include "symbols_list.h"
#include "const.h"

void delete_file(char *filename) {
    if (remove(filename) != 0)
        printf(" Error: Failed to delete redundant file");
}

/* Adds an extension to a file label. */
char *add_extension(char *name, char *extension) {
    char *new_name;
    size_t name_len = strlen(name);
    size_t extension_len = strlen(extension);

    new_name = (char *) malloc(name_len + extension_len + 1);
    if (new_name == NULL) {
        printf("Error: Memory allocation failed");
        free_labels();
        exit(1);
    }
    strcpy(new_name, name);
    strcat(new_name, extension);
    return new_name;
}

char *trim_whitespace(char *str) {
    int start, i;
    size_t end = strlen(str) - 1;;
    start = 0;

    /* Skip leading spaces */
    while (isspace((unsigned char) str[start]))
        start++;


    /* If string is all spaces */
    if (str[start] == NULL_TERMINATOR)
        str[0] = NULL_TERMINATOR;

    /* Skip trailing spaces */
    while (end > start && isspace((unsigned char) str[end]))
        end--;

    /* Shift trimmed content to the beginning */
    i = 0;
    while (start <= end)
        str[i++] = str[start++];
    str[i] = NULL_TERMINATOR;
    return str;
}

int contains_whitespace(char *str) {
    while (*str != NULL_TERMINATOR) {
        if (isspace(*str)) {
            return 1; /* Indicates whitespace character found */
        }
        str++;
    }
    return 0; /* Indicates no whitespace character found */
}

int *get_numbers(char *file_name, int line_num, FILE *file, char *line, int *num_count) {
    int *result;
    char buffer[BUFFER_SIZE];
    int numbers[MAX_DATA_NUM];
    int temp_count = 0, i = 0, last_was_comma = 0, num, j;
    size_t length = strlen(line);

    while (i < length && isspace(line[i])) /* Skipping leading whitespace */
        i++;

    if (!isdigit(line[i]) && line[i] != MINUS && line[i] != PLUS) {
        /* Checking for an invalid character */
        if (line[i] == COMMA) {
            print_error("Instruction \".data\" has an illegal comma following the instruction label", file_name,
                        line_num);
            return NULL;
        }
        print_error("Instruction \".data\" has illegal characters, only integers allowed", file_name, line_num);
        return NULL;
    }
    while (i < length) {
        while (i < length && isspace(line[i])) /* Skipping whitespace characters */
            i++;
        if (i >= length)
            break;
        /* Checking for a number with an optional sign */
        if (isdigit(line[i]) || ((line[i] == MINUS || line[i] == PLUS) && isdigit(line[i + 1]))) {
            last_was_comma = 0;
            j = 0;

            if (line[i] == MINUS || line[i] == PLUS) /* Handling optional sign */
                buffer[j++] = line[i++];
            while (i < length && isdigit(line[i])) /* Getting the number */
                buffer[j++] = line[i++];

            /* Checking for an invalid character after the number */
            if (i < length && !isspace(line[i]) && line[i] != COMMA && line[i] != MINUS && line[i] != PLUS) {
                print_error("Instruction \".data\" has illegal characters, only integers allowed", file_name, line_num);
                return NULL;
            }
            buffer[j] = NULL_TERMINATOR;
            num = atoi(buffer);

            /* Checking if the number is in range */
            if (num < MIN_24BIT || num > MAX_24BIT) {
                print_error("Instruction \".data\" has a number that is out of range", file_name, line_num);
                return NULL;
            }
            /* Adding the number to the array */
            numbers[temp_count] = num;
            temp_count++;

            while (i < length && isspace(line[i])) /* Skipping whitespace characters after the number */
                i++;

            if (i < length && line[i] == COMMA) {
                /* Checking for a comma after the number */
                last_was_comma = 1;
                i++;
            } else if (i < length && line[i] != COMMA) {
                print_error("Instruction \".data\" has a missing comma", file_name, line_num);
                return NULL;
            }
        } else {
            print_error("Instruction \".data\" has illegal characters, only integers allowed", file_name, line_num);
            return NULL;
        }
        /* Checking for multiple consecutive commas */
        if (last_was_comma) {
            while (i < length && isspace(line[i]))
                i++;

            if (i < length && line[i] == COMMA) {
                print_error("Instruction \".data\" has multiple consecutive commas", file_name, line_num);
                return NULL;
            }
        }
    }
    if (last_was_comma) {
        print_error("Instruction \".data\" expects an integer after the last comma", file_name, line_num);
        return NULL;
    }
    result = (int *) malloc(temp_count * sizeof(int));
    if (result == NULL) {
        /* Indicates memory allocation failed (all other allocations were freed inside function) */
        fclose(file);
        exit(1); /* Exiting program */
    }
    memcpy(result, numbers, temp_count * sizeof(int));
    *num_count = temp_count;

    return result;
}

char *get_first_word(char *str) {
    char *first_word;
    /* Finding the length of the first word */
    int length = 0;
    while (str[length] != NULL_TERMINATOR && !isspace(str[length])) {
        length++;
    }
    /* Allocating memory for the first word */
    first_word = (char *) malloc((length + 1) * sizeof(char));
    if (first_word == NULL) {
        printf("Error: Memory allocation failed");
        return NULL; /* Indicates memory allocation failed */
    }

    strncpy(first_word, str, length); /* Copying the first word to the allocated memory */
    first_word[length] = NULL_TERMINATOR; /* Null-terminating the string */

    return first_word;
}

/* Checks if word appears alone in string */
int is_standalone_word(char *str, char *word) {
    size_t len = strlen(word);
    char *pos = strstr(str, word);
    while (pos) {
        if ((pos == str || isspace(pos[-1])) &&
            (isspace(pos[len]) || pos[len] == '\0'))
            return 1;
        pos = strstr(pos + len, word);
    }
    return 0;
}

void create_ob_file(char *file_ob_name, Code *code_head, Data *data_head, const int *IC, const int *DC) {
    FILE *file_ob = fopen(file_ob_name, "w");
    int i = IC_INITIAL, j = *IC;;
    if (file_ob == NULL) {
        /* Failed to open file for writing */
        printf("Error: Failed to open new file for writing");
        free_labels();

        exit(1); /* Exiting program */
    }
    /* Writing header into file */
    fprintf(file_ob, "%7d %d\n", (*IC) - IC_INITIAL, *DC);
    /* Writing machine code into file */
    for (; i < *IC; i++) {
        fprintf(file_ob, "%07d %06x\n", i, code_head->value);
        code_head = code_head->next;
    }
    for (; j < *DC + *IC; j++) {
        fprintf(file_ob, "%07d %06x\n", j, data_head->value);
        data_head = data_head->next;
    }

    fclose(file_ob);
}

void create_ent_file(char *file_ent_name) {
    FILE *file_ent = fopen(file_ent_name, "w");
    Symbol *current;

    if (file_ent == NULL) {
        /* Failed to open file for writing */
        printf("Error: Failed to open new file for writing");
        free_labels();

        exit(1); /* Exiting program */
    }

    current = get_label_head();
    while (current != NULL) {
        if (current->type == ENTRY) {
            fprintf(file_ent, "%s %07d\n", current->label, current->address);
        }
        current = current->next;
    }
    fclose(file_ent);
}

void create_ext_file(char *file_ext_name) {
    FILE *file_ext = fopen(file_ext_name, "w");
    Symbol *current;

    if (file_ext == NULL) {
        /* Failed to open file for writing */
        printf("Error: Failed to open new file for writing");
        free_labels();

        exit(1); /* Exiting program */
    }

    current = get_label_head();
    while (current != NULL) {
        if (current->type == EXTERN && current->address != 0) {
            fprintf(file_ext, "%s %07d\n", current->label, current->address);
        }
        current = current->next;
    }
    fclose(file_ext);
}

/* Prints error with filename and line number */
void print_error(char *msg, char *file, int line) {
    printf("Error in %s line %d: %s\n", file, line, msg);
}

void print_error_type(char *error_msg, char *file_name, int line_num, const char *type) {
    /* Printing the file label, assembly line number, specific error quoted and the error message */
    printf("Error in \"%s\" line %d for \"%s\": %s\n", file_name, line_num, type, error_msg);
}
