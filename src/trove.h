// -------------------- INCLUDES -------------------- //
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

// -------------------- MACROS -------------------- //

// Defining macro to check if variable is null
//  - If value is null it returns an error
#define CHECK_NULL(val) if (val == NULL) return 1;

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

// -------------------- GLOBALS -------------------- //

// Flags
extern bool bflag;
extern bool fflag;
extern bool lflag;
extern bool rflag;
extern bool uflag;

// Arg values
extern char *fvalue;
extern int lvalue;

// Args
extern int filelist_index;
extern int filelist_length;

// Argv
extern char **filelist;

// -------------------- UTILITY -------------------- //

// Defining save_trovefile function
extern int save_trovefile(TROVEFILE *trovefile);

// Defining populate_trovefile function
extern int populate_trovefile(TROVEFILE *trovefile, FILE *fp);

// -------------------- SEARCH FUNCTION -------------------- //

// Defining search_trovefile function
extern int search_trovefile();

// -------------------- MODIFY FUNCTION -------------------- //

// Defining modify_trovefile function
extern int modify_trovefile();
