/**
 * @file machine_code.c
 * @brief This file contains functions for generating machine code from assembly instructions.
 * It handles the conversion of data and instruction codes into binary format, as well as
 * managing the memory allocation for the generated code.
 */
#include <stdio.h>
#include <stdlib.h>
#include "machine_code.h"
#include "validations.h"
#include "symbols_list.h"
#include "const.h"


void add_data_code(Data **data_head, int *DC, int number) {
    /* Getting the 24-bit 2's complement binary representation of the number */
    unsigned int word = number & MASK_24BIT;
    /* Adding the code to the data array */
    add_data(*DC, word, data_head);
    (*DC)++; /* Incrementing data count */
}

void add_instruction_code(Code **code_head, int *usage, int *IC, unsigned int word, int *error) {
    /* Checking if memory limit was reached */
    if (*usage == CAPACITY) {
        printf(
            "Error: Memory capacity exceeded! Assembler machine-coding is suspended, however line scanning continues");
        *error = 1;
        (*usage)++; /* Incrementing usage count so the next iteration will not print another error message */
        return; /* Scanning line finished */
    }
    if (*usage > CAPACITY) {
        /* Checking if memory limit was exceeded */
        return; /* Scanning line finished */
    }
    /* Adding the code to the code array */
    add_code(*IC, word, code_head);
    (*IC)++; /* Incrementing data count */
    *usage += 1; /* Incrementing usage count */
}

void process_instruction_code(Code **code_head, int *usage, int *IC, FILE *file, int method, char *operand,
                              int operands_num, int *error) {
    unsigned int word = 0, temp = 0;
    /* Handling the word */
    switch (method) {
        case IMMEDIATE:
            operand++; /* Skipping the 'HASH' sign */
            word |= BIT_ABSOLUTE_FLAG; /* Setting bit 2 for "Absolute" */
            temp |= (unsigned int) (atoi(operand) & MASK_21BIT);
            word |= temp << FUNCS_POS; /* Setting bits 3-23 */
            break; /* Scanning line finished */
        case DIRECT:
            if (add_symbol(operand, *IC, OPERAND) == NULL) {
                /* Indicates memory allocation failed */
                fclose(file);
                free_labels();
                exit(1); /* Exiting program */
            }
            word |= BIT_MASK_DIRECT;
        /* Setting bits 0 and 1 to signal to the "second pass" to update this label address */
            break; /* Scanning line finished */
        case RELATIVE:
            operand++; /* Skipping the 'AMPERSAND' sign */
            if (add_symbol(operand, *IC, OPERAND) == NULL) {
                /* Indicates memory allocation failed */
                fclose(file);
                free_labels();
                exit(1); /* Exiting program */
            }
            word |= BIT_MASK_RELATIVE;
        /* Setting bits 1 and 2 to signal to the "second pass" to update this label address */
            word |= (*IC) << FUNCS_POS;
            break;
        default:
            return;
    }
    add_instruction_code(code_head, usage, IC, word, error); /* Adding machine code (second word) */
}

void handle_one_operand(Code **code_head, int *usage, int *IC, FILE *file, int method, char *operand, int instruct_id,
                        int *error) {
    unsigned int word = 0;
    word |= (INSTRUCTIONS[instruct_id].opcode << OPCODE_POS) | (INSTRUCTIONS[instruct_id].funct << FUNCS_POS) |
            BIT_ABSOLUTE_FLAG;

    word |= (method << (DST_OPERAND_POS));
    /* Handling the word */
    if (method == DIRECT_REGISTER) {
        word |= (get_regis(operand) << DST_REGISTER_POS);
    }
    add_instruction_code(code_head, usage, IC, word, error); /* Adding machine code (first word) */
    /* Handling the second word */
    process_instruction_code(code_head, usage, IC, file, method, operand, INSTRUCTIONS[instruct_id].operands_num,
                             error);
}

void handle_two_operands(Code **code_head, int *usage, int *IC, FILE *file, char *src_operand, char *dest_operand,
                         int instruct_id, int *error, int src_method, int dest_method) {
    unsigned int word = 0;
    word |= (INSTRUCTIONS[instruct_id].opcode << OPCODE_POS) | (INSTRUCTIONS[instruct_id].funct << FUNCS_POS) |
            BIT_ABSOLUTE_FLAG;
    word |= (src_method << SRC_OPERAND_POS);
    word |= (dest_method << DST_OPERAND_POS);
    /* Handling the word */
    if (src_method == DIRECT_REGISTER)
        word |= get_regis(src_operand) << SRC_REGISTER_POS;
    /* Handling the word */
    if (dest_method == DIRECT_REGISTER)
        word |= get_regis(dest_operand) << DST_REGISTER_POS;

    add_instruction_code(code_head, usage, IC, word, error); /* Adding machine code (first word) */
    /* Handling the second word */
    process_instruction_code(code_head, usage, IC, file, src_method, src_operand,
                             INSTRUCTIONS[instruct_id].operands_num, error);
    /* Handling the third word */
    process_instruction_code(code_head, usage, IC, file, dest_method, dest_operand,
                             INSTRUCTIONS[instruct_id].operands_num - 1,
                             error); /* operands_num-1 to signal that operand is of type "destination" */
}
