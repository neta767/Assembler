/*
* Handles macro declarations, content management, and memory cleanup
 */

#ifndef MACROS_H
#define MACROS_H

/* Struct for a macro */
typedef struct Macro {
 char *name;             /* Macro name */
 char *content;          /* Macro body text */
 struct Macro *next;     /* Pointer to next macro in list */
} Macro;

/**
 * Adds a new macro to the list.
 * @param name - Name of the macro.
 * @return 0 on success, 1 on memory allocation failure.
 */
int add_macro(char *name);

/*
 * Checks if a name matches a defined macro.
 * macro_name - Name to search for.
 * Returns pointer to the macro if found, or NULL otherwise.
 */
Macro *is_macro_name(char *macro_name);

/*
 * Appends text to the last defined macro's content.
 * new_content - String to append.
 * Returns 0 on success, 1 on memory allocation failure.
 */
int append_macro_content(char *new_content);

/*
 * Removes the last macro from the list and frees its memory.
 */
void remove_last_macro(void);

/*
 * Frees all allocated macros and resets the macro list.
 */
void free_macros(void);

#endif