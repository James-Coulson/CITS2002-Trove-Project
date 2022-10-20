// -------------------- INCLUDES -------------------- //

#include "trove.h"

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
	}

	return 0;
}
