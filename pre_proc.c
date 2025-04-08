#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "file.h"
#include "pre_proc.h"
#include "macro.h"

int pre_proc(char *filename, FILE *source) {
    char line[MAX_LINE_LEN];           /* Buffer for reading lines from the file */
    char *output_name;                 /* Output file name with .am extension */
    FILE *output;
    int macro_start_line = 0;
    int line_num = 0, in_macro = 0, has_errors = 0, i;
    char start[MAX_NAME];
    char name[MAX_NAME];
    char *token;
    Macro *m;
	char *oldstr;

    Macro *current_macro = NULL;   /* Pointer to the current macro being created */

    /* Generate the .am output filename */
    output_name = add_extension(filename, ".am");
    if (!output_name) {
        printf("Memory allocation failed for output filename.\n");
        return 1;
    }

    /* Open the output file for writing */
    output = fopen(output_name, "w");
    if (!output) {
        printf("Could not open file %s for writing\n", output_name);
        free(output_name);
        return 1;
    }

    /* Loop through each line in the file */
    while (fgets(line, sizeof(line), source)) {
		oldstr=malloc(sizeof(line));
        strcpy(oldstr,line);
        line_num++;
        /* Get first token (ignoring whitespace) */
        token = strtok(line, " \t\n");
		printf("hiiii  %s \n", line);
        if (!token) continue;  /* Skip empty lines */

        strcpy(start, token);

        /* If inside a macro definition */
        if (in_macro) {
            if (strcmp(start, "mcroend") == 0) {
                /* Check for extra tokens after endmcro */
                if (strtok(NULL, " \t\n")) {
                    fprintf(stderr, "Error on line %d: 'endmcro' should not contain extra tokens.\n", line_num);
                    has_errors++;
                    continue;
                } else {
                    in_macro = 0;
                    add_macro(current_macro);
                }
            } else {
                add_line_to_macro(current_macro, oldstr);
            }
            continue;
        }

        /* Start of a new macro */
        if (strcmp(start, "mcro") == 0) {
            token = strtok(NULL, " \t\n");
            if (!token || strtok(NULL, " \t\n")) {
                fprintf(stderr, "Error on line %d: Invalid macro declaration format.\n", line_num);
                has_errors++;
                continue;
            }

            strcpy(name, token);

            if (!is_valid_macro_name(name)) {
                fprintf(stderr, "Error on line %d: Invalid macro name '%s'.\n", line_num, name);
                has_errors++;
            } else if (is_reserved(name)) {
                fprintf(stderr, "Error on line %d: Macro name '%s' is a reserved word.\n", line_num, name);
                has_errors++;
            }

            /* Create new macro */
            current_macro = malloc(sizeof(Macro));
            if (!current_macro) {
                fprintf(stderr, "Memory allocation failed for macro.\n");
                has_errors++;
                continue;
            }

            strcpy(current_macro->name, name);
            current_macro->num_lines = 0;
            current_macro->capacity = 10;
            current_macro->lines = malloc(current_macro->capacity * sizeof(char *));
            current_macro->next = NULL;

            in_macro = 1;
            macro_start_line = line_num;
            continue;
        }

        /* Not inside macro: check if line is macro call */
        m = find_macro(start);
        if (m) {
            printf("Macro '%s' found, expanding...\n", start);
            /* If it's a macro call, expand it */
            for (i = 0; i < m->num_lines; ++i) {
                fputs(m->lines[i], output);  /* Insert macro content */
            }
        } else {
            /* Copy original line */

            fputs(oldstr, output);
        }
    }

    /* Cleanup */
    fclose(output);
    free(output_name);
    free_macros();

    return 0; /* success */
}