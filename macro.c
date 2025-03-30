#define MAX_LINE_LEN 82
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//MACRO_LIST *macroListHead = NULL;  // Head of the macro list

/**
 * @brief Adds a macro to the macro list.
 */
void add_macro(char *name) {
    MACRO_LIST *newMacro = (MACRO_LIST *)malloc(sizeof(MACRO_LIST));
    if (!newMacro) {
        printf("Memory allocation failed for macro!\n");
        exit(1);
    }
    strcpy(newMacro->macro_name, name);
    newMacro->head_content = NULL;
    newMacro->next = macroListHead;
    macroListHead = newMacro;
}

/**
 * @brief Adds a line of content to the last stored macro.
 */
void add_macro_content(char *line) {
    if (!macroListHead) return;  // No macro exists yet

    MACRO *newContent = (MACRO *)malloc(sizeof(MACRO));
    if (!newContent) {
        printf("Memory allocation failed for macro content!\n");
        exit(1);
    }
    strcpy(newContent->line, line);
    newContent->next = NULL;

    if (!macroListHead->head_content) {
        macroListHead->head_content = newContent;
    } else {
        MACRO *temp = macroListHead->head_content;
        while (temp->next) temp = temp->next;
        temp->next = newContent;
    }
}

/**
 * @brief Frees all allocated memory for macros.
 */
void free_macros() {
    MACRO_LIST *currentMacro = macroListHead;
    while (currentMacro) {
        MACRO *currentContent = currentMacro->head_content;
        while (currentContent) {
            MACRO *tempContent = currentContent;
            currentContent = currentContent->next;
            free(tempContent);
        }
        MACRO_LIST *tempMacro = currentMacro;
        currentMacro = currentMacro->next;
        free(tempMacro);
    }
}