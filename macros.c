/* Handles macros: creation, content management, and cleanup */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macros.h"

/* Head of macro linked list (static to this file) */
static Macro *head = NULL;

/* Returns the last macro in the list, or NULL if list is empty */
static Macro *get_last_macro() {
    Macro *curr = head;
    if (!curr) return NULL;
    while (curr->next)
        curr = curr->next;
    return curr;
}

/* Adds a new macro to the list */
int add_macro(char *name) {
    Macro *new_macro = malloc(sizeof(Macro));
    if (!new_macro) {
        printf("Error: malloc failed\n");
        return 1;
    }

    new_macro->name = malloc(strlen(name) + 1);
    if (!new_macro->name) {
        free(new_macro);
        printf("Error: malloc failed\n");
        return 1;
    }
    strcpy(new_macro->name, name);

    new_macro->content = NULL;
    new_macro->next = NULL;

    if (!head)
        head = new_macro;
    else
        get_last_macro()->next = new_macro;

    return 0;
}

/* Appends text to the content of the last macro */
int append_macro_content(char *text) {
    Macro *curr = get_last_macro();
    int old_len = curr->content ? strlen(curr->content) : 0;
    int add_len = strlen(text);
    char *new_mem = realloc(curr->content, old_len + add_len + 1);
    if (!new_mem) {
        printf("Error: realloc failed\n");
        return 1;
    }
    curr->content = new_mem;

    if (old_len == 0)
        curr->content[0] = '\0';

    strcat(curr->content, text);
    return 0;
}

/* Checks if name is a defined macro; returns pointer or NULL */
Macro *is_macro_name(char *name) {
    Macro *curr = head;
    while (curr) {
        if (strcmp(curr->name, name) == 0)
            return curr;
        curr = curr->next;
    }
    return NULL;
}

/* Removes the last macro from the list */
void remove_last_macro(void) {
    Macro *curr = head;

    if (!curr) return;

    if (!curr->next) {
        free(curr->name);
        free(curr->content);
        free(curr);
        head = NULL;
        return;
    }

    while (curr->next->next)
        curr = curr->next;

    free(curr->next->name);
    free(curr->next->content);
    free(curr->next);
    curr->next = NULL;
}

/* Frees the entire macro list */
void free_macros(void) {
    Macro *curr = head, *next;
    while (curr) {
        next = curr->next;
        free(curr->name);
        free(curr->content);
        free(curr);
        curr = next;
    }
    head = NULL;
}

