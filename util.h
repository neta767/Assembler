/**
 * This is the util header file.
 */
#ifndef UTIL_H
#define UTIL_H
#include <stdio.h>

#include "code_list.h"
#include "data_list.h"


/**
 * Deletes a file from the filesystem.
 * @param filename The name of the file to delete.
 */
void delete_file(char *filename);


/**
 * Adds an extension to a file label.
 * @param name - Source file name without extension
 * @param extension - The extension to append.
 * @return Pointer to the new file name with the extension, or NULL if an error occurred.
 */
char *add_extension(char *name, char *extension);


/**
 * Trims leading and trailing whitespace from a string.
 * @param str The string to trim.
 */
char *trim_whitespace(char *str);


/**
 * Checks if a string contains whitespace characters.
 * @param str The string to check.
 * @return 1 if whitespace is found, 0 otherwise.
 */
int contains_whitespace(char *str);


/**
 * Parses the numbers from the given line and returns them as an array.
 * @param file_name The name of the file being processed.
 * @param line_num The line number in the file.
 * @param file The file pointer associated with the line.
 * @param line Pointer to the structure representing the current line of code being processed.
 * @param line The current position in the line.
 * @param num_count Pointer to an integer to store the count of numbers found.
 * @return Pointer to an array of integers containing the parsed numbers.
 */
int *get_numbers(char *file_name, int line_num, FILE *file, char *line, int *num_count);


/**
 * Gets the first word from a string.
 * @param str The string to extract the first word from.
 * @return Pointer to the first word, or NULL if memory allocation failed.
 */
char *get_first_word(char *str);


/**
 * Checks if a word is a standalone word in a string.
 * @param str The string to search in.
 * @param word The word to search for.
 * @return 1 if the word is found, 0 otherwise.
 */
int is_standalone_word(char *str, char *word);


/**
 * Creates an object file (.ob) with machine code.
 * @param file_ob_name The name of the object file to create.
 * @param code_head Pointer to the head of the code list.
 * @param data_head Pointer to the head of the data list.
 * @param IC Pointer to the instruction counter.
 * @param DC Pointer to the data counter.
 */
void create_ob_file(char *file_ob_name, Code *code_head, Data *data_head, const int *IC, const int *DC);


/**
 * Creates an entry file (.ent) with entry labels.
 * @param file_ent_name The name of the entry file to create.
 */
void create_ent_file(char *file_ent_name);


/**
 * Creates an external file (.ext) with external labels.
 * @param file_ext_name The name of the external file to create.
 */
void create_ext_file(char *file_ext_name);


/**
 * Prints an error message with file label and line number
 * @param error_msg Message to print
 * @param file_name Name of the file
 * @param line_num Line number of the error
 */
void print_error(char *error_msg, char *file_name, int line_num);

/**
 * Prints an error message with file label, line number, and type
 * @param error_msg Message to print
 * @param file_name Name of the file
 * @param line_num Line number of the error
 * @param type Type of the error (e.g., "ERROR", "WARNING")
 */
void print_error_type(char *error_msg, char *file_name, int line_num, const char *type);

#endif
