#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "macro.h"

Macro *macro_list = NULL;

const char *reserved_words[] = { "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red",
    "prn", "jsr", "rts", "stop", "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", ".data", ".string", ".entry", ".extern" };

int is_reserved(const char *name) {
    int i;
    for (i = 0; i < sizeof(reserved_words) / sizeof(reserved_words[0]); i++) {
        if (strcmp(name, reserved_words[i]) == 0)
            return 1;
    }
    return 0;
}

Macro* find_macro(const char *name) {
    Macro *curr = macro_list;
    while (curr) {
        if (strcmp(curr->name, name) == 0)
            return curr;
        curr = curr->next;
    }
    return NULL;
}

void add_macro(Macro *macro) {
    macro->next = macro_list;
    macro_list = macro;
}

int is_valid_macro_name(const char *name) {
    int i;
    if (!isalpha(name[0]) && name[0] != '_')
        return 0;
    for (i = 1; name[i] != '\0'; i++) {
        if (!isalnum(name[i]) && name[i] != '_')
            return 0;
    }
    return strlen(name) <= MAX_NAME - 1;
}

void free_macros() {
    int i;
    Macro *curr = macro_list;
    Macro *temp;
    while (curr) {
        for (i = 0; i < curr->num_lines; i++) {
            free(curr->lines[i]);
        }
        free(curr->lines);
        temp = curr;
        curr = curr->next;
        free(temp);
    }
    macro_list = NULL;
}

void add_line_to_macro(Macro *macro, const char *line) {
    if (macro->num_lines >= macro->capacity) {
        macro->capacity *= 2;
        macro->lines = realloc(macro->lines, macro->capacity * sizeof(char *));
    }
    macro->lines[macro->num_lines] = malloc(strlen(line) + 1);
    strcpy(macro->lines[macro->num_lines], line);
    macro->num_lines++;
}
