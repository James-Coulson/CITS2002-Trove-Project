// -------------------- INCLUDES -------------------- //

#include "trove.h"

// --------------------  MAIN METHOD -------------------- //

int main(int argc, char *argv[]) {
	// Defining c
	int c;
	// Assigning argc to max_arg
	filelist_length = argc;
	filelist = argv;

	// Checking options
	while ((c = getopt(argc, argv, OPT_STRING)) != -1) {
		switch (c) {
			case 'b':
				bflag = true;
				if (optind > filelist_index) { filelist_index = optind; }
				break;

			case 'f':
				fflag = true;
				fvalue = optarg;
				if (optind > filelist_index) { filelist_index = optind; }
				break;
			
			case 'l':
				lflag = true;
				lvalue = atoi(optarg);
				if (optind > filelist_index) { filelist_index = optind; }
				break;

			case 'r':
				rflag = true;
				if (optind > filelist_index) { filelist_index = optind; }
				break;
			
			case 'u':
				uflag = true;
				if (optind > filelist_index) { filelist_index = optind; }
				break;
		}
	}

	// Checking fflag is raised
	if (!fflag) {
		printf("No file was given in the arg (use -f)\n");
		return 1;
	}

	// Checking if modifying trovefile
	if (bflag || rflag || uflag) {
		// Calling modify_trovefile and returning exit status
		return modify_trovefile();
	} else {
		// Calling search_trovefile and returning exit status
		return search_trovefile();
	}

	// If we are here something has gone wrong
	return 1;
}
