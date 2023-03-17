#ifndef UTILS_H
#define UTILS_H
#include "constants.h"

/* Joins two strings together. This allocates memory, 
 * and needs to be freed after use. */
char* str_with_ext(char*, char*);

/* Malloc with monitor - if memory allocation is failed, exits the program. */
void* malloc_with_monitor(long);

/* Realloc with monitor - if memory reallocation is failed, exits the program. */
void* realloc_with_monitor(void*, long);

/* Returns pointer right after spaces and tabs. */
char* skip_spaces(char*);

/* Counts length of word (until space-character or NULL character is reached). */
int word_length(char*);

/* Counts length of line (until \n or NULL character is reached). */
int count_line_length(char*);

/* Returns true if the string received is empty. */
bool empty_string(char*);

/* Returns a pointer right after the current word. */
char* skip_word(char*);

/* Deletes the spaces, tabs and newline in a given string. */
void delete_spaces(char*);

/* Remove spaces and tabs in a line. */
void removeSpacesAndTabs(char *str);

/* Remove spaces and tabs in the end of the line. */
void remove_end_spaces(char *str);

/* Remove new line char from a line. */
void delete_new_line(char* p);

#endif
