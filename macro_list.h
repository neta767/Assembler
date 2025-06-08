#ifndef MACROS_LIST_H
#define MACROS_LIST_H

/* Macro struct definition */
typedef struct Macro {
    char *name;
    char *content;
    struct Macro *next;
} Macro;

/**
 * Adds a new macro to the end of the linked list.
 * @param name The name of the new macro.
 * @param head Pointer to the head of the macro list.
 * @return 0 for a successful addition, 1 if errors were detected.
 */
int add_macro(char *name, Macro **head);


/**
 * Checks if the given label is a macro label.
 * @param macro_name The name to check.
 * @param head Pointer to the head of the macro list.
 * @return Pointer to the macro if found, NULL otherwise.
 */
Macro *is_macro_name(char *macro_name, Macro *head);


/**
 * Appends new content to the last macro in the linked list.
 * @param new_content The content to append.
 * @param head Pointer to the head of the macro list.
 * @return 0 for a successful operation, 1 if errors were detected.
 */
int append_macro_content(char *new_content, Macro *head);


/**
 * Gets the last macro in the linked list.
 * @param head Pointer to the head of the macro list.
 * @return Pointer to the last macro.
 */
Macro *get_last_macro(Macro *head);


/**
 * Frees all macros in the linked list and releases their memory.
 * @param head Pointer to the head of the macro list.
 */
void free_macros(Macro **head);


#endif
