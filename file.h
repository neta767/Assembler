int handle_file(char *filename);

/**
 * Adds an extension to a file name.
 * @param filename The original file name.
 * @param extension The extension to append.
 * @return Pointer to the new file name with the extension, or NULL if an error occurred.
 */
char *add_extension(char *filename,char *extension);

/**
* Allocates memory in the required size. Exits the program if failed.
 * @param size The size to allocate in bytes
 * @return A generic pointer to the allocated memory if succeeded
 */
void *allocate_memory(long size);

