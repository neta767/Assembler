#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "pre_proc.h"
#include "util.h"
#include "macros.h"
#include "definitions.h"
#include "file.h"

/** kind of error that will terminate the program
 * failed to add macro
 * failed to append macro content
 * */

/** kind of will move to the next file
 * file name is with ".as" extension
 * failed to open or read file
 * longer than 80 characters
 * comment that not start the line
 * in the line of mcro or mcroend contains more characters than allowed
 * macro name is not allowed
 * */


/**
 * Checks if the input name ends with ".as"
 * @param name - Source file name without extension
 * @return 1 if end with ".as",otherwise 0
 * */
static int is_valid_filename(char *name) {
    int len = strlen(name);
    return (len > 3 && strcmp(name + len - 3, ".as") == 0);
}

/**
 * Parses a macro declaration line and returns the macro name
 * @param line - The line to parse
 * @param file - Source file name
 * @return Pointer to the macro name, or NULL if invalid
 */
static char *parse_macro_line(char *line, char *file, int line_num) {
    char *name;
    if (!isspace(line[MCRO_LENGTH])) {
        print_error("Invalid macro start", file, line_num);
        return NULL;
    }
    name = trim_whitespace(line + MCRO_LENGTH);
    if (valid_macro_name(file, name, line_num))
        return NULL;
    return name;
}

/**
 * Processes a single line from the source file
 * @param line - The line to process
 * @param out - Output file pointer
 * @param src - Source file pointer
 * @param src_name - Source file name
 * @param out_name - Output file name
 * @param line_num - Current line number
 * @param in_macro - Flag indicating if currently in a macro
 * @param error - Error flag
 */
static void process_line(char *line, FILE *out, FILE *src, char *src_name, char *out_name, int *line_num, int *in_macro, int *error) {
    char copy[MAX_LINE_LENGTH], *trimmed, *name;

    /* If line is comment, write it to output */
    if (*line == COMMENT) {
        fputs(line, out);
        return;
    }

    strcpy(copy, line);
    trimmed = trim_whitespace(line);

    /* Check if comment is illegal */
    if (*trimmed == COMMENT) {
        print_error("Comment must start line", src_name, *line_num);
        *error = 1;
        return;
    }

    if (*in_macro) {
        /* Check if mcroend is valid */
        if (is_standalone_word(trimmed, "mcroend")) {
            if (strlen(trimmed) > MCROEND_LENGTH) {
                print_error("Bad mcroend", src_name, *line_num);
                *error = 1;
            }
            else {
                *in_macro = 0;
            }
            return;
        }
        /* Append content to macro */
        if (append_macro_content(copy)) {
            fclose(src);
            fclose(out);
            delete_file(out_name);
            free_macros();
            free_all_memory();
            exit(1);
        }
        return;
    }
    /* Append contect of macro to output file */
    if (is_macro_name(trimmed)) {
        fputs(is_macro_name(trimmed)->content, out);
        return;
    }

    if (is_standalone_word(trimmed, "mcro")) {
        name = parse_macro_line(trimmed, src_name,*line_num);
        if (!name) {
            *error = 1;
            return;
        }

        if (is_macro_name(name)) {
            print_error("Macro exists", src_name, *line_num);
            *error = 1;
            return;
        }

        if (add_macro(name)) {
            fclose(src);
            fclose(out);
            delete_file(out_name);
            free_macros();
            free_all_memory();
            exit(1);
        }
        *in_macro = 1;
        return;
    }
    fputs(copy, out);
    return;
}

/**
 * Handles macros in source and writes expanded output
 * @param src - Source file pointer
 * @param out - Output file pointer
 * @param src_name - Source file name
 * @param out_name - Output file name
 * @return 0 on success, 1 on failure
 */
int handle_macros(FILE *src, FILE *out, char *src_name, char *out_name) {
    char line[MAX_LINE_LENGTH];
    int line_num = 0, in_macro = 0, error = 0, ch;

    while (fgets(line, MAX_LINE_LENGTH, src)) {
        line_num++;

        if (strlen(line) == MAX_LINE_LENGTH - 1 && line[MAX_LINE_LENGTH - 2] != '\n') {
            print_error("Line too long", src_name, line_num);
            while ((ch = fgetc(src)) != '\n' && ch != EOF) {}
            error = 1;
            continue;
        }

        process_line(line, out,src, src_name, out_name, &line_num, &in_macro, &error);
    }

    return error;
}

/* Expands macro calls and creates an .am output file from a .as source */
int pre_proc(char *name) {
    char *src_name, *out_name;
    FILE *src, *out;

    if (is_valid_filename(name)) {
        printf("Error: omit '.as' extension\n");
        return 1;
    }

    src_name = add_extension(name, ".as");
    out_name = add_extension(name, ".am");

    src = fopen(src_name, "r");
    if (!src) {
        printf("Error: can't open %s\n", src_name);
        free_all_memory();
        return 1;
    }

    out = fopen(out_name, "w");
    if (!out) {
        printf("Error: can't create %s\n", out_name);
        fclose(src);
        free_all_memory();
        return 1;
    }

    if (handle_macros(src, out, src_name, out_name)) {
        fclose(src);
        fclose(out);
        delete_file(out_name);
        free_macros();
        free_all_memory();
        return 1;
    }

    fclose(src);
    fclose(out);
    free_macros();
    free_all_memory();
    /* TODO: free src_name and out_name */
    free_tracked_block(out_name);
    return 0;
}

