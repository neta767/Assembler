#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#include "code_list.h"
#include "data_list.h"

/**
 * This function performs the second pass of the assembler.
 * @param file_name The name of the input file.
 * @param data_head Pointer to the head of the data linked list.
 * @param code_head Pointer to the head of the code linked list.
 * @param IC Pointer to the instruction counter.
 * @param DC Pointer to the data counter.
 * @return 0 for a successful instruction, 1 if errors were detected.
 */
int second_pass(char *file_name, Data *data_head, Code *code_head, const int *IC, const int *DC);


/**
 * @param file_am_name The name of the input file after pre-processing.
 * @param code_head Array containing the instruction code.
 * @return 0 if no errors were detected, 1 if errors were detected.
 */
int code_operand_labels(char *file_am_name, Code *code_head);


/**
 * @param file_name The name of the input file.
 */
int scan_file(char *file_name);

/**
 * @param line_num The current line number.
 * @param file_name The name of the input file.
 * @param line The current line being processed.
 * @param error Pointer to an integer that will be set to 1 if an error is detected.
 * @param file Pointer to the output file.
 */
void process_the_line(int line_num, char *file_name, char *line, int *error, FILE *file);

#endif
