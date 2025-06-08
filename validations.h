#ifndef VALIDATIONS_H
#define VALIDATIONS_H
#include "code_list.h"
#include "data_list.h"
#include "symbols_list.h"
#include "util.h"

/**
 * Checks if a name is valid according to the assembler's rules.
 * @param name The name to be checked.
 * @param line_num The line number where the name is found.
 * @param file_name The name of the file containing the name.
 * @param type The type of the label (REGULAR, ENTRY, EXTERN, OPERAND).
 * @return 1 if the name is valid, 0 if not valid.
 */
int valid_name(char *name, int line_num, char *file_name, Type type);

/**
 * Determines if the given string is a valid opcode.
 * @param str The string to be checked.
 * @return The index of the opcode if valid, -1 if not valid.
 */
int get_instruct_id(const char *str);


/**
 * Determines if the given string is a valid register.
 * @param str The string to be checked.
 * @return The index of the register if valid, -1 if not valid.
 */
int get_regis(const char *str);


/**
 * Determines if the given string is a valid instruction.
 * @param str The string to be checked.
 * @return The index of the instruction if valid, -1 if not valid.
 */
int get_prompt(const char *str);


/**
 * Determines if the given string is a valid addressing method.
 * @param operand The string to be checked.
 * @param file_name The name of the file containing the operand.
 * @param line_num The line number where the operand is found.
 * @return The index of the addressing method if valid, -1 if not valid.
 */
int get_addressing_method(char *operand, char *file_name, int line_num);


/**
 * Determines if the given string is a reserved word.
 * @param file_name The name of the file containing the word.
 * @param str The word to be checked.
 * @param line_num The line number where the word is found.
 * @param type The type of the label (REGULAR, ENTRY, EXTERN, OPERAND).
 * @return 0 if the word is not reserved, 1 if it is reserved.
 */
int is_reserved_word(char *file_name, const char *str, int line_num, Type type);


/**
 * Checks if the given line contains a data prompt.
 * @param data_head Pointer to the data array to store the machine code.
 * @param usage Pointer to the usage counter.
 * @param DC Pointer to the data counter.
 * @param line_num The line number in the file.
 * @param file_name The name of the file being processed.
 * @param file  The file pointer associated with the line.
 * @param line The current position in the line.
 * @param current_word The current word being processed.
 * @param error Pointer to an integer to keep track of errors found.
 * @param label The label found in the line.
 * @return 1 if the line contains a data prompt, 0 if not.
 */
int is_data_prompt(Data **data_head, int *usage, int *DC, int line_num, char *file_name, FILE *file, char *line,
                   char *current_word, int *error, char *label);


/**
 * Checks if the given line contains an instruction.
 * @param code_head Pointer to the code array to store the machine code.
 * @param usage Pointer to the usage counter.
 * @param IC Pointer to the instruction counter.
 * @param line The current position in the line.
 * @param line_num The line number in the file.
 * @param file The file pointer associated with the line.
 * @param file_name The name of the file being processed.
 * @param current_word The current word being processed.
 * @param error Pointer to an integer to keep track of errors found.
 * @param label The label found in the line.
 * @return 1 if the line contains an instruction, 0 if not.
 */
int is_instruction(Code **code_head, int *usage, int *IC, char *line, int line_num, FILE *file, char *file_name,
                   char *current_word, int *error, char *label);


/**
 * Checks if an instruction is valid.
 * @param file_name The name of the file containing the symbol.
 * @param line_num The line number where the symbol is found.
 * @param method The method of the symbol.
 * @param instruct_id The instruction ID of the symbol.
 * @param operands_num The number of operands in the instruction.
 * @return 1 if the line contains a symbol, 0 if not.
 */
int is_method_legal(char *file_name, int line_num, int method, int instruct_id, int operands_num);

/**
 * Handles the data found in the given line.
 * @param data_head Pointer to the data array to store the machine code.
 * @param usage Pointer to the usage counter.
 * @param DC Pointer to the data counter.
 * @param line_nums The line number in the file.
 * @param file_name The name of the file being processed.
 * @param file The file pointer associated with the line.
 * @param line
 * @param error Pointer to an integer to keep track of errors found.
 * @param current_word The current word being processed.
 * @return 1 if the line contains data, 0 if not.
 */
int is_data(Data **data_head, int *usage, int *DC, int line_nums, char *file_name, FILE *file, char *line, int *error,
            const char *current_word);


/**
 * Handles the string found in the given line.
 * @param data_head Pointer to the data array to store the machine code.
 * @param usage Pointer to the usage counter.
 * @param DC Pointer to the data counter.
 * @param line_nums The line number in the file.
 * @param file_name The name of the file being processed.
 * @param label The label found in the line.
 * @param error Pointer to an integer to keep track of errors found.
 * @param current_word The current word being processed.
 * @return 1 if the line contains a string, 0 if not.
 */
int is_string(Data **data_head, int *usage, int *DC, int line_nums, char *file_name, char *label, int *error,
              const char *current_word);

/**
 * Handles the extern found in the given line.
 * @param line_num The line number in the file.
 * @param file_name The name of the file being processed.
 * @param file The file pointer associated with the line.
 * @param line The current position in the line.
 * @param error Pointer to an integer to keep track of errors found.
 * @param current_word The current word being processed.
 * @return 1 if the line contains extern, 0 if not.
 */
int is_extern(int line_num, char *file_name, FILE *file, char *line, int *error, char *current_word);


/**
 * Validates the instruction found in the given line.
 * @param code_head Pointer to the code array to store the machine code.
 * @param usage Pointer to the usage counter.
 * @param IC Pointer to the instruction counter.
 * @param instruct_id The instruction ID of the symbol.
 * @param error Pointer to an integer to keep track of errors found.
 * @param file_name The name of the file being processed.
 * @param line_num The line number where the symbol is found.
 * @param file The file pointer associated with the line.
 * @param line The current position in the line.
 * return 1 if the instruction is valid, 0 if not.
 */
int valid_instruction(Code **code_head, int *usage, int *IC, int instruct_id, int *error, char *file_name, int line_num
                      , FILE *file, char *line);


/**
 * Analyzes the numbers in the ".data" instruction line and encodes them into machine code.
 * @param data_head Pointer to the data array to store the machine code.
 * @param usage Pointer to the usage counter for memory.
 * @param DC Pointer to the data counter.
 * @param line_num The line number in the file.
 * @param file_name The name of the file being processed.
 * @param file The file pointer associated with the line.
 * @param line The current position in the line.
 * @param error Pointer to the error counter.
 * @return 1 if the line contains numbers, 0 if not.
 */
int analyze_numbers(Data **data_head, int *usage, int *DC, int line_num, char *file_name, FILE *file, char *line,
                    int *error);


#endif
