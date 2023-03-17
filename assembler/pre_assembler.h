#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H


#include "constants.h"

#define MAX_MACRO_LENGTH 20
#define MAX_MACRO_INST 100 /* Maximum length of content assumed. */

typedef struct {
    char* content;
    char* name;
} macro;

typedef struct {
    macro* array;
    int used;
    int size;
} macroArray;

enum { MACRO_START,
       MACRO_CONTENT,
       MACRO_END,
       MACRO_NONE };

/* Receives a file with ".as" extension and creates a new one
 *  with ".am" extension, with the macros replaced.*/
int macro_remove(char*, char*);

/* Initializes the macro array. */
void init_macro_array(macroArray*, int);

/* Handles insertion into macro array.*/
void insert_macro_array(macroArray*, char*, char*);

/* Frees all memory allocations in macro array. */
void free_macro_array(macroArray*);

/* This function receives the string, a flag and the command length, and returns the current "macro status". */
int current_macro_status(char*, bool, int);

#endif
