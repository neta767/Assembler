#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "code_list.h"
#include "data_list.h"
#include "util.h"

/**
 * Performs the first pass of the assembler.
 * Scans the file to identify and store instructions, data definitions, and labels.
 * If no errors are detected, it proceeds to the second pass.
 *
 * @param file_name The name of the file to process.
 * @param data_head Pointer to the pointer to the head of the data list.
 * @param code_head Pointer to the pointer to the head of the code list.
 * @param IC Pointer to the Instruction Counter.
 * @param DC Pointer to the Data Counter.
 * @return 0 if successful, 1 if errors were detected.
 */
int first_pass(char *file_name, Data **data_head, Code **code_head, int *IC, int *DC);

/**
 * Scans the given file and processes each line to identify and handle
 * instructions, data, and labels.
 *
 * @param file_name The name of the file to scan.
 * @param data_head Pointer to the pointer to the data list head.
 * @param code_head Pointer to the pointer to the code list head.
 * @param IC Pointer to the Instruction Counter.
 * @param DC Pointer to the Data Counter.
 * @return 0 if no errors were detected, 1 otherwise.
 */
int scan_am_file(char *file_name, Data **data_head, Code **code_head, int *IC, int *DC);

/**
 * Processes a single line of the file to identify and handle instructions,
 * data declarations, and labels.
 *
 * @param code_head Pointer to the pointer to the code list head.
 * @param data_head Pointer to the pointer to the data list head.
 * @param usage Pointer to a usage counter.
 * @param IC Pointer to the Instruction Counter.
 * @param DC Pointer to the Data Counter.
 * @param line_num The current line number.
 * @param file_name The name of the file being processed.
 * @param line The line of text being processed.
 * @param error Pointer to the error flag.
 * @param file File pointer for reading additional content if needed.
 */
void process_each_line(Code **code_head, Data **data_head, int *usage, int *IC, int *DC,
                       int line_num, char *file_name, char *line, int *error, FILE *file);

/**
 * Checks whether the current word is a valid symbol (label) and processes it.
 *
 * @param current_word The current word being processed.
 * @param line_num The current line number.
 * @param file_name The name of the file.
 * @param file File pointer to the current file.
 * @param line The full line being parsed.
 * @param label Buffer to store the detected label name.
 * @return 1 if it's a valid symbol, 0 otherwise.
 */
int is_symbol(char *current_word, int line_num, char *file_name, FILE *file, char *line, char *label);


/**
 * Cleans up all allocated resources and exits the program.
 *
 * @param file File pointer to close.
 * @param file_name Name of the file being processed.
 * @param data_head Pointer to the pointer to the head of the data list.
 * @param code_head Pointer to the pointer to the head of the code list.
 * @param current_word Pointer to the current word being processed.
 */
void cleanup_and_exit(FILE *file, char *file_name, Data **data_head, Code **code_head, char *current_word);

#endif
