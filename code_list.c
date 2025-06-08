/**
 * @file code_list.c
 * @brief Implementation of functions for managing a linked list of Code structures.
 *
 * This file contains functions to add, retrieve, free, and print elements in a linked list.
 * Each element in the list represents a code with an instruction counter (IC) and a value.
 */
#include <stdio.h>
#include <stdlib.h>
#include "code_list.h"

int add_code(unsigned int DC, unsigned int value, Code **head) {
    Code *last_code;

    Code *new_Code = malloc(sizeof(Code));
    if (new_Code == NULL) {
        printf("Error: Memory allocation failed\n");
        return 1;
    }

    new_Code->IC = DC;
    new_Code->value = value;
    new_Code->next = NULL;

    if (*head == NULL) {
        *head = new_Code;
    } else {
        last_code = get_last_code(*head);
        last_code->next = new_Code;
    }

    return 0;
}

Code *get_last_code(Code *head) {
    if (head == NULL)
        return NULL;

    while (head->next != NULL)
        head = head->next;

    return head;
}

void free_code_list(Code **head) {
    Code *current = *head;
    Code *temp;

    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }

    *head = NULL;
}

void print_code_list(const Code *head) {
    while (head != NULL) {
        printf("IC: %u VALUE: %u\n", head->IC, head->value);
        head = head->next;
    }
}
