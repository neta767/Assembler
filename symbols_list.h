#ifndef SYMBOLS_LIST_H
#define SYMBOLS_LIST_H

typedef enum Type {
    LABEL,
    MACRO,
    ENTRY,
    EXTERN,
    OPERAND,
    CODE,
    DATA
} Type;


/* Symbol struct definition */
typedef struct Symbol {
    char *label;
    int address;
    Type type;
    struct Symbol *next;
} Symbol;

/**
 * Adds a new Symbol to the end of the linked list.
 * @param name The name of the new label.
 * @param address The address of the new label in memory.
 * @param type The type of the new label.
 * @return 0 for a successful instruction ,1 if errors were detected.
 */
Symbol *add_symbol(const char *name, int address, Type type);


/**
 * Checks if a given label is a label label.
 * @param label_name The name to check.
 * @return Pointer to the label if found, NULL otherwise.
 */
Symbol *is_symbol_name(const char *label_name);


/**
 * Checks if a given label label is defined.
 * @param label_name The name to check.
 * @return Pointer to the label if found, NULL otherwise.
 */
Symbol *is_label_defined(const char *label_name);


/**
 * Updates the addresses of data labels.
 * @param ICF Pointer to the instruction counter.
 */
void update_data_labels(const int *ICF);


/**
 * Retrieves the first "operand" label.
 * @return Pointer to the operand label if found, NULL otherwise.
 */
Symbol *get_operand_label();


/**
 * Checks if any "entry" type labels exist.
 * @return 1 if an entry label exists, 0 otherwise.
 */
int entry_exist();


/**
 * Checks if any "extern" type labels exist.
 * @return 1 if an extern label exists, 0 otherwise.
 */
int extern_exist();


/**
 * Gets the head of the label list.
 * @return Pointer to the head of the label list.
 */
Symbol *get_label_head();


/**
 * Gets the last label in the linked list.
 * @return Pointer to the last label in the list.
 */
Symbol *get_last_label();


/**
 * Removes the last label in the linked list.
 */
void remove_last_label();


/**
 * Removes a specific label from the linked list.
 * @param label Pointer to the label to be removed.
 */
void remove_label(const Symbol *label);


/**
 * Frees all the labels in the linked list.
 */
void free_labels();


#endif

