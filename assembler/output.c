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

void create_entry(head_ptr_t headPtr, char* entryFile) {
    FILE* filePointer;
    int tableLength, idx;
    bool isEmpty;

    tableLength = get_table_used(headPtr);
    isEmpty = True;

    FILE_CREATE(filePointer, entryFile)

    for (idx = 0; idx < tableLength; idx++) {
        if (get_symbol_is_entry(headPtr, idx)) {
            isEmpty = False;
            fprintf(filePointer, "%s\t\t%d\n", get_symbol_name(headPtr, idx), get_symbol_value(headPtr, idx));
        }
    }
    fclose(filePointer);
    if (isEmpty)
        remove(entryFile);
}

void create_external(head_ptr_t headPtr, char* externalFile) {
    FILE* filePointer;
    int codeLength, idx;
    bool isEmpty;

    codeLength = get_code_used(headPtr);
    isEmpty = True;

    FILE_CREATE(filePointer, externalFile)

    for (idx = 0; idx < codeLength; idx++) {
        if (get_code_is_extern(headPtr, idx)) {
            isEmpty = False;
            fprintf(filePointer, "%s\t\t%d\n", get_direct_label(headPtr, idx), get_code_line(headPtr, idx));
        }
    }

    fclose(filePointer);
    if (isEmpty)
        remove(externalFile);
}
