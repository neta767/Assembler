/**
 * @file macro_list.c
 * @brief This file contains the implementation of functions to manage a linked list of macros.
 *
 * The functions include adding a macro, checking if a name is a macro, appending content to a macro,
 * getting the last macro in the list, and freeing the list.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro_list.h"
#include "const.h"


int add_macro(char *name, Macro **head) {
    Macro *last_macro;

    Macro *new_macro = malloc(sizeof(Macro));
    if (new_macro == NULL) {
        printf("Error: Memory allocation failed");
        return 1;
    }

    new_macro->name = malloc(strlen(name) + 1);
    if (new_macro->name == NULL) {
        printf("Error: Memory allocation failed");
        free(new_macro);
        return 1;
    }
    strcpy(new_macro->name, name);
    new_macro->content = NULL;
    new_macro->next = NULL;

    if (*head == NULL) {
        *head = new_macro;
    } else {
        last_macro = get_last_macro(*head);
        last_macro->next = new_macro;
    }

    return 0;
}


Macro *is_macro_name(char *macro_name, Macro *head) {
    Macro *current = head;
    while (current != NULL) {
        if (strcmp(current->name, macro_name) == 0) {
            return current; /* Indicates name is a macro name and returns a pointer to its node */
        }
        current = current->next;
    }
    return NULL; /* Indicates name is not a macro name */
}

int append_macro_content(char *new_content, Macro *head) {
    Macro *current;
    size_t current_length, new_content_length, total_length;
    char *new_memory;

    current = get_last_macro(head); /* Skipping to the last macro in the list */

    /* Current cannot be NULL because append_macro_content is called only if a macro node was created - the list is not empty */
    if (current != NULL && current->content != NULL) {
        current_length = strlen(current->content);
    } else {
        current_length = 0;
    }

    new_content_length = strlen(new_content);
    total_length = current_length + new_content_length + 1; /* +1 to accommodate '\0' */
    if (new_content == NULL) return 1;

    new_memory = realloc(current->content, total_length); /* Reallocating or allocating memory for the new content */
    if (new_memory == NULL) {
        printf("Error: Memory allocation failed");
        return 1; /* Indicates failure */
    }

    current->content = new_memory;

    if (current_length == 0) {
        /* If the current content is NULL, initializing it as an empty string */
        current->content[0] = NULL_TERMINATOR;
    }

    strcat(current->content, new_content); /* Appending the new content to the existing content */

    return 0; /* Indicates success */
}

Macro *get_last_macro(Macro *head) {
    Macro *current;

    if (head == NULL)
        return NULL; /* Indicates list is empty */

    current = head;
    while (current->next != NULL)
        current = current->next;

    return current; /* Returning the last macro in the list */
}

void free_macros(Macro **head) {
    Macro *current = *head;
    Macro *next;

    while (current != NULL) {
        next = current->next; /* Updating the next pointer */

        free(current->name); /* Freeing the dynamically allocated name */
        free(current->content); /* Freeing the dynamically allocated content */
        free(current); /* Freeing the macro node itself */

        current = next; /* Moving to the next node */
    }
    *head = NULL;
}
