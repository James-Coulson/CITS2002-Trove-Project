// -------------------- INCLUDES -------------------- //

#include "trove.h"

// -------------------- SEARCH TROVEFILE METHDO -------------------- //

int search_trovefile() {
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

	// Get word
	char *word = filelist[filelist_index];

	// Defining found boolean
	bool found = false;

	// Iterate through entries
	for (int i = 0; i < trovefile->num_entries; i++) {
		// Getting entry
		TROVEFILE_ENTRY *entry = &trovefile->entries[i];

		// Iterating through words
		for (int j = 0; j < entry->num_words; j++) {
			// Check if words are same
			if (strcmp(word, entry->words[j]) != 0) { continue; }

			// Print to output
			printf("Found matching word in: %s\n", entry->file_path);

			// Setting found to true
			found = true;
		}
	}

	// Returning
	if (found) { return 0; }
	else {return false; }
}
