#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

// Defining options used in command line invocation
#define OPT_STRING "bf:l:ru"

int main(int argc, char *argv[]) {
	// Defining flags
	bool bflag = false;
	bool fflag = true;
	bool lflag = false;
	bool rflag = false;
	bool uflag = false;

	// Defining arg values
	char *fvalue = NULL;
	int lvalue = -1;
	
	// Defining c
	int c;

	// Checking options
	while ((c = getopt(argc, argv, OPT_STRING)) != -1) {
		switch (c) {
			case 'b':
				bflag = true;
				break;

			case 'f':
				fflag = true;
				fvalue = optarg;
				break;
			
			case 'l':
				lflag = true;
				lvalue = atoi(optarg);
				break;

			case 'r':
				rflag = true;
				break;
			
			case 'u':
				uflag = true;
				break;
		}
	}

	// Checking fflag is raised
	if (!fflag) {
		printf("No file was given in the arg (use -f)\n");
		return 0;
	}

	// Checking if modifying trovefile
	if (bflag || rflag || uflag) {
		printf("hello\n");
		return 0;
	}

	return 0;
}
