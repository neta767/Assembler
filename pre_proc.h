#ifndef PRE_PROC_H
#define PRE_PROC_H
#include "macro_list.h"

/**
 * Preprocesses an assembly source file:
 * Expands macro calls and creates an .am output file from a .as source.
 * @param name - Source file name without extension
 * @return 0 on success, 1 on failure
 */
int pre_proc(char *name);

/**
 * Checks if the input label ends with ".as"
 * @param name - Source file name without extension
 * @return 0 if the filename ends with ".as", otherwise returns 1
 */
int is_invalid_filename(const char *name);

/**
 * Handles macros in source and writes expanded output
 * @param src - Source file pointer
 * @param out - Output file pointer
 * @param src_name - Source file name
 * @param out_name - Output file name
 * @param head - Pointer to the head of the macro linked list
 * @return 0 on success, 1 on failure
 */
int scan_as_file(FILE *src, FILE *out, char *src_name, char *out_name, Macro **head);

/**
 * Processes a single line from the source file
 * @param line - The line to process
 * @param out - Output file pointer
 * @param src - Source file pointer
 * @param src_name - Source file name
 * @param out_name - Output file name
 * @param line_num - Current line number
 * @param in_macro - Flag indicating if currently in a macro
 * @param error - Error flag
 * @param head - Pointer to the head of the macro linked list
 */
void process_line(char *line, FILE *out, FILE *src, char *src_name, char *out_name, const int *line_num,
                  int *in_macro, int *error, Macro **head);

/**
 * Parses a macro declaration line and returns the macro label
 * @param line - The line to parse
 * @param file - Source file name
 * @param line_num - The current line number
 * @param head - Pointer to the head of the macro linked list
 * @return Pointer to the macro name, or NULL if invalid
 */
char *parse_macro_line(char *line, char *file, int line_num, Macro **head);

/**
 * Cleans up resources and exits after an error or completion
 * @param src - Source file pointer
 * @param out - Output file pointer
 * @param src_name - Source file name
 * @param out_name - Output file name
 * @param head - Pointer to the head of the macro linked list
 */
void cleanup(FILE *src, FILE *out, char *src_name, char *out_name, Macro **head);

#endif
