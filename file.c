
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * add_suffix(char * file_name , char * ending){
    char *new_name = (char *)malloc(strlen(file_name) + strlen(ending) + 1); //need to check malloc??
    strcpy(new_name,file_name);
    strcat(new_name , ending);
    return new_name;
}

char handel_file(char *input_file_name){
    input_file_name = add_suffix(input_file_name,".as");
    FILE *input_file_pointer = fopen(input_file_name, "r");
    if (input_file_pointer == NULL) {
        /* Check if file exist and readable */
        printf("Could not open file %s\n",input_file_name);
        free(input_file_name);
        return 0;
    }
    return 0;
}
