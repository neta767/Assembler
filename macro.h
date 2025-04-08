#ifndef MACRO_H
#define MACRO_H

#include <stdio.h>

/* Constants */
#define MAX_LINE 100
#define MAX_NAME 31

/* Macro structure definition */
typedef struct Macro {
    char name[MAX_NAME];     /* Macro name */
    char **lines;            /* Array of lines in the macro */
    int num_lines;           /* Number of lines */
    int capacity;            /* Allocated capacity */
    struct Macro *next;      /* Pointer to the next macro */
} Macro;

/* Function declarations */
int is_reserved(const char *name);

int is_valid_macro_name(const char *name);

/* Checks if a line starts with a given keyword */
int starts_with(const char *line, const char *keyword);

/* Finds a macro in the list by name */
Macro* find_macro(const char *name);

/* Adds a macro to the global macro list */
void add_macro(Macro *macro);

/* Adds a line of text to a macro's body */
void add_line_to_macro(Macro *macro, const char *line);

/* Frees all memory used by the macro list */
void free_macros(void);

#endif /* MACRO_H */
