#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* 0 or NULL */
int handle_file(char *filename){
    char *as_filename;
	FILE *file_pointer;
	as_filename = add_extension(filename,".as");
    if (!as_filename)
        return 0;
    file_pointer = fopen(as_filename, "r");
    if (!file_pointer) {
        /* Check if file exist and readable */
        printf("Could not open file %s.as\n",filename);
        free(as_filename);
        return 0;
    }

    fclose(file_pointer);
    free(as_filename);
    return 0;
}

/* Using void * for compatibility with different data types */
void *allocate_memory(long size) {
    void *ptr = malloc(size);
    if (!ptr) {
        printf("Memory allocation failed");
        return 0;
    }
    return ptr;
}

char * add_extension(char * filename , char * extension){
    size_t filename_len; /* Length of file Name */
    size_t extension_len; /* Length of expected filename extension */
	char *new_name;
	filename_len= strlen(filename); /* Length of file Name */
    extension_len = strlen(extension); /* Length of expected filename extension */

    /* Allocating memory for the new filename */
	new_name = allocate_memory((filename_len + extension_len + 1)*sizeof(char));  /* +1 for the null terminator */
    if (!new_name)  /* Indicates memory allocation failed */
        return 0;

    /* Copying the original filename and appending the extension */
    strcpy(new_name, filename);
    strcat(new_name, extension);
    return new_name;
}