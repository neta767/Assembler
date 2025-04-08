#define MAX_LINE_LEN 82
#include <stdio.h>

/**
* Executes the first pass of the assembly file parsing and processing.
* * This function reads an assembly source file, processes macros, and writes the
 * processed content to a new file with ".am" suffix. It also handles macro
 * declaration and calls, while reporting errors and cleaning up as necessary.
 * @param file_name The name of the assembly file to process.
 * @return An integer indicating whether an error occurred during the first pass.
 *         - If no error occurred, it returns 0.
 *         - If an error occurred, it returns a non-zero value.
 */
int *pre_process(char *filename,FILE *file_pointer);
