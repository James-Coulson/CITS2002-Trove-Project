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

// -------------------- POPULATING TROVEFILE METHOD -------------------- //

int populate_trovefile(TROVEFILE *trovefile, FILE *fp) {
	// Define buffer
	char buffer[MAX_BUF];

	// Iterate through lines
	while(fgets(buffer, MAX_BUF, fp) != NULL) {
		// Get entry
		TROVEFILE_ENTRY *entry = &trovefile->entries[trovefile->num_entries];

		// Assign filepath to entry
		buffer[strcspn(buffer, "\n")] = '\0';
		entry->file_path = strdup(buffer);

		// Get files
		if (fgets(buffer, MAX_BUF, fp) == NULL) { return 1; }

		// Defining strtok token
		char *token = strtok(buffer, " ");

		// Iterate through tokens
		while (token != NULL) {
			// Removing newline
			token[strcspn(token, "\n")] = '\0';

			// Saving word to trovefile entry
			entry->words[entry->num_words] = malloc((strlen(token)) * sizeof(char));
			strcpy(entry->words[entry->num_words++], token);

			// Get next token
			token = strtok(NULL, " ");
		}

		// Incrementing entries
		trovefile->num_entries++;

		// // Print
		// for (int i = 0; i < entry->num_words; i++) {
		// 	printf("%s | ", entry->words[i]);
		// }
		// printf("\n");
	}

	return 0;
}

// -------------------- MODIFY TROVEFILE METHODS -------------------- //

int modify_trovefile() {
	// TODO: Add check for minimum length of word

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

	// If an entry in a trovefile needs to be deleted
	if (rflag) {
		// Attempting to open trovefile
		FILE *fp = fopen(fvalue, "r");

		// Checking file was found
		CHECK_NULL(fp);

		// Defining TROVEFILE
		TROVEFILE *trovefile = calloc(1, sizeof(TROVEFILE));

		// Populating trovefile
		populate_trovefile(trovefile, fp);

		// printf("%i\n", trovefile->num_entries);

		// Closing file
		fclose(fp);

		// Iterate through files to be removed
		for (int i = filelist_index; i < filelist_length; i++) {
			// Getting file and converting to absolute path
			char *file = realpath(filelist[i], NULL);

			// Iterate through trovefile entries
			for (int j = 0; j < trovefile->num_entries; j++) {
				// Get entry
				TROVEFILE_ENTRY *entry = &trovefile->entries[j];
				// Check if files match

				// printf("\"%s\" : \"%s\"", file, entry->file_path);
				if (strcmp(file, entry->file_path) == 0) {
					// Remove entry
					// TODO: The previous entry may need to be freed for space efficency
					if (j != trovefile->num_entries - 1) { trovefile->entries[j] = trovefile->entries[--trovefile->num_entries]; }
					else { trovefile->num_entries--; }
					break;
				}
			}
		}

		// // // Print
		// for (int i = 0; i < trovefile->entries[0].num_words; i++) {
		// 	printf("%s | ", trovefile->entries[0].words[i]);
		// }
		// printf("\n");

		// Saving trovefile
		return save_trovefile(trovefile);

	}
	
	if (uflag) {
		// Attempting to open trovefile
		FILE *fp = fopen(fvalue, "r");

		// Checking file was opened
		CHECK_NULL(fp);

		// Defining trovefile
		TROVEFILE *trovefile = calloc(1, sizeof(TROVEFILE));

		// Populating trovefile
		populate_trovefile(trovefile, fp);

		// Closing file
		fclose(fp);

		// Iterate through files, removing filese that have been given to update.
		// TODO: The rest of it
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
