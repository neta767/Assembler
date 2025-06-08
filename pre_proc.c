/**
 * @file pre_proc.c
 * @brief Preprocessor for assembly language files.
 * This file contains functions to handle macro definitions and expansions,
 * as well as file handling for the source and output files.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pre_proc.h"
#include "validations.h"
#include "util.h"
#include "macro_list.h"
#include "const.h"

/* Expands macro calls and creates an .am output file from a .as source */
int pre_proc(char *name) {
    char *src_name, *out_name; /* Source and output file names */
    FILE *src, *out; /* Source and output file pointers */
    Macro *head = NULL; /* Defining the head of the macros linked list */

    if (!is_invalid_filename(name)) {
        printf("Error: file label should be without \".as\" extension\n");
        return 1;
    }

    src_name = add_extension(name, ".as");
    out_name = add_extension(name, ".am");

    /* opening/creating files */
    src = fopen(src_name, "r");
    if (!src) {
        printf("Error: can't open %s\n", src_name);
        free(src_name);
        free(out_name);
        return 1;
    }
    out = fopen(out_name, "w");
    if (!out) {
        printf("Error: can't create %s\n", out_name);
        fclose(src);
        free(src_name);
        free(out_name);
        return 1;
    }

    if (scan_as_file(src, out, src_name, out_name, &head)) {
        delete_file(out_name);
        cleanup(src, out, src_name, out_name, &head);
        return 1;
    }
    cleanup(src, out, src_name, out_name, &head);
    return 0; /* Indicates success */
}

/* Checks if the input name ends with ".as" */
int is_invalid_filename(const char *name) {
    size_t len = strlen(name);
    return (len < 4 || strcmp(name + len - 3, ".as") != 0);
}

/* Handles macros in source and writes expanded output */
int scan_as_file(FILE *src, FILE *out, char *src_name, char *out_name, Macro **head) {
    char line[MAX_LINE_LENGTH]; /* Buffer to store each line */
    int line_num = 0; /* Current line number */
    int in_macro = 0; /* Flag indicating if currently in a macro */
    int error = 0; /* Error flag */
    int ch;

    /* loop through each line of the source file */
    while (fgets(line, MAX_LINE_LENGTH, src)) {
        line_num++;
        /* Check if line is too long */
        if (strlen(line) == MAX_LINE_LENGTH - 1 && line[MAX_LINE_LENGTH - 2] != '\n') {
            print_error("Line too long", src_name, line_num);
            while ((ch = fgetc(src)) != '\n' && ch != EOF) {
            }
            error = 1;
            continue;
        }

        process_line(line, out, src, src_name, out_name, &line_num, &in_macro, &error, head);
    }

    return error;
}

/* Processes a single line from the source file */
void process_line(char *line, FILE *out, FILE *src, char *src_name, char *out_name, const int *line_num, int *in_macro,
                  int *error, Macro **head) {
    char trimmed_line[MAX_LINE_LENGTH]; /* trimmed line of the line */
    char *macro_name = NULL; /* Macro aname */
    char *current_word; /* Current word being processed */
    Macro *macro;
    strcpy(trimmed_line, line);
    trim_whitespace(trimmed_line);

    /* If line is comment, write it to output */
    if (*line == COMMENT) {
        fputs(line, out);
        return;
    }

    /* Check if comment is illegal */
    if (*trimmed_line == COMMENT && strlen(trimmed_line) > 1) {
        print_error("Comment must start the line", src_name, *line_num);
        *error = 1;
        return;
    }

    /* Check if line is a macro call */
    macro = is_macro_name(trimmed_line, *head);
    if (macro) {
        fputs(macro->content, out);
        return;
    }

    /* Check if line is a macro declaration */
    if (is_standalone_word(trimmed_line, MACRO_START)) {
        /* Check if there is extra text */
        macro_name = parse_macro_line(trimmed_line, src_name, *line_num, head);
        if (!macro_name) {
            *error = 1;
            return;
        }
        /* Add macro to list */
        if (add_macro(macro_name, head)) {
            cleanup(src, out, src_name, out_name, head);
            exit(1);
        }
        *in_macro = 1;
        return;
    }

    /* if currently in a macro, check for macro-end */
    if (*in_macro) {
        /* Check if macro-end is valid */
        if (is_standalone_word(trimmed_line, MACRO_END)) {
            /* Check if there is extra text */
            if (strlen(trimmed_line) > strlen(MACRO_END)) {
                print_error("\"macroend\" should be without extra text", src_name, *line_num);
                *error = 1;
            } else {
                *in_macro = 0;
            }
            return;
        }
        /* Append content to macro */
        if (append_macro_content(line, *head)) {
            cleanup(src, out, src_name, out_name, head);
            exit(1);
        }
        return;
    }

    current_word = get_first_word(line);
    if (current_word == NULL) {
        /* Indicates memory allocation failed */
        cleanup(src, out, src_name, out_name, head);
        free(current_word);
        exit(1);
    }

    /* Checking for a potential label definition */
    if (current_word[strlen(current_word) - 1] == COLON) {
        /* Checking if the label is a macro label */
        if (is_macro_name(current_word, *head) != NULL) {
            print_error("Invalid label declaration: a label cannot be the same as a macro label", src_name, *line_num);
            free(current_word);
            *error = 1;
            return;
        }
    }
    free(current_word);
    fputs(line, out);
}

char *parse_macro_line(char *line, char *file, int line_num, Macro **head) {
    char *name;
    if (!isspace(line[4])) {
        print_error("Invalid macro declaration", file, line_num);
        return NULL;
    }
    name = trim_whitespace(line + 4);
    if (contains_whitespace(name)) {
        print_error("Macro name must be one word", file, line_num);
        return NULL;
    }
    /* Check if macro name is valid */
    if (is_macro_name(name, *head)) {
        print_error("Macro name is already in use", file, line_num);
        return NULL;
    }
    if (!valid_name(name, line_num, file, MACRO)) {
        return NULL;
    }
    return name;
}

void cleanup(FILE *src, FILE *out, char *src_name, char *out_name, Macro **head) {
    fclose(src);
    fclose(out);
    free_macros(head);
    free(src_name);
    free(out_name);
}

