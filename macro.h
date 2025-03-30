#include "assembler.h"
/**
* @brief Struct for macro content lines.
 */
typedef struct MACRO {
    char line[MAX_LINE_LEN]; /* Line of macro content */
    struct MACRO * next; /* Pointer to the next line */
} MACRO;

/**
 * @brief Struct for a macro in the macro list.
 */
typedef struct MACRO_LIST {
    char  macro_name [MAX_LINE_LEN]; /* Name of the macro */
    MACRO * head_content; /* Head of macro content */
    struct MACRO_LIST * next; /* Pointer to the next macro */
} MACRO_LIST;