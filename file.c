#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

#include "util.h"

/* Adds an extension to a file name. */
char *add_extension(char *name, char *extension) {
    char *new_name;
    long name_len = strlen(name);
    long extension_len = strlen(extension);

    /* Allocating memory for the new name */
    new_name = (char *)allocate_memory(name_len + extension_len + 1);  /* +1 for the null terminator */
    if (new_name == NULL)
        exit(1);
    /* Copying the original name and appending the extension */
    strcpy(new_name, name);
    strcat(new_name, extension);
    return new_name;
}

/* Deletes a file from the filesystem. */
void delete_file(char *filename) {
    if (remove(filename) != 0)
        printf("Error: Failed to delete %s file",filename);
}