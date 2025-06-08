#ifndef CONST_H
#define CONST_H

/* Enums */
typedef enum Addressing_Method {
    IMMEDIATE,
    DIRECT,
    RELATIVE,
    DIRECT_REGISTER
} Addressing_Method;

typedef enum valid_methods {
    NONE,
    METHOD_1,
    METHODS_1_3,
    METHODS_1_2,
    METHODS_0_1_3
} valid_methods;

/* Instruction Struct */
typedef struct instruction {
    char *instruction;
    int opcode;
    int funct;
    int operands_num;
    valid_methods source_methods;
    valid_methods destination_methods;
} instruction;

/* External Declarations */
extern const instruction INSTRUCTIONS[];
extern const char *REGISTERS[];
extern const char *PROMPTS[];
extern const char *TYPES[];

/* Useful Constants */
#define NO_OPERANDS 0
#define EXTENSION_LEN 3
#define MAX_LINE_LENGTH 82
#define MAX_DECLARATION_LENGTH 31
#define REGISTERS_COUNT 8
#define INSTRUCTIONS_COUNT 16
#define PROMPTS_COUNT 4
#define MACRO_START "mcro"
#define MACRO_END "mcroend"
#define CAPACITY 2097152
#define DC_INITIAL 0
#define IC_INITIAL 100
#define DECIMAL_BASE 10
#define TWO 2

/* A bit Encoding Positions */
#define OPCODE_POS 18
#define SRC_OPERAND_POS 16
#define DST_OPERAND_POS 11
#define SRC_REGISTER_POS 13
#define DST_REGISTER_POS 8
#define FUNCS_POS 3

/* Bit Masks */
#define BIT_MASK_EXTERNAL 1
#define BIT_MASK_RELOCATABLE 2
#define BIT_MASK_DIRECT 3
#define BIT_MASK_RELATIVE 6
#define BIT_ABSOLUTE_FLAG 4

/* Data Limits */
#define MASK_24BIT 0xffffff
#define MASK_21BIT 0x1fffff
#define MAX_24BIT 8388607
#define MIN_24BIT (-8388608)
#define MAX_21BIT 1048575
#define MIN_21BIT (-1048576)
#define BUFFER_SIZE 20
#define MAX_DATA_NUM 38

/* Characters */
#define DOT '.'
#define HASH '#'
#define PLUS '+'
#define MINUS '-'
#define COLON ':'
#define COMMA ','
#define COMMENT ';'
#define AMPERSAND '&'
#define DOUBLE_QUOTE '\"'
#define NULL_TERMINATOR '\0'
#define UNDERSCOR '_'

#endif /* CONST_H */
