// -------------------- INCLUDES -------------------- //

#include "trove.h"

// -------------------- GLOBAL VARIABLES -------------------- //

// Defining flags
bool bflag = false;
bool fflag = false;
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
