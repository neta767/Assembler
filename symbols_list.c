/**
 * @file symbols_list.c
 * @brief This file contains the implementation of the functions for managing labels in the assembler.
 * It includes functions to add, check, and free labels.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbols_list.h"

/* Defining the head of the labels linked list */
static Symbol *head = NULL;

Symbol *add_symbol(const char *name, int content, Type type) {
    Symbol *last_symbol;

    Symbol *new_symbol = (Symbol *) malloc(sizeof(Symbol));
    if (new_symbol == NULL) {
        printf("Error: Memory allocation failed");
        return NULL; /* Indicates failure */
    }
    /* Allocating memory and copying the label */
    new_symbol->label = (char *) malloc(strlen(name) + 1); /* +1 to accommodate '\0' */
    if (new_symbol->label == NULL) {
        printf("Error: Memory allocation failed");
        free(new_symbol);
        return NULL; /* Indicates failure */
    }
    strcpy(new_symbol->label, name);

    /* Setting the content, type, location and next pointer */
    new_symbol->type = type;
    new_symbol->address = content;

    new_symbol->next = NULL;

    /* If the list is empty, setting the new label as the head */
    if (head == NULL) {
        head = new_symbol;
    } else {
        /* Otherwise, finding the end of the list and adding the new label */
        last_symbol = get_last_label();
        last_symbol->next = new_symbol;
    }
    return new_symbol; /* Indicates success */
}

Symbol *is_symbol_name(const char *label_name) {
    Symbol *current = head;

    while (current != NULL) {
        if (strcmp(current->label, label_name) == 0 && current->type != OPERAND) {
            return current; /* Indicates label is a label label and returns a pointer to its node */
        }
        current = current->next;
    }
    return NULL; /* Indicates label is not a label label */
}

Symbol *is_label_defined(const char *label_name) {
    Symbol *current = head;

    while (current != NULL) {
        if ((strcmp(current->label, label_name) == 0) && (
                (current->type == CODE) || (current->type == EXTERN && current->address == 0) || (
                    current->type == DATA))) {
            return current; /* Indicates label is a label label and returns a pointer to its node */
        }
        current = current->next;
    }
    return NULL; /* Indicates label is not a label label */
}

void update_data_labels(const int *ICF) {
    Symbol *current = head;

    while (current != NULL) {
        if (current->type == DATA) {
            current->address += *ICF;
        }
        current = current->next;
    }
}

Symbol *get_operand_label() {
    Symbol *current = head;

    while (current != NULL) {
        if (current->type == OPERAND) {
            return current; /* Indicates an "operand" type label was found */
        }
        current = current->next;
    }
    return NULL; /* Indicates no "operand" type label was found */
}

int entry_exist() {
    Symbol *current = head;

    while (current != NULL) {
        if (current->type == ENTRY) {
            return 1; /* Indicates an "entry" type label was found */
        }
        current = current->next;
    }
    return 0; /* Indicates no "entry" type label was found */
}

int extern_exist() {
    Symbol *current = head;

    while (current != NULL) {
        if (current->type == EXTERN) {
            return 1; /* Indicates an "extern" type label was found */
        }
        current = current->next;
    }
    return 0; /* Indicates no "extern" type label was found */
}

Symbol *get_label_head() {
    return head;
}

Symbol *get_last_label() {
    Symbol *current;

    if (head == NULL)
        return NULL; /* Indicates list is empty */

    current = head;
    while (current->next != NULL)
        current = current->next;

    return current; /* Returning the last label in the list */
}

void remove_last_label() {
    Symbol *current;

    if (head->next == NULL) {
        /* Indicates only one label in the list */
        free(head->label);
        free(head);
        head = NULL;
        return;
    }
    current = head;

    while (current->next->next != NULL) {
        /* Skipping to the second-to-last label */
        current = current->next;
    }
    free(current->next->label);
    free(current->next);

    current->next = NULL; /* Updating the second-to-last label to be the new last */
}

void remove_label(const Symbol *label) {
    Symbol *current = head;
    Symbol *prev = NULL;

    while (current != NULL) {
        if (current == label) {
            if (prev == NULL) {
                /* Checking if "head" is the label to be removed */
                head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current->label);
            free(current);
            return; /* Symbol found and removed */
        }
        prev = current;
        current = current->next;
    }
}

void free_labels() {
    Symbol *current = head;
    Symbol *next;

    while (current != NULL) {
        next = current->next; /* Updating the next pointer */

        free(current->label); /* Freeing the dynamically allocated label */
        free(current); /* Freeing the macro node itself */

        current = next; /* Moving to the next node */
    }
    head = NULL;
}
