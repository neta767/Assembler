
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *allocate_memory(long size) {
    void *ptr = malloc(size);
    if (!ptr) {
        printf("Memory allocation failed");
        return 0;
    }
    return ptr;
}

char * add_extension(char * filename , char * extension){
    char *new_name;
    size_t filename_len= strlen(filename);; /* Length of file Name */
    size_t extension_len = strlen(extension); /* Length of expected filename extension */

    /* Checking if the file name already has the extention */
    if (extension_len < filename_len) {
        if (strcmp(filename + filename_len - extension_len, extension) == 0) {
            printf("\n");
            return 0;
        }
    }
    /* Allocating memory for the new filename */
    new_name = (char *)allocate_memory(filename_len + extension_len + 1);  /* +1 for the null terminator */
    if (!new_name)  /* Indicates memory allocation failed */
        return 0;

    /* Copying the original filename and appending the extension */
    strcpy(new_name, filename);
    strcat(new_name, extension);
    return new_name;
}

char * handel_file(char *filename){
    char *as_filename;
    as_filename = add_extension(filename,".as");
    if (!as_filename)
        return 0;
    FILE *file_pointer = fopen(as_filename, "r");
    if (!file_pointer) {
        /* Check if file exist and readable */
        printf("Could not open file %s\n",filename);
        free(filename);
        return 0;
    }
    pre_process(filename,file_pointer);
    fclose(file_pointer);
    free(filename);
    return 0;
}
