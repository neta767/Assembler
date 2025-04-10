#ifndef FILE_H
#define FILE_H

/**
 * Adds an extension to a file name.
 * @param name - Source file name without extension
 * @param extension - The extension to append.
 * @return Pointer to the new file name with the extension, or NULL if an error occurred.
 */
char *add_extension(char *name,char *extension);

/**
 * Deletes a file from the filesystem.
 * @param filename - The name of the file to delete.
 */
void delete_file(char *filename);

#endif