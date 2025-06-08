/**
* @file main.c
 * @brief This file contains the main function for the assembler program.
 * @details The assembler program processes assembly language files, performing
 *          pre-processing, first pass and second pass instructions.
 */
#include <stdio.h>
#include "util.h"
#include "pre_proc.h"
#include "first_pass.h"
#include "const.h"
#include "data_list.h"
#include "code_list.h"
#include "second_pass.h"
#include "symbols_list.h"

/**
 * @brief The main function of the assembler program.
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments.
 * @return 0 on success, 1 on failure.
 */
int main(int argc, char *argv[]) {
    int i = 1;
    int IC = IC_INITIAL; /* Instruction Counter */
    int DC = DC_INITIAL; /* Data Counter */
    Data *data_head = NULL; /* Defining the head of the data linked list */
    Code *code_head = NULL; /* Defining the head of the data linked list */
    /* Checking if the user entered at least one file label */
    if (argc < TWO) {
        printf("Error: No files entered\n");
        return 1;
    }
    /* Looping through all the command-line arguments */
    for (; i < argc; i++) {
        printf("\nProcessing file: \"%s\"\n", argv[i]);
        if (pre_proc(argv[i]) != 0) {
            printf("Process terminated\n");
            continue;
        }
        printf("Pre-Process was successful\n");
        if (first_pass(argv[i], &data_head, &code_head, &IC, &DC) != 0) {
            printf("Process terminated\n");
            continue;
        }
        printf("First pass pass was successful\n");
        if (second_pass(argv[i], data_head, code_head, &IC, &DC) != 0) {
            printf("Process terminated\n");
            continue;
        }
        printf("Second pass was successful\n");
        printf("Process ended\n");
        free_code_list(&code_head);
        free_data_list(&data_head);
        free_labels();
    }
    return 0;
}
