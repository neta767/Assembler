#include "file.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
	/* Validates the provided as a command-line argument. */
	if (argc < 2) {
		printf("No file provided to read\n");
		return 0;
	}

	/* Scanning files */
	for (int i = 1; i < argc; i++) {  /* Refactored: replaced while loop with clearer for loop */
		if (handle_file(argv[i]) != 0) {
			printf("Error processing file: %s\n", argv[i]);
		}
	}

	return 0;
}