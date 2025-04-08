#include "file.h"
#include "pre_proc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* 0 or NULL ? */

/*
 * Handles a single file by adding the ".as" extension, opening it,
 * passing it to the preprocessor, and cleaning up resources.
 */
int handle_file(char *filename){
    char *as_filename;
	FILE *file_pointer;

    /* Add .as extension to the filename */
	as_filename = add_extension(filename,".as");
    if (!as_filename)
        return 0;

    /* Try to open the file for reading */
    file_pointer = fopen(as_filename, "r");
    if (!file_pointer) {
        /* Print error if file cannot be opened */
        printf("Could not open file %s.as\n",filename);
        free(as_filename);
        return 0;
    }
    /* Perform pre+processing on the opened file */
    pre_process(filename, file_pointer);
    fclose(file_pointer);
    free(as_filename);
    return 0;
}

/*
 * Concatenates a file name with a given extension.
 * Returns a new string (must be freed by the caller).
 */
char * add_extension(char * filename , char * extension){
    size_t filename_len; /* Length of file Name */
    size_t extension_len; /* Length of expected filename extension */
    char *new_name;
    filename_len= strlen(filename); /* Length of file Name */
    extension_len = strlen(extension); /* Length of expected filename extension */

    /* Allocate space for new filename (+1 for null terminator) */
    new_name = allocate_memory((filename_len + extension_len + 1)*sizeof(char));  /* +1 for the null terminator */
    if (!new_name)  /* Indicates memory allocation failed */
        return 0;

    /* Copying the original filename and appending the extension */
    strcpy(new_name, filename);
    strcat(new_name, extension);
    return new_name;
}

/*
 * Allocates memory and checks for success.
 * Returns pointer or NULL if failed.
 */
void *allocate_memory(long size) {
    void *ptr = malloc(size);
    if (!ptr) {
        printf("Memory allocation failed");
        return 0;
    }
    return ptr;
}