#include "output.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_structure.h"
#include "errors.h"
#include "first_pass.h"
#include "parse.h"
#include "utils.h"

/* Creates 3 output files: Object File, Entry File, External File. */
void create_output(head_ptr_t headPtr, char* filename) {
    char *objectFile, *entryFile, *externalFile;

    objectFile = str_with_ext(filename, ".ob");
    entryFile = str_with_ext(filename, ".ent");
    externalFile = str_with_ext(filename, ".ext");

    create_object(headPtr, objectFile);

    create_entry(headPtr, entryFile);

    create_external(headPtr, externalFile);

    free(objectFile);
    free(entryFile);
    free(externalFile);
}

/* Creates an object file. */
void create_object(head_ptr_t headPtr, char* objectFile) {
    FILE* filePointer;
    int codeLines, dataLines;
    char *code_binary, *data_binary;

    FILE_CREATE(filePointer, objectFile);

    codeLines = get_code_used(headPtr);
    dataLines = get_data_used(headPtr);

    fprintf(filePointer, "\t%d %d\n", codeLines, dataLines);

    instructions_to_binary(get_code_image(headPtr), get_code_used(headPtr), &code_binary);

    fprintf(filePointer, "%s", code_binary);

    data_to_binary(get_data_image(headPtr), get_data_used(headPtr), &data_binary);
    fprintf(filePointer, "%s", data_binary);

    fclose(filePointer);
    free(code_binary);
}

/* If labels declared as ".entry" exist, creates an entry file. */
void create_entry(head_ptr_t headPtr, char* entryFile) {
    FILE* filePointer;
    int tableLength, idx;
    bool isEmpty;

    tableLength = get_table_used(headPtr);
    isEmpty = True;

    FILE_CREATE(filePointer, entryFile)

    for (idx = 0; idx < tableLength; idx++) {
        if (get_symbol_isEntry(headPtr, idx)) {
            isEmpty = False;
            fprintf(filePointer, "%s, %04d\n", get_symbol_name(headPtr, idx), get_symbol_value(headPtr, idx));
        }
    }
    fclose(filePointer);
    if (isEmpty)
        remove(entryFile);
}

/* If labels declared as ".extern" exist, creates an extern file. */
void create_external(head_ptr_t headPtr, char* externalFile) {
    FILE* filePointer;
    int codeLength, idx;
    bool isEmpty;

    codeLength = get_code_used(headPtr);
    isEmpty = True;

    FILE_CREATE(filePointer, externalFile)

    for (idx = 0; idx < codeLength; idx++) {
        if (get_code_isExtern(headPtr, idx)) {
            isEmpty = False;
            fprintf(filePointer, "%s %04d\n", get_code_label(headPtr, idx), get_code_line(headPtr, idx));
        }
    }

    fclose(filePointer);
    if (isEmpty)
        remove(externalFile);
}
