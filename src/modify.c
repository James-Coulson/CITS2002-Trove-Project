// -------------------- INCLUDES -------------------- //

#include "trove.h"

// -------------------- MODIFY TROVEFILE METHODS -------------------- //

int modify_trovefile() {
	// TODO: Add check for minimum length of word

	// If a new trovefile needs to be built
	if (bflag) {
		// Defining new TROVEFILE
		TROVEFILE *trovefile = calloc(1, sizeof(TROVEFILE));

		// Check null
		CHECK_NULL(trovefile);

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

		// Close file
		fclose(fp);

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
				if (strcmp(file, entry->file_path) != 0) { continue; }

				// Removing file and breaking
				if (j != trovefile->num_entries - 1) { trovefile->entries[j] = trovefile->entries[--trovefile->num_entries]; }
				else { trovefile->num_entries--; }
				break;
			}
		}

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
		for (int i = filelist_index; i < filelist_length; i++) {
			// Get file
			char *file = realpath(filelist[i], NULL);

			// Iterate through entries in trovefile for matches
			for (int j = 0; j < trovefile->num_entries; j++) {
				// Get entry
				TROVEFILE_ENTRY *entry = &trovefile->entries[j];

				// Check if files match
				if (strcmp(file, entry->file_path) != 0) { continue; }

				// Remvoing entry if files match and breaking
				if (j != trovefile->num_entries - 1) { trovefile->entries[j] = trovefile->entries[--trovefile->num_entries]; }
				else { trovefile->num_entries--; }
				break;
			}
		}

		// --- Update new files --- //

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

			// Resetting num_words
			entry->num_words = 0;

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
		}

		// Close file
		fclose(fp);

		return save_trovefile(trovefile);
	}

	return 0;
}
