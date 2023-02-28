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
int process_first_pass(head* headPtr, char* filename) {
    int inst_count, data_count, shift, line_num;
    char line[MAX_LINE_LENGTH], label[MAX_LABEL_LENGTH], line_copy[MAX_LINE_LENGTH], original_line[MAX_LINE_LENGTH];
    char* wordPointer;
    opcode op;
    bool isLabel, errorsFound;
    FILE* filePointer;
    int err;

    inst_count = INITIAL_IC;
    data_count = line_num = 0;
    isLabel = errorsFound = False;
    filePointer = fopen(filename, "r");

    while (fgets(line, MAX_LINE_LENGTH, filePointer)) {
        line_num++;
        if (empty_string(line) || is_comment(line))
            continue;

      
        strcpy(original_line, line);
    printf("[%d]: original_line = %s, line = %s, count = %d\n",
            __LINE__, original_line, line, inst_count);

        wordPointer = skip_spaces(line); /* Points at first word */

        printf("[%d]: original_line = %s, line = %s, count = %d, wordPointer = %s\n",
            __LINE__, original_line, line, inst_count, wordPointer);

        if ((shift = label_check(wordPointer, label)) != -1) {
            isLabel = True;
            //err += (errors_in_label(headPtr, original_line, wordPointer, line_num));  TODO!!
            wordPointer += shift;
            wordPointer = skip_spaces(wordPointer);
        }

        // printf("[%d]: original_line = %s, wordPointer =  %s, line = %s, count = %d, op = %d\n",
        //     __LINE__, original_line, wordPointer, line_copy, inst_count, (int)op);

        /* By here, points at first word after label, if label exists. */
        op = firstWord(wordPointer);
        wordPointer = skip_word(wordPointer);
        delete_spaces(wordPointer); //= STR,r6
        strcpy(line_copy, wordPointer);  //= STR,r6


        printf("[%d]: wordPointer = %s, line_copy = %s, op = %d\n",
            __LINE__, wordPointer, line_copy, (int)op);

    
        if (op == ENTRY) /* Second Pass deals with this case. */
            continue;
        if (op == DATA || op == STRING) {
            if (isLabel)
                insert_data_symbol(headPtr, label, data_count, op);

            isLabel = False;

            errorsFound = (errors_in_data_line(original_line, wordPointer, line_num, op)) ? True : errorsFound;

            if (!errorsFound)
                data_count = parse_data_line(headPtr, wordPointer, data_count, op);
            continue;
        }
        if (op == EXTERNAL) {
            /* Error detection done during second pass. */
            insert_extern(headPtr, wordPointer, op);
            isLabel = False;
            continue;
        }

        /* By this point it must be an instruction line. */
        if (isLabel)
            insert_code_symbol(headPtr, label, inst_count, op);

        isLabel = False;


        printf("[%d]: original_line = %s, wordPointer =  %s, line = %s, count = %d, op = %d\n",
            __LINE__, original_line, wordPointer, line_copy, inst_count, (int)op);
        inst_count = parse_inst_line(headPtr, original_line, wordPointer, line_copy, inst_count, op, &errorsFound, line_num);
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
        if (label != NULL) {
            strncpy(label, ptr, i);
            label[i] = 0;
        }
        return i + 1;
    }
    return -1;
}
/* Returns number of operation (first word). If it's incorrect then it returns -1. */
opcode firstWord(char* line) {
    //char* opNames[INST_AND_DIR_NUM] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop", ".data", ".string", ".entry", ".extern"};
    int count = 0, i;
    for (i = 0; !isspace(line[i]) && line[i] != '\n'; i++)
        count++; /* Count characters of first word. */
    for (i = 0; i < OPCODE_SIZE; i++) {
        if (strlen(opcode_to_str((opcode)i)) == count && !strncmp(line, opcode_to_str((opcode)i), count)) {
            printf("%s: %d\n",opcode_to_str((opcode)i), i);
            return i;
        }
    }
    return -1;
}
