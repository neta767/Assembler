#include "assembler.h"

int main(int argc, char *argv[]) {
	/* Validates the provided as a command-line argument. */
	if (argc < 2) {
        printf("No file provided to read\n");
        return 0;
    }

 	/* Scanning files */
    while (--argc > 0) {
	 	hadnle_file(argv[argc]);
    }
    return 0;
}