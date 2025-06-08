/**
 * @file const.c
 * @brief This file contains the definitions of constants used in the assembler.
 *
 * The constants include instruction definitions, register names, directive prompts,
 * and type names. These are used throughout the assembler for various purposes.
 */
#include "const.h"

/* Instruction Definitions */
const instruction INSTRUCTIONS[] = {
    {"mov", 0, 0, 2, METHODS_0_1_3, METHODS_1_3},
    {"cmp", 1, 0, 2, METHODS_0_1_3, METHODS_0_1_3},
    {"add", 2, 1, 2, METHODS_0_1_3, METHODS_1_3},
    {"sub", 2, 2, 2, METHODS_0_1_3, METHODS_1_3},
    {"lea", 4, 0, 2, METHOD_1, METHODS_1_3},
    {"clr", 5, 1, 1, NONE, METHODS_1_3},
    {"not", 5, 2, 1, NONE, METHODS_1_3},
    {"inc", 5, 3, 1, NONE, METHODS_1_3},
    {"dec", 5, 4, 1, NONE, METHODS_1_3},
    {"jmp", 9, 1, 1, NONE, METHODS_1_2},
    {"bne", 9, 2, 1, NONE, METHODS_1_2},
    {"jsr", 9, 3, 1, NONE, METHODS_1_2},
    {"red", 12, 0, 1, NONE, METHODS_1_3},
    {"prn", 13, 0, 1, NONE, METHODS_0_1_3},
    {"rts", 14, 0, NO_OPERANDS, NONE},
    {"stop", 15, 0, NO_OPERANDS, NONE}
};

/* Register Names */
const char *REGISTERS[] = {
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
};

/* Directive Prompts */
const char *PROMPTS[] = {
    ".data", ".string", ".entry", ".extern"
};

/* Type Names */
const char *TYPES[] = {
    "label", "macro", "entry", "extern", "instruction"
};
