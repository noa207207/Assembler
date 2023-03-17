#ifndef OUTPUT_H
#define OUTPUT_H
#include <stdio.h>

#include "constants.h"
#include "data_structure.h"

#define FORMAT_LEN 5

#define FILE_CREATE(filePointer, name)            \
    if (!(filePointer = fopen(name, "w"))) {      \
        printf("Cannot create file %s.\n", name); \
        return;                                   \
    }

/* Creates 3 output files: Object File, Entry File, External File. */
void create_object(head_ptr_t headPtr, char*);

/* Creates an object file. */
void create_output(head_ptr_t headPtr, char* filename);

/* If labels declared as ".entry" exist, creates an entry file. */
void create_entry(head_ptr_t headPtr, char*);

/* If labels declared as ".extern" exist, creates an extern file. */
void create_external(head_ptr_t headPtr, char*);
#endif
