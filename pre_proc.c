#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pre_process(char *filename,FILE *file_pointer){
    int MAX_LINE_LEN = 82;/* +2 to accommodate '\0' */
    int i=0, line_length=0;
    char line[MAX_LINE_LEN]; /* Buffer for reading lines from the file */
    char *token; /* String to store tokenized data */

    /* Loop through each line in the file */
    while (fgets(line, sizeof(line), file_pointer)) {
        line_length = strlen(line);

        /* Validating line length */
        if (line_length == MAX_LINE_LEN && line[MAX_LINE_LEN-1] != '\n') {
            if (is_standalone_word(line,"macr") != 0) {
                if (macro_found == 0) {
                    macro_found = 1;
                } else {
                    macro_found = 0;
                }
            }
            print_syntax_error(Error_7,file_name,line_count);
            errors_found = 1;
            while ((ch = fgetc(file)) != '\n' && ch != EOF);  /* Clearing the rest of the line from the buffer */
            continue;  /* Skipping to the next line */
        }
    }
}
