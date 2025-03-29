#include <stdio.h>

void print_error(const char *error_message, int  line){
    if (line != -1) {
        /* Print the error message along with the line number */
        printf("Line: %d , Error: %s\n", line , error_message);
    } else {
        /* Print the error message without a line number */
        printf("Error: %s\n", error_message);
    }
}
