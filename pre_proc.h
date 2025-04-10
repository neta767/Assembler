#ifndef PRE_PROC_H
#define PRE_PROC_H

#include <stdio.h>

/**
 * Preprocesses an assembly source file:
 *  Expands macro calls and creates an .am output file from a .as source
 *
 * @param name - Source file name without extension
 * @return 0 on success, 1 on failure
 */
int pre_proc(char *name);

#endif
