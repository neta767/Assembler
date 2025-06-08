/**
 * @file data_list.c
 * @brief Implementation of a linked list to store data.
 * @details This file contains the implementation of functions to manage a linked list of data.
 *          It includes functions to add data, get the last data, free the list, and print the list.
 */
#include <stdio.h>
#include <stdlib.h>
#include "data_list.h"


int add_data(unsigned int DC, unsigned int value, Data **head) {
    Data *last_Data;

    Data *new_Data = malloc(sizeof(Data));
    if (new_Data == NULL) {
        printf("Error: Memory allocation failed\n");
        return 1;
    }

    new_Data->DC = DC;
    new_Data->value =value;
    new_Data->next = NULL;

    if (*head == NULL) {
        *head = new_Data;
    } else {
        last_Data = get_last_data(*head);
        last_Data->next = new_Data;
    }

    return 0;
}


Data *get_last_data(Data *head) {
    if (head == NULL)
        return NULL;

    while (head->next != NULL)
        head = head->next;

    return head;
}

void free_data_list(Data **head) {
    Data *current = *head;
    Data *temp;

    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }

    *head = NULL;
}

void print_data_list(const Data *head) {
    while (head != NULL) {
        printf("DC: %u VALUE: %u\n", head->DC,head->value);
        head = head->next;
    }
}
