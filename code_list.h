#ifndef CODE_LIST_H
#define CODE_LIST_H

/* Data struct definition */
typedef struct Code {
    unsigned int IC;
    unsigned int value;
    struct Code *next;
} Code;


/**
 * @brief Adds a new code to the linked list.
 * This function creates a new Code structure, initializes it with the given values,
 * and adds it to the end of the linked list.
 * @param DC The instruction counter (IC) for the new code.
 * @param value The value associated with the new code.
 * @param head Pointer to the head of the linked list.
 * @return 0 on success, 1 on failure (memory allocation error).
 */
int add_code(unsigned int DC, unsigned int value, Code **head);


/**
 * @brief Frees the memory allocated for the linked list.
 * This function traverses the linked list and frees each Code structure.
 * @param head Pointer to the head of the linked list.
 */
void free_code_list(Code **head);

/**
 * @brief Retrieves the last code in the linked list.
 * This function traverses the linked list to find the last Code structure.
 * @param head Pointer to the head of the linked list.
 * @return Pointer to the last Code structure, or NULL if the list is empty.
 */
Code *get_last_code(Code *head);


/**
 * @brief Prints the contents of the linked list.
 * This function traverses the linked list and prints the IC and value of each Code structure.
 * @param head Pointer to the head of the linked list.
 */
void print_code_list(const Code *head);

#endif
