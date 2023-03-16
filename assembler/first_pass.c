#include "first_pass.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structure.h"
#include "errors.h"
#include "parse.h"
#include "utils.h"
#include "constants.h"

/* The function is in charge of first pass. It returns False if no errors were found, otherwise returns True. */
int process_first_pass(head_ptr_t headPtr, char* filename) {
    int inst_count, data_count, shift, line_num;
    char line[MAX_LINE_LENGTH], label[MAX_LABEL_LENGTH], line_copy[MAX_LINE_LENGTH], original_line[MAX_LINE_LENGTH], wordPointer_cpy[MAX_LINE_LENGTH];
    char* wordPointer;
    opcode op;
    bool isLabel, errorsFound;
    FILE* filePointer;
    int prev_inst_count, prev_data_count;
    int c;

    inst_count = INITIAL_IC;
    data_count = line_num = 0;
    isLabel = errorsFound = False;
    filePointer = fopen(filename, "r");

    while (fgets(line, MAX_LINE_LENGTH, filePointer)) {
        
        if (line[strlen(line) - 1] != '\n' && line[strlen(line)] != EOF) {
            ERR_LONG_LINE(++line_num, line)
            
        /* Discard the remaining characters in the line */ 
    
        while ((c = fgetc(filePointer)) != '\n' && c != EOF) {}
        
        if (c == EOF) {
            /* End-of-file reached while discarding characters */ 
            break;
        }
        continue;
    }
        
        prev_inst_count = inst_count;
        prev_data_count = data_count;
        errorsFound = False;
        isLabel = False;
        line_num++;
        if (empty_string(line) || is_comment(line)){
            continue;
        }

        strcpy(original_line, line);

        wordPointer = skip_spaces(line); /* Points at first word */

        

        if ((shift = label_check(wordPointer, label)) != -1) {
            isLabel = True;
            errorsFound = (err_label(headPtr, original_line, shift - 1, wordPointer, line_num, False));
            if (errorsFound)
                continue;
            wordPointer += shift;
            wordPointer = skip_spaces(wordPointer);
        }

        /* By here, points at first word after label, if label exists. */
        
        op = firstWord(wordPointer);
    
        if (err_in_opcode(headPtr, original_line, op, line_num))
            continue;
    
        wordPointer = skip_word(wordPointer);
        wordPointer = skip_spaces(wordPointer);
        strcpy(wordPointer_cpy, wordPointer);
        remove_end_spaces(wordPointer_cpy);
        removeSpacesAndTabs(wordPointer);
        strcpy(line_copy, wordPointer);

        if (op == ENTRY) /* Second Pass deals with this case. */
            continue;
        if (op == DATA || op == STRING) {
            errorsFound = (errors_in_data_line(original_line, wordPointer, line_num, op)) ? True : errorsFound;

            if (errorsFound)
                continue;
            data_count = parse_data_line(headPtr, wordPointer, data_count, op);
            if (isLabel && data_count > prev_data_count)
                insert_data_symbol(headPtr, label, prev_data_count, op);         
        }
        if (op == EXTERNAL) {
            errorsFound = (err_label(headPtr, original_line, strlen(wordPointer_cpy), wordPointer_cpy, line_num, False));
            if (errorsFound)
                continue;
            insert_extern(headPtr, wordPointer, op);
            isLabel = False;
            continue;
        }

        inst_count = parse_inst_line(headPtr, original_line, wordPointer_cpy, wordPointer, line_copy, inst_count, op, &errorsFound, line_num);
        if (isLabel && inst_count > prev_inst_count)
            insert_code_symbol(headPtr, label, prev_inst_count, (int)op);
    }
    fclose(filePointer);

    update_data_count(headPtr, inst_count);

    return errorsFound;
}

/* The function checks if there's a label in line. If so, it copies it into label, and returns the index of the character after ':'. Otherwise returns -1. */
int label_check(char* line, char* label) {
    char* ptr = line;
    int i = 0;
    while (ptr[i] != ':' && ptr[i])
        i++;
    if (ptr[i] == ':') {
        if (label != NULL && i < MAX_LABEL_LENGTH) {
            strncpy(label, ptr, i);
            label[i] = 0;
        }
        return i + 1;
    }
    return -1;
}
/* Returns number of operation (first word). If it's incorrect then it returns -1. */
opcode firstWord(char* line) {
    int count = 0, i;
    for (i = 0; !isspace(line[i]) && line[i] != '\n' && line[i] != ','; i++)
        count++; /* Count characters of first word. */
    for (i = 0; i < OPCODE_SIZE; i++) {
        if (strlen(opcode_to_str((opcode)i)) == count && !strncmp(line, opcode_to_str((opcode)i), count)) {
            return i;
        }
    }
    return -1;
}
