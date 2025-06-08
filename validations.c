/**
 * @file validations.c
 * @brief This file contains functions to analyze and validate assembly code.
 * It checks for valid names, instructions, registers, addressing methods,
 * and handles data and string declarations.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "validations.h"
#include "code_list.h"
#include "data_list.h"
#include "util.h"
#include "symbols_list.h"
#include "machine_code.h"
#include "const.h"

/* Function to check if a name is valid */
int valid_name(char *name, int line_num, char *file_name, Type type) {
    int i = 0;
    Symbol *symbol;
    /* Checking if the name is empty */
    if (*name == NULL_TERMINATOR) {
        print_error_type("Invalid declaration, no name was defined", file_name, line_num, TYPES[type]);
        return 0; /* Indicates name is not valid */
    }
    /* Checking if the first character is an alphabetic */
    if (!isalpha(name[0])) {
        if (get_prompt(name) != -1) {
            /* Checking if the  name is an instruction in case of a non-alphabetic first character */
            print_error_type("Invalid declaration, an instruction name cannot be", file_name, line_num, TYPES[type]);
            return 0; /* Indicates  name is not valid */
        }
        print_error_type("Invalid declaration", file_name, line_num, TYPES[type]);
        return 0; /* Indicates  name is not valid */
    }
    /* Checking if the  name length is valid */
    if (strlen(name) > MAX_DECLARATION_LENGTH) {
        print_error_type("Invalid declaration, name is too long, 31 characters max", file_name, line_num, TYPES[type]);
        return 0; /* Indicates  name is not valid */
    }
    /* Checking if the name only contains alphabetic or numeric characters */
    for (i = 0; name[i] != NULL_TERMINATOR; i++) {
        if (!isalnum(name[i]) && (type != MACRO && name[i] == UNDERSCOR)) {
            print_error_type("Invalid declaration, name must contain alphabetic or numeric characters only", file_name,
                             line_num, TYPES[type]);
            return 0; /* Indicates  name is not valid */
        }
    }
    /* Checking if the  name is a reserved word */
    if (is_reserved_word(file_name, name, line_num, type)) {
        return 0; /* Indicates  name is not valid */
    }
    if (type == LABEL) {
        symbol = is_symbol_name(name);
        if (symbol != NULL) {
            if (symbol->type == EXTERN) {
                print_error_type(
                    "Invalid label declaration, local label name cannot be the same as an external label name",
                    file_name, line_num, TYPES[type]);
                return 0; /* Indicates name label is not valid */
            }
            if (symbol->type == LABEL) {
                print_error("Invalid label declaration, this label name is already in use", file_name, line_num);
                return 0; /* Indicates name label is not valid */
            }
        }
    }
    return 1; /* Indicates  name is valid */
}

/* Function to check if a string is a valid instruction */
int get_instruct_id(const char *str) {
    int i = 0;

    if (str == NULL) /* Indicates string is not instruction */
        return -1;

    /* Comparing the string with each of the system instruction */
    for (; i < INSTRUCTIONS_COUNT; i++) {
        if (strcmp(str, INSTRUCTIONS[i].instruction) == 0) {
            return i; /* Returning the index of the matching instruction */
        }
    }
    return -1; /* Indicates string is not an instruction */
}

/* Function to check if a string is a valid register */
int get_regis(const char *str) {
    int i = 0;

    if (str == NULL) /* Indicates string is not a register */
        return -1;

    /* Comparing the string with each of the system registers */
    for (; i < REGISTERS_COUNT; i++) {
        if (strcmp(str, REGISTERS[i]) == 0) {
            return i; /* Returning the index of the matching register */
        }
    }
    return -1; /* Indicates string is not a register */
}

/* Function to check if a string is a valid prompt */
int get_prompt(const char *str) {
    int i = 0;

    if (str == NULL) /* Indicates string is not a prompt */
        return -1;

    /* Comparing the string with each of the system prompts */
    for (i = 0; i < PROMPTS_COUNT; i++) {
        if (strcmp(str, PROMPTS[i]) == 0) {
            return i; /* Returning the index of the matching prompt */
        }
    }
    return -1; /* Indicates string is not a prompt */
}

/* Function to check if a string is a valid addressing method */
int get_addressing_method(char *operand, char *file_name, int line_num) {
    char *endline;
    long val;

    if (operand[0] == HASH) {
        operand++;
        /* Checking if no value was entered */
        if (*operand == NULL_TERMINATOR) {
            print_error("This instruction has an operand that uses an 'IMMEDIATE' method type but has no value",
                        file_name, line_num);
            return -1; /* Indicates failure */
        }
        val = strtol(operand, &endline,DECIMAL_BASE);
        /* Checking if the conversion was successful */
        if (*endline != NULL_TERMINATOR || endline == operand) {
            print_error("This operand is invalid for an 'IMMEDIATE' method type, only integers allowed", file_name,
                        line_num);
            return -1; /* Indicates failure */
        }
        /* Checking if the number is in range */
        if (val < MIN_21BIT || val > MAX_21BIT) {
            print_error("This operand is out of range for an 'IMMEDIATE' method type", file_name, line_num);
            return -1; /* Indicates failure */
        }
        return IMMEDIATE; /* Returning the matching addressing method */
    }
    if (operand[0] == AMPERSAND) {
        operand++;
        /* Checking if no value was entered */
        if (*operand == NULL_TERMINATOR) {
            print_error("This instruction has an operand that uses an 'RELATIVE' method type but has no value",
                        file_name, line_num);
            return -1; /* Indicates failure */
        }
        if (!valid_name(operand, line_num, file_name, OPERAND)) {
            print_error("This operand is not a valid label for an 'RELATIVE' method type", file_name, line_num);
            return -1; /* Indicates failure */
        }
        return RELATIVE; /* Returning the matching addressing method */
    }
    if (get_regis(operand) != -1) /* Indicates operand is a register */
        return DIRECT_REGISTER; /* Returning the matching addressing method */

    /* If this line was reached then the operand is a potential label - validating */
    if (!valid_name(operand, line_num, file_name, OPERAND)) {
        return -1; /* Indicates failure */
    }
    /* If the name is valid - returning the matching addressing method */
    return DIRECT;
}

/* Function to check if a string is a reserved word */
int is_reserved_word(char *file_name, const char *str, int line_num, Type type) {
    /* Comparing the string with each of the system's reserved words and determining the error message */
    if (get_instruct_id(str) != -1 || get_regis(str) != -1 || get_prompt(str) != -1 || strcmp(str,MACRO_START) == 0 ||
        strcmp(str,MACRO_END) == 0) {
        /* Function returns the index of the matching word or -1 if no word matched */
        print_error_type("Invalid declaration, reserved words cannot be used as a name", file_name, line_num,
                         TYPES[type]);
        return 1; /* Indicates the mame is invalid */
    }
    return 0; /* Indicates the name is valid */
}

/* Function to check if a line is a "data prompt" line */
int is_data_prompt(Data **data_head, int *usage, int *DC, int line_num, char *file_name, FILE *file, char *line,
                   char *current_word, int *error, char *label) {
    Symbol *symbol;
    if (strcmp(label, "") != 0) {
        symbol = add_symbol(label, *DC, DATA);
        if (!symbol) {
            /* Indicates memory allocation failed */
            free(current_word);
            fclose(file);
            free(file_name);
            free_labels();
            exit(1); /* Exiting program */
        }
    }
    if (is_data(data_head, usage, DC, line_num, file_name, file, line, error, current_word) ||
        is_string(data_head, usage, DC, line_num, file_name, line, error, current_word)) {
        return 1;
    }
    if (strcmp(label, "") != 0)
        remove_last_label();
    return 0; /* Indicates line is not a "data prompt" line, continue scanning */
}

/* Function to check if a line is an "instruction" line */
int is_instruction(Code **code_head, int *usage, int *IC, char *line, int line_num, FILE *file, char *file_name,
                   char *current_word, int *error, char *label) {
    Symbol *symbol;
    size_t curr_word_len = strlen(current_word);
    /* Checking for a potential instruction */
    int instruct_id = get_instruct_id(current_word);
    if (instruct_id != -1) {
        /* Indicates line is an "instruction" line */
        /* Updating label properties */
        line += curr_word_len; /* Skipping the first word */
        if (strcmp(label, "") != 0) {
            symbol = add_symbol(label, *IC, CODE);
            if (symbol == NULL) {
                /* Indicates memory allocation failed */
                free(current_word);
                fclose(file);
                free(file_name);
                free_labels();
                exit(1); /* Exiting program */
            }
        } /* Validating instruction */
        /* Validating instruction */
        if (valid_instruction(code_head, usage, IC, instruct_id, error, file_name, line_num, file, line)) {
            return 1; /* Scanning line finished */
        }
        *error = 1;
        if (strcmp(label, "") != 0)
            remove_last_label();
    }
    return 0; /* Indicates line is not an "instruction" line, continue scanning */
}

/* Function to check if an instruction is valid */
int is_method_legal(char *file_name, int line_num, int method, int instruct_id, int operands_num) {
    /* Getting the legal methods for the current instruction */
    if (operands_num == 1) {
        /* Indicates operand is of type "destination" */
        switch (INSTRUCTIONS[instruct_id].destination_methods) {
            case METHODS_1_2:
                if (method != DIRECT && method != RELATIVE) {
                    print_error("This instruction uses an illegal method for a destination operand", file_name,
                                line_num);
                    return 0; /* Indicates method is illegal */
                }
                return 1; /* Indicates method is legal */
            case METHODS_1_3:
                if (method != DIRECT && method != DIRECT_REGISTER) {
                    print_error("This instruction uses an illegal method for a destination operand", file_name,
                                line_num);
                    return 0; /* Indicates method is illegal */
                }
                return 1;
            case METHODS_0_1_3:
                if (method == RELATIVE) {
                    print_error("This instruction uses an illegal method for a destination operand", file_name,
                                line_num);
                    return 0; /* Indicates method is illegal */
                }
                return 1;
            default: /* Indicates method is illegal */
                print_error("This instruction uses an illegal method for a destination operand", file_name, line_num);
                return 0;
        }
    }
    /* If this line was reached then operand is of type "source" */
    switch (INSTRUCTIONS[instruct_id].source_methods) {
        case METHOD_1:
            if (method != DIRECT) {
                print_error("This instruction uses an illegal method for a source operand", file_name, line_num);
                return 0; /* Indicates method is illegal */
            }
            return 1;
        case METHODS_0_1_3:
            if (method == RELATIVE) {
                print_error("This instruction uses an illegal method for a destination operand", file_name, line_num);
                return 0; /* Indicates method is illegal */
            }
            return 1;
        default: /* Indicates method is illegal */
            print_error("This instruction uses an illegal method for a source operand", file_name, line_num);
            return 0;
    }
}

/* Function to check if a line is a "data" line */
int is_data(Data **data_head, int *usage, int *DC, int line_num, char *file_name, FILE *file, char *line, int *error,
            const char *current_word) {
    if (get_prompt(current_word) != 0)
        return 0; /* Indicates line is not a "data prompt" line, continue scanning */
    /* Checking if there are no parameters */
    if (*line == NULL_TERMINATOR) {
        print_error("Instruction \".data\" has no parameters", file_name, line_num);
        *error = 1;
        return 0;
    }
    /* Analyzing input numbers */
    return analyze_numbers(data_head, usage, DC, line_num, file_name, file, line, error);
}

/* Function to check if a line is a "string" line */
int is_string(Data **data_head, int *usage, int *DC, int line_num, char *file_name, char *line, int *error,
              const char *current_word) {
    int i = 0;
    size_t line_len;

    if (get_prompt(current_word) != 1) {
        return 0; /* Indicates line is not a "string prompt" line, continue scanning */
    }
    /* Checking for a parameter */
    if (*line == NULL_TERMINATOR) {
        print_error("Instruction \".string\" has no parameter", file_name, line_num);
        *error = 1;
        return 0;
    }
    trim_whitespace(line);
    line_len = strlen(line);

    /* Checking for double quotes at the start and the end of the string */
    if (line[0] != DOUBLE_QUOTE || line[line_len - 1] != DOUBLE_QUOTE) {
        print_error("Instruction \".string\" expects one string parameter enclosed in double quotes (\" \")", file_name,
                    line_num);
        *error = 1;
        return 0;
    }
    /* Checking if the string is empty */
    if (strlen(line) == TWO) {
        /* Indicates string contains only double quotes */
        printf(" WARNING in \"%s\" line %d: Instruction \".string\" parameter"
               " is an empty string\n", file_name, line_num);
    }
    line[line_len - 1] = NULL_TERMINATOR;
    line++;
    line_len = strlen(line);

    /* Adding machine code to data array */
    for (; i < line_len; i++) {
        if (*usage + 1 == CAPACITY) {
            /* Checking if memory limit was reached (+1 to account for the null-terminator) */
            printf(
                "Error: Memory capacity exceeded! Assembler machine-coding is suspended, however line scanning continues");
            *error = 1;
            (*usage)++; /* Incrementing usage count so the next iteration will not print another error message */
            return 0; /* Scanning line finished */
        }
        if (*usage + 1 > CAPACITY) {
            /* Checking if memory limit was exceeded */
            return 0; /* Scanning line finished */
        }
        /* Getting the ASCII value by converting 'char' type to 'int' and then adding code */
        add_data_code(data_head, DC, (int) line[i]);
        *usage += 1; /* Incrementing usage count */
    }
    add_data_code(data_head, DC, 0); /* Adding the null-terminator */
    *usage += 1; /* Incrementing usage count */
    return 1;
}

/* Function to check if a line is an "extern" line */
int is_extern(int line_num, char *file_name, FILE *file, char *line, int *error, char *current_word) {
    Symbol *symbol;
    Symbol *temp_symbol;

    if (get_prompt(current_word) != 3) {
        return 0; /* Indicates line is not "extern prompt" line, continue scanning */
    }

    /* Checking if there is no label declaration */
    if (*line == NULL_TERMINATOR) {
        print_error("Instructions \".extern\" must have a label declaration", file_name, line_num);
        *error = 1;
        return 0; /* Scanning line finished */
    }
    /* Checking if there is more than one label declaration */
    trim_whitespace(line);
    if (contains_whitespace(line)) {
        print_error("Instructions \".extern\" allow one label declaration at a time", file_name, line_num);
        *error = 1;
        return 0; /* Scanning line finished */
    }
    if (!valid_name(line, line_num, file_name, EXTERN)) {
        *error = 1;
        return 0; /* Scanning line finished */
    }

    temp_symbol = is_symbol_name(line);
    if (temp_symbol) {
        if (temp_symbol->type != EXTERN) {
            print_error("Instruction \".extern\" label cannot be the same as a local label", file_name, line_num);
            *error = 1;
            return 0; /* Indicates label label is not valid */
        }
        printf("WARNING in \"%s\" line %d: Instructions \".extern\" duplicate declarations will be ignored\n",
               file_name, line_num);
        return 1;
    }

    symbol = add_symbol(line, 0, EXTERN);
    if (symbol == NULL) {
        /* Indicates memory allocation failed */
        free(current_word);
        fclose(file);
        free(file_name);
        free_labels();
        exit(1); /* Exiting program */
    }
    return 1;
}

/* Function to check if an instruction is valid */
int valid_instruction(Code **code_head, int *usage, int *IC, int instruct_id, int *error, char *file_name, int line_num,
                      FILE *file, char *line) {
    char *src_operand, *dest_operand, *comma_pos;
    int operands_num = INSTRUCTIONS[instruct_id].operands_num, src_method, dest_method;
    size_t length;
    unsigned int word = 0;
    word |= (INSTRUCTIONS[instruct_id].opcode << OPCODE_POS) | (INSTRUCTIONS[instruct_id].funct << FUNCS_POS) |
            BIT_ABSOLUTE_FLAG;

    /* Analyzing operands */
    switch (operands_num) {
        case 0:
            if (line[0] != NULL_TERMINATOR) {
                /* Checking if there is extraneous text */
                print_error("This instruction has extraneous text, no operands required", file_name, line_num);
                return 0; /* Scanning line finished */
            }
            add_instruction_code(code_head, usage, IC, word, error); /* Adding machine code */
            return 1; /* Scanning line finished */
        case 1:
            if (line[0] == NULL_TERMINATOR) {
                /* Checking if there is a missing operand */
                print_error("This instruction has a missing operand", file_name, line_num);
                *error = 1;
                return 0; /* Scanning line finished */
            }
            while (*line && isspace(*line)) /* Skipping leading whitespace */
                line++;

            if (line[0] == COMMA) {
                /* Checking if there is an illegal comma */
                print_error("This instruction has an illegal comma after the instruction label", file_name, line_num);
                *error = 1;
                return 0; /* Scanning line finished */
            }
            if (contains_whitespace(line) || strchr(line,COMMA) != NULL) {
                /* Checking for extraneous text */
                print_error("This instruction has extraneous text, only one operand is required", file_name, line_num);
                *error = 1;
                return 0; /* Scanning line finished */
            }
        /* Checking if the addressing method is legal */
            src_method = get_addressing_method(line, file_name, line_num);
            if (src_method == -1) {
                *error = 1;
                return 0; /* Scanning line finished */
            }
            if (!is_method_legal(file_name, line_num, src_method, instruct_id, operands_num)) {
                /* Checking if the addressing method is legal */
                *error = 1;
                return 0; /* Scanning line finished */
            }
            handle_one_operand(code_head, usage, IC, file, src_method, line, instruct_id, error);
            return 1; /* Scanning line finished */
        case 2:
            if (line[0] == NULL_TERMINATOR) {
                /* Checking if there are missing operands */
                print_error("This instruction has missing operands", file_name, line_num);
                *error = 1;
                return 0; /* Scanning line finished */
            }
            while (*line && isspace(*line)) /* Skipping leading whitespace */
                line++;

            if (line[0] == COMMA) {
                /* Checking if there is an illegal comma */
                print_error("This instruction has an illegal comma after the instruction label", file_name, line_num);
                *error = 1;
                return 0; /* Scanning line finished */
            }
            src_operand = get_first_word(line);
            if (src_operand == NULL) {
                /* Indicates memory allocation failed (all the other allocations were freed inside function) */
                fclose(file);
                free(file_name);
                free_labels();
                exit(1); /* Exiting program */
            }
            length = strlen(src_operand);

            comma_pos = strchr(src_operand,COMMA);
            if (comma_pos != NULL) {
                /* If a comma was found, splitting the string into two operands */
                if (comma_pos - src_operand == strlen(src_operand) - 1 && line[length] == NULL_TERMINATOR) {
                    print_error("This instruction has a missing operand", file_name, line_num);
                    *error = 1;
                    free(src_operand);
                    return 0; /* Scanning line finished */
                }
                *comma_pos = NULL_TERMINATOR;
                length = comma_pos - src_operand;
                dest_operand = &line[length + 1]; /* Setting a pointer to the second operand */

                while (*dest_operand && isspace(*dest_operand)) /* Skipping leading whitespace */
                    dest_operand++;

                if (dest_operand[0] == COMMA) {
                    /* Checking if there are consecutive commas */
                    print_error("This instruction has multiple consecutive commas", file_name, line_num);
                    *error = 1;
                    free(src_operand);
                    return 0; /* Scanning line finished */
                }
                if (contains_whitespace(dest_operand) || strchr(dest_operand,COMMA) != NULL) {
                    /* Checking for extraneous text */
                    print_error("This instruction has extraneous text, only two operands are required", file_name,
                                line_num);
                    *error = 1;
                    free(src_operand);
                    return 0; /* Scanning line finished */
                }
            } else {
                /* No comma was found in the first string */
                line = &line[length];
                while (*line && isspace(*line)) /* Skipping leading whitespace */
                    line++;
                if (line[0] == NULL_TERMINATOR || (*line == COMMA && line[1] == NULL_TERMINATOR)) {
                    /* Checking if there is a missing operand */
                    print_error("This instruction has a missing operand", file_name, line_num);
                    *error = 1;
                    free(src_operand);
                    return 0; /* Scanning line finished */
                }
                if (line[0] != COMMA) {
                    print_error("This instruction has a missing comma", file_name, line_num);
                    *error = 1;
                    free(src_operand);
                    return 0; /* Scanning line finished */
                }
                line++;
                while (*line && isspace(*line)) /* Skipping leading whitespace */
                    line++;
                dest_operand = get_first_word(line);
                if (dest_operand == NULL) {
                    /* Indicates memory allocation failed (all the other allocations were freed inside function) */
                    fclose(file);
                    free(file_name);
                    free_labels();
                    free(src_operand);
                    exit(1); /* Exiting program */
                }
                if (dest_operand[0] == COMMA) {
                    print_error("This instruction has multiple consecutive commas", file_name, line_num);
                    *error = 1;
                    free(src_operand);
                    free(dest_operand);
                    return 0; /* Scanning line finished */
                }
                length = strlen(dest_operand);
                if (strchr(dest_operand,COMMA) != NULL || line[length] != NULL_TERMINATOR) {
                    /* Checking for extraneous text */
                    print_error("This instruction has extraneous text, only two operands are required", file_name,
                                line_num);
                    *error = 1;
                    free(src_operand);
                    free(dest_operand);
                    return 0; /* Scanning line finished */
                }
            }
            src_method = get_addressing_method(src_operand, file_name, line_num);
            dest_method = get_addressing_method(dest_operand, file_name, line_num);
            if (src_method == -1 || dest_method == -1) {
                free(src_operand);
                *error = 1;
                return 0; /* Scanning line finished */
            }
            if (!is_method_legal(line, line_num, src_method, instruct_id, operands_num) ||
                /* Checking if the addressing method is legal */
                !is_method_legal(line, line_num, dest_method, instruct_id, operands_num - 1)) {
                /* operands_num-1 to signal that operand is of type "destination" */
                free(src_operand);
                *error = 1;
                return 0; /* Scanning line finished */
            }
            handle_two_operands(code_head, usage, IC, file, src_operand, dest_operand, instruct_id, error, src_method,
                                dest_method);
            free(src_operand);
            return 1;
        default: /* Indicates method is illegal */
            print_error("This instruction has an illegal number of operands", file_name, line_num);
            *error = 1;
            return 0; /* Scanning line finished */
    }
}

/* Function to check if a line is a "data" line */
int analyze_numbers(Data **data_head, int *usage, int *DC, int line_num, char *file_name, FILE *file, char *line,
                    int *error) {
    int num_count = 0, i = 0;
    int *num_array = get_numbers(file_name, line_num, file, line, &num_count);
    if (num_array == NULL) {
        *error = 1;
        return 0;
    }

    /* Adding machine code to data array */
    for (; i < num_count; i++) {
        if (*usage == CAPACITY) {
            /* Checking if memory limit was reached */
            printf(
                "Error: Memory capacity exceeded! Assembler machine-coding is suspended, however line scanning continues");
            *error = 1;
            (*usage)++; /* Incrementing usage count so the next iteration will not print another error message */
            free(num_array);
            return 0; /* Scanning line finished */
        }
        if (*usage > CAPACITY) {
            /* Checking if memory limit was exceeded */
            free(num_array);
            return 0; /* Scanning line finished */
        }
        add_data_code(data_head, DC, num_array[i]); /* Adding machine code */
        (*usage)++; /* Incrementing usage count */
    }
    free(num_array);
    return 1;
}
