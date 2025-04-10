/* Utility functions: memory, string handling, error printing */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "util.h"
#include "macros.h"
#include "definitions.h"

/* Defining the registers */
char *REGISTERS[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/* Defining the instructions */
char *INSTRUCTIONS[] = {".data", ".string", ".entry", ".extern"};



/* Defining the opcodes */
Op_Code OPCODES[] = {
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
    {"rts", 14, 0, NONE, NONE},
    {"stop", 15, 0, NONE, NONE}
};

/* Head of the linked list tracking all dynamically allocated blocks */
static TrackedBlock *tracked_memory_head = NULL;

/* Allocates memory and tracks it for future cleanup */
void *allocate_memory(long size) {
    void *ptr = malloc(size);
    TrackedBlock *node;

    if (!ptr) {
        printf("Error: malloc failed\n");
        free_macros();
        free_all_memory();
        return NULL;
    }

    node = malloc(sizeof(TrackedBlock));
    if (!node) {
        printf("Error: malloc failed\n");
        free(ptr);
        free_macros();
        free_all_memory();
        return NULL;
    }

    node->ptr = ptr;
    node->next = tracked_memory_head;
    tracked_memory_head = node;

    return ptr;
}

/* Frees a specific pointer and removes it from tracking */
void free_tracked_block(void *target_ptr) {
    TrackedBlock *curr = tracked_memory_head, *prev = NULL;

    while (curr) {
        if (curr->ptr == target_ptr) {
            if (prev)
                prev->next = curr->next;
            else
                tracked_memory_head = curr->next;
            free(curr->ptr);
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

/* Frees all allocated memory from tracking list */
void free_all_memory() {
    TrackedBlock *curr = tracked_memory_head, *temp;
    while (curr) {
        temp = curr;
        curr = curr->next;
        free(temp->ptr);
        free(temp);
    }
    tracked_memory_head = NULL;
}

/* Removes leading and trailing whitespace */
char *trim_whitespace(char *str) {
    char *end;
    while (*str && isspace(*str)) str++;
    if (*str == '\0') return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    *(end + 1) = '\0';

    return str;
}

/* Checks if a string contains whitespace */
int contains_whitespace(char *str) {
    while (*str) {
        if (isspace(*str))
            return 1;
        str++;
    }
    return 0;
}

/* Prints error with filename and line number */
void print_error(char *msg, char *file, int line) {
    printf("Error in %s line %d: %s\n", file, line, msg);
}

/* Checks if word appears alone in string */
int is_standalone_word(char *str, char *word) {
    int len = strlen(word);
    char *pos = strstr(str, word);

    while (pos) {
        if ((pos == str || isspace(pos[-1])) &&
            (isspace(pos[len]) || pos[len] == '\0'))
            return 1;
        pos = strstr(pos + len, word);
    }
    return 0;
}

/* Reserved word validation */

/* Checks if string is a valid opcode */
int which_opcode(char *str) {
    int i;
    if (!str) return -1;

    for (i = 0; i < OPCODES_COUNT; i++) {
        if (strcmp(str, OPCODES[i].operation) == 0)
            return i;
    }
    return -1;
}

/* Checks if string is a register name */
int which_regis(char *str) {
    int i;
    if (!str) return -1;

    for (i = 0; i < REGISTERS_COUNT; i++) {
        if (strcmp(str, REGISTERS[i]) == 0)
            return i;
    }
    return -1;
}

/* Checks if string is an instruction name */
int which_instr(char *str) {
    int i;
    if (!str) return -1;

    for (i = 0; i < INSTRUCTIONS_COUNT; i++) {
        if (strcmp(str, INSTRUCTIONS[i]) == 0)
            return i;
    }
    return -1;
}

/* Checks if word is a reserved keyword (for macro/label/operand) */
int is_reserved_word(char *file, char *str, int line, Type type) {
    int ext = strlen(file) >= EXTENSION_LEN && strcmp(file + strlen(file) - EXTENSION_LEN, ".as") == 0;

    if (which_opcode(str) != -1) {
        if (ext && type != OPERAND)
            print_error("Opcode name not allowed", file, line);
        else
            print_error("Reserved word", file, line);
        return 1;
    }

    if (which_regis(str) != -1) {
        if (ext && type != OPERAND)
            print_error("Register name not allowed", file, line);
        else
            print_error("Reserved word", file, line);
        return 1;
    }

    if (which_instr(str) != -1) {
        if (ext && type != OPERAND)
            print_error("Instruction name not allowed", file, line);
        else
            print_error("Reserved word", file, line);
        return 1;
    }

    if (!strcmp(str, "macr") || !strcmp(str, "endmacr")) {
        print_error("Reserved word", file, line);
        return 1;
    }

    return 0;
}


int is_valid_macro_name(const char *name) {
    int i;
    if (!isalpha(name[0]) && name[0] != '_')
        return 0;
    for (i = 1; name[i] != '\0'; i++) {
        if (!isalnum(name[i]) && name[i] != '_')
            return 0;
    }
    return strlen(name) <= MAX_MACRO_LENGTH - 1;
}

/* Validates macro name syntax and reserved usage */
int valid_macro_name(char *file, char *name, int line) {
    if (contains_whitespace(name)) {
        print_error("Macro name must be one word", file, line);
        return 1;
    }

    if (strlen(name) > MAX_MACRO_LENGTH) {
        print_error("Macro name too long", file, line);
        return 1;
    }

    if(!is_valid_macro_name(name)) {
        print_error("Invalid macro name", file, line);
        return 1;
    }

    if (is_reserved_word(file, name, line, REGULAR))
        return 1;

    return 0;
}

