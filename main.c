// -------------------- INCLUDES -------------------- //
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

// -------------------- CONSTANTS -------------------- //

// Defining options used in command line invocation
#define OPT_STRING "bf:l:ru"

// Defining maximum number of words per indexed file
#define MAX_WORDS_PER_FILE 100

// Defining maximum number of entries per trove file
#define MAX_ENTRIES 100

// Defining maximum buffer size
#define MAX_BUF 300

// Defining "whitespace" characters
#define WORD_WHITESPACE " ~,-"

// -------------------- STRUCTS -------------------- //

// Struct to store information about a trovefuile entry
typedef struct {
	char *file_path;
	int num_words;
	char *words[MAX_WORDS_PER_FILE];
} TROVEFILE_ENTRY;

// Struct to store information about a trovefile
typedef struct {
	char *file;
	int num_entries;
	TROVEFILE_ENTRY entries[MAX_ENTRIES];
} TROVEFILE;

// -------------------- MACROS -------------------- //

// Defining macro to check if variable is null
//  - If value is null it returns an error
#define CHECK_NULL(val) if (val == NULL) return 1;

// -------------------- GLOBAL VARIABLES -------------------- //

// Defining flags
bool bflag = false;
bool fflag = true;
bool lflag = false;
bool rflag = false;
bool uflag = false;

// Defining arg values
char *fvalue = NULL;
int lvalue = 4;

// Beginning of file list in argv
int filelist_index = -1;

// Length of argv
int filelist_length = -1;

// argv
char **filelist = NULL;

// -------------------- SAVE TROVEFILE METHOD -------------------- //

int save_trovefile(TROVEFILE *trovefile) {
	// Create file pointer
	FILE *fp = fopen(fvalue, "wb");

	// Check if null
	CHECK_NULL(fp);

	

	// Iterate through trovefile entries
	for (int i = 0; i < trovefile->num_entries; i++) {
		// Getting pointer to trovefile entry
		TROVEFILE_ENTRY *entry = &trovefile->entries[i];

		// Write filepath to trovefile
		fprintf(fp, "%s\n", entry->file_path);

		// Iterate through words
		for (int j = 0; j < entry->num_words; j++) {
			// Printing word to line
			fprintf(fp, "%s ", entry->words[j]);
		}

		// Writing new line
		fprintf(fp, "\n");
	}

	// Close file
	fclose(fp);

	// Return success
	return 0;
}

// -------------------- MODIFY TROVEFILE METHODS -------------------- //

int modify_trovefile() {
	// If a new trovefile needs to be built
	if (bflag) {
		// Defining new TROVEFILE
		TROVEFILE *trovefile = calloc(1, sizeof(TROVEFILE));

		// Assigning file name
		trovefile->file = strdup(fvalue);

		// Assigning num_entires
		trovefile->num_entries = 0;

		// Defining file pointer
		FILE *fp;

		// Iterating through filelist
		for (int i = filelist_index; i < filelist_length; i++) {
			// Attempting to read file
			fp = fopen(filelist[i], "r");

			// Check fp is not NULL
			CHECK_NULL(fp);

			// Defining buffer
			char buffer[MAX_BUF];

			// Increment num_entries
			trovefile->num_entries++;

			// Getting trovefile entry
			TROVEFILE_ENTRY *entry = &trovefile->entries[trovefile->num_entries - 1];

			// Adding absolute path to entry
			entry->file_path = realpath(filelist[i], NULL);

			// Max words reached
			bool max_words_reached = false;

			// Read line by line
			while((fgets(buffer, MAX_BUF, fp)) && !max_words_reached) {
				// Defining strtok token
				char *token = strtok(buffer, WORD_WHITESPACE);

				// Iterate through tokens
				while (token != NULL) {
					// Removing newline
					token[strcspn(token, "\n")] = '\0';

					// Saving word to trovefile entry
					entry->words[entry->num_words] = malloc((strlen(token)) * sizeof(char));
					strcpy(entry->words[entry->num_words++], token);

					// If max tokens exceeded break
					if (entry->num_words >= MAX_ENTRIES) {
						max_words_reached = true;
						break;
					}

					// Get next token
					token = strtok(NULL, WORD_WHITESPACE);
				}
			}

			// This prints out each word in an entry
			// for (int j = 0; j < MAX_ENTRIES; j++) {
			// 	printf("%s\n", trovefile->entries[trovefile->num_entries - 1].words[j]);
			// }
		}

		// Save trovefile
		return save_trovefile(trovefile);
	}

	return 0;
}

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
		return 0;
	}

	// Checking if modifying trovefile
	if (bflag || rflag || uflag) {
		// Calling modify_trovefile and returning exist status
		return modify_trovefile();
	}

	return 0;
}
