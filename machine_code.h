#ifndef MACHINE_CODE_H
#define MACHINE_CODE_H
#include "code_list.h"
#include "data_list.h"
#include "util.h"

/**
 * Adds a data code to the data array.
 * Converts the given number to its 24-bit complement binary representation and adds it to the array.
 * @param data_head Pointer to the data array.
 * @param DC Pointer to the data counter.
 * @param number The number to be added as data code.
 */
void add_data_code(Data **data_head, int *DC, int number);


/**
 * Adds an instruction code to the code array.
 * Checks for memory limits before adding the code to the array.
 * @param code_head Pointer to the array holding the instruction code.
 * @param usage Pointer to the usage counter for memory.
 * @param IC Pointer to the instruction counter.
 * @param word The instruction code to be added.
 * @param error Pointer to the error counter.
 */
void add_instruction_code(Code **code_head, int *usage, int *IC, unsigned int word, int *error);

/**
 * Processes and encodes the instruction code for a given operand using bit-wise operations.
 * Handles different addressing methods and updates the machine code accordingly.
 * @param code_head Pointer to the array holding the machine code.
 * @param usage Pointer to the usage counter for memory.
 * @param IC Pointer to the instruction counter.
 * @param file Pointer to the file for error reporting.
 * @param method The addressing method of the operand.
 * @param operand The operand string.
 * @param operands_num Number of operands in the instruction.
 * @param error Pointer to the error counter.
 */
void process_instruction_code(Code **code_head, int *usage, int *IC, FILE *file, int method, char *operand,
                              int operands_num, int *error);


/**
 * Handles the encoding and processing of an instruction with one operand.
 * Generates machine code for the instruction and manages the different addressing methods for the operand.
 * @param code_head Pointer to the array holding the machine code.
 * @param usage Pointer to the usage counter for memory.
 * @param IC Pointer to the instruction counter.
 * @param file Pointer to the file for error reporting.
 * @param src_operand The source operand string.
 * @param dest_operand
 * @param instruct_id Index of the instruction in the opcode table.
 * @param error Pointer to the error counter.
 * @param src_method
 * @param dest_method
 */
void handle_two_operands(Code **code_head, int *usage, int *IC, FILE *file, char *src_operand, char *dest_operand,
                         int instruct_id, int *error, int src_method, int dest_method);

/**
 * Handles the encoding and processing of an instruction with two operands.
 * Generates machine code for the instruction and manages the different addressing methods for the operands.
 * @param code_head Pointer to the array holding the machine code.
 * @param usage Pointer to the usage counter for memory.
 * @param IC Pointer to the instruction counter.
 * @param file Pointer to the file for error reporting.
 * @param method The addressing method of the operand.
 * @param operand The operand string.
 * @param instruct_id Index of the instruction in the opcode table.
 * @param error Pointer to the error counter.
 */
void handle_one_operand(Code **code_head, int *usage, int *IC, FILE *file, int method, char *operand, int instruct_id,
                        int *error);

#endif
