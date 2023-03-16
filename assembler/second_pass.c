#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data_structure.h"
#include "errors.h"
#include "first_pass.h"
#include "parse.h"
#include "utils.h"
#include "second_pass.h"

/* The function is in charge of the second pass. It returns False if no errors were found, and updates the headPointer accordingly. */
bool process_second_pass(head_ptr_t headPtr, char* filename) {
    char line[MAX_LINE_LENGTH], original_line[MAX_LINE_LENGTH];
    char wordPointer_cpy[MAX_LABEL_LENGTH];
    int line_num, shift;
    char* wordPointer;
    FILE* filePointer;
    bool errorsFound;
    opcode op;

    line_num = 0;
    errorsFound = False;
    filePointer = fopen(filename, "r");

    while (fgets(line, MAX_LINE_LENGTH, filePointer)) {
        if (line[strlen(line) - 1] != '\n' && line[strlen(line)] != EOF) {
            int c;
            while ((c = fgetc(filePointer)) != '\n' && c != EOF) {}
        
            if (c == EOF) {
                /* End-of-file reached while discarding characters */ 
                break;
            }
            line_num++;
            continue;
        }
        errorsFound = False;
        line_num++;
        if (empty_string(line) || is_comment(line))
            continue;

        strcpy(original_line, line);

        wordPointer = skip_spaces(line);

        if ((shift = label_check(wordPointer, NULL)) != -1) {
            wordPointer += shift;
            wordPointer = skip_spaces(wordPointer);
        }

        op = firstWord(wordPointer);
        if (op != ENTRY)
            continue;

        wordPointer = skip_word(wordPointer);
        wordPointer = skip_spaces(wordPointer);
        strcpy(wordPointer_cpy, wordPointer);
        delete_new_line(wordPointer_cpy);
        delete_spaces(wordPointer);

        errorsFound = (err_label(headPtr, original_line, strlen(wordPointer_cpy), wordPointer_cpy, line_num, True));

        if (!errorsFound)
            errorsFound = (insert_entry(headPtr, wordPointer, original_line, line_num)) ? errorsFound : True;
    }
    fclose(filePointer);

    errorsFound = (update_code_symbols(headPtr)) ? True : errorsFound;

    return errorsFound;
}

/* The function searches the symbol table for the label, and adds the entry attribute to it. Returns False if not found in table or if duplicate external label is found. */
bool insert_entry(head_ptr_t headPtr, char* label, char* line, int lineNumber) {
    int idx, arrLength;
    bool insertedFlag;

    insertedFlag = False;
    arrLength = get_table_used(headPtr);

    for (idx = 0; idx < arrLength; idx++) {
        if (!strcmp(get_symbol_name(headPtr,idx), label)) {
            if (get_symbol_isExternal(headPtr,idx)) {
                printf("Error on line %d: %sLabel declared as ENTRY and EXTERN simultaneously.\n", lineNumber, line);
                return False;
            }
            set_symbol_isEntry(headPtr, idx, True);

            insertedFlag = True;
        }
    }
    if (insertedFlag)
        return True;

    printf("Error on line %d: %sLabel stated after .entry does not exist.\n", lineNumber, line);
    return False;
}

/* Updates the code image with BASE/HIST. It returns True if errors were found, and False upon success. */
bool update_code_symbols(head_ptr_t headPtr) {
    int i, length, value;
    bool errors, is_extern;
    char *label;
    errors = False;
    length = get_code_used(headPtr);

    for (i = 0; i < length; i++) {
        label = get_direct_label(headPtr, i);
        if (get_code_toDecode(headPtr, i)) {
                value = find_symbol_value(headPtr, label);
                if (value != -1) {
                    set_direct_value(headPtr, i, value);
                }
                else
                    errors = True; 
                is_extern = find_symbol_is_extern(headPtr, label);
                if(is_extern){
                    set_direct_era(headPtr, i, E);
                    set_code_isExtern(headPtr, i, True);
                }
                    
                else
                    set_direct_era(headPtr, i, R);
        }
    }
    return errors;
}

/* Returns BASE or HIST of symbol. If symbol doesn't exist in symbol table, returns -1. */
int find_symbol_value(head_ptr_t headPtr, char* symbol) {
    int i, length, value;

    length = get_table_used(headPtr);

    for (i = 0; i < length; i++) {
        if (!strcmp(get_symbol_name(headPtr,i), symbol)) {
            value = get_symbol_value(headPtr,i);
            return value;
        }
    }

    return -1; /* Not found. */
}

bool find_symbol_is_extern(head_ptr_t headPtr, char* symbol) {
    int i, length;
    bool is_extern;

    length = get_table_used(headPtr);

    for (i = 0; i < length; i++) {
        if (!strcmp(get_symbol_name(headPtr,i), symbol)) {
            is_extern = get_symbol_isExternal(headPtr,i);
            return is_extern;
        }
    }

    return False; /* Not found. */
}
