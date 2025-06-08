#ifndef DATA_LIST_H
#define DATA_LIST_H

/* Data struct definition */
typedef struct Data {
    unsigned int DC;
    unsigned int value;
    struct Data *next;
} Data;

/**
 * Creates a new data node.
 * @param DC The data code.
 * @param value The value of the data.
 * @return Pointer to the new data node.
 */
int add_data(unsigned int DC, unsigned int value, Data **head);


/**
* Frees the data list.
* @param head Pointer to the head of the data list.
*/
void free_data_list(Data **head);


/**
* Gets the last data node in the list.
* @param head Pointer to the head of the data list.
*/
Data *get_last_data(Data *head);


/**
* Prints the data list.
* @param head Pointer to the head of the data list.
*/
void print_data_list(const Data *head);

#endif
