#include <ctype.h>

#include "assembler.h"
#include "macro.c"

void remove_white_space(char *str) {
    int i = 0, j = 0;

    while (str[i]) {
        if (!isspace((unsigned char)str[i])) {
            str[j++] = str[i];
        }
        i++;
    }
    str[j] = '\0'; // Null-terminate the modified string
}

void add_macro() {

}


void handle_line(char *line,FILE *as_file_pointer, FILE *am_file_pointer, int i) {
    int in_macro = 0;
    char macro_name[MAX_LINE_LEN];
    remove_white_space(line);

    /* Check if the line is a macro declaration */
    if(strncmp(line , "macr" , strlen("macr")) == 0){
        sscanf(line, "macr %s", macro_name); // Extract macro name
        add_macro(macro_name);
        in_macro=1;
    }
    else if(in_macro) {
        add_macro_content(line);
    }
    /* Check if the line marks the end of a macro */
    else if(strncmp(line , "endmacr" , strlen("endmacr")) == 0){
        in_macro=0;
    }
    /* Check if the line calls an existing macro */
    else if(find_macro(head , line)){

    }
    else {
        fprintf(am_file_pointer, "%s", line);
    }
}

int *pre_process(char *filename,FILE *as_file_pointer){
    int i=0, line_length=0;
    char line[MAX_LINE_LEN]; /* Buffer for reading lines from the file */
    char *am_filename = add_extension(filename, ".am");
    FILE *am_file_pointer =fopen(am_filename,"w");
    if (!am_file_pointer) {
        /* Check if file exist and readable */
        printf("Could not open file %s\n",filename);
        fclose(am_file_pointer);
        free(filename);
        return 0;
    }
    /* Loop through each line in the file */
    while (fgets(line, sizeof(line), as_file_pointer)) {
        line_length = strlen(line);

        /* Validating line length */
        if (line_length == MAX_LINE_LEN && line[MAX_LINE_LEN - 1] == '\n')
            printf("line number %d in %s is too long\n", i, filename);
        handle_line(line, as_file_pointer, am_file_pointer, i);
        i++;
    }
    // Free allocated memory
    free_macros();

    return 0;
}