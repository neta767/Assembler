/*
 * This is the utility header file.
 * Provides memory tracking, string helpers, and reserved word validation.
 */

#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>

/* A node representing a tracked dynamically allocated memory block */
typedef struct TrackedBlock {
 void *ptr;
 struct TrackedBlock *next;
} TrackedBlock;


/* Line information struct (used for future parsing stages) */
typedef struct Line {
    FILE *file;          /* File pointer for error context */
    char *file_am_name;  /* File name for error printing */
    char *content;       /* Line content */
    int line_num;        /* Line number in original file */
} Line;

typedef enum Type {
    REGULAR,
    ENTRY,
    EXTERN,
    OPERAND
} Type;

typedef enum valid_methods {
    NONE,
    METHOD_1,
    METHODS_1_2,
    METHODS_1_3,
    METHODS_0_1_3,
    METHODS_1_2_3
} valid_methods;

/* Opcode metadata */
typedef struct Op_Code {
    char *operation;
    int code_num;
    int functs_num;
    int operands_num;
    valid_methods source_methods;
    valid_methods destination_methods;
} Op_Code;

/**
 * Allocates memory and tracks it for safe cleanup.
 * @param size - Number of bytes to allocate
 * @return Pointer to allocated memory, or NULL if failed
 */
void *allocate_memory(long size);

/**
 * Frees all memory allocated via allocate_memory
 */
void free_all_memory();

/**
 * Removes and frees a specific pointer from memory tracking
 * @param target_ptr - Pointer to remove and free
 */
void free_tracked_block(void *target_ptr);

/*
 * Trims leading and trailing whitespace from a string
 * @param str String to trim
 * @return Pointer to the trimmed string
 */
char *trim_whitespace(char *str);

/*
 * Checks if a word appears as a standalone token in a string
 * @param str The full line or string
 * @param word The word to search
 * @return 1 if found standalone, 0 otherwise
 */
int is_standalone_word(char *str, char *word);

/*
 * Checks if string contains whitespace characters
 * @param str String to check
 * @return 1 if found, 0 otherwise
 */
int contains_whitespace(char *str);

/*
 * Prints an error message with file name and line number
 * @param error_msg Message to print
 * @param file_name Name of the file
 * @param line_num Line number of the error
 */
void print_error(char *error_msg, char *file_name, int line_num);

/*
 * Validates a macro name against rules and reserved words
 * @param file_name Source file name
 * @param macro_name Macro name to validate
 * @param line_count Line number for error context
 * @return 0 if valid, 1 if invalid
 */
int valid_macro_name(char *file_name, char *macro_name, int line_count);

/*
 * Checks if a string is a reserved word (opcode, register, instruction)
 * @param file_name File name for context
 * @param str String to check
 * @param line_count Line number for context
 * @param type Type of usage (label, operand, etc.)
 * @return 1 if reserved, 0 if valid
 */
int is_reserved_word(char *file_name, char *str, int line_count, Type type);

/*
 * Returns index of the opcode if valid
 * @param str Word to check
 * @return Opcode index or -1
 */
int which_opcode(char *str);

/*
 * Returns index of the register if valid
 * @param str Word to check
 * @return Register index or -1
 */
int which_regis(char *str);

/*
 * Returns index of the instruction if valid
 * @param str Word to check
 * @return Instruction index or -1
 */
int which_instr(char *str);

#endif
