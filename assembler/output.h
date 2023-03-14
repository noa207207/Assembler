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

void create_object(head_ptr_t headPtr, char*);
void create_output(head_ptr_t headPtr, char* filename);
void create_entry(head_ptr_t headPtr, char*);
void create_external(head_ptr_t headPtr, char*);
#endif
