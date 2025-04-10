#include <stdio.h>

#include "pre_proc.h"
#include "util.h"

/**
 * This program receives one or more file names as command-line arguments.
 * Each file is expected to contain assembly code.
 * The program performs preprocessing on each file (e.g., macro expansion).
 *
 * Assumptions:
 * -
 */

/* Entry point for the assembler */
int main(int argc, char *argv[]){
	int i;

	/* Process each file provided as argument */
	for (i = 1; i < argc; i++) {
		printf("Processing file: %s\n", argv[i]);

		/* Preprocess the file */
		if (pre_proc(argv[i])) {
			printf("Preprocessing failed for file: %s\n", argv[i]);
			continue;
		}

		/* Free any memory allocated globally (e.g., macros) */
		free_all_memory();

		/* TODO: Add first-pass and second-pass assembler logic here */
	}

	return 0;
  }

