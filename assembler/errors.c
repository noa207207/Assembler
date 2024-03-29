#include "errors.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "utils.h"
#include "data_structure.h"
#include "parse.h"


bool err_in_opcode(head_ptr_t headPtr, char* original_line, opcode op, int lineNumber)
{
    if (!(op >= 0 && op <OPCODE_SIZE)) {
        printf("Error on line %d: %sUnknown opcode.\n", lineNumber, original_line);
        return True;
    }
    return False;

}
bool err_label(head_ptr_t headPtr, char* original_line, int label_len, char* line, int lineNumber, bool is_entry)
{
    if (errors_in_label_format(label_len, line)) {
        printf("Error on line %d: %sLabel has to be at most 31 characters long, first character has to be a letter, cannot be a saved word and all characters have to be alphaneumeric.\n", lineNumber, original_line);
        return True;
    }

    if (!is_entry && is_duplicate_label(headPtr, line, label_len)) {
        printf("Error on line %d: %sDuplicate label name. The name has already been used.\n", lineNumber, original_line);
        return True;
    }

        if (is_entry && !is_duplicate_label(headPtr, line, label_len)) {
        printf("Error on line %d: %sLabel name not exist in file.\n", lineNumber, original_line);
        return True;
    }

    return False;

}

bool errors_in_label_format(int label_len, char* line) {
    char* ptr;
    int i, length, reg;

    i = 0;
    ptr = line;

    length = label_len;

    if (length == 0 || length > MAX_LABEL_LENGTH || isdigit(ptr[0]))
        return True;

    for (i = 0; i < length; i++) {
        if (!isalnum(ptr[i]))
            return True;
    }

    for (i = 0; i < OPCODE_SIZE; i++) {
        if (strlen(opcode_to_str(i)) == length && !strncmp(line, opcode_to_str(i), length))
            return True;
    }

    reg = atoi(ptr + 1);

    if (ptr[0] == 'r' && ((reg >= 0 && reg < MAX_REGISTERS && length == strlen("r0"))))
        return True;

    return False;
}

bool errors_in_data_line(char* original_line, char* line, int lineNumber, int op) {
    char *ptr, *token;
    char line_copy[MAX_LINE_LENGTH];
    bool errors;
    int i = 0;

    ptr = line;

    errors = False;

    if (op == STRING) {
        if (ptr[i++] != '\"')
            errors = True;
        
         while (ptr[i] != '\"' && ptr[i])
            i++;

        if (!ptr[i])
            errors = True;

        if (!errors) /* Ensures we are in range of array. */
            EXTRANEOUS_TEXT(ptr[i + 1], lineNumber, original_line)

        if (errors) {
            printf("Error on line %d: %sString has to contain ASCII characters surrounded by parentheses (\").\n", lineNumber, original_line);
            return True;
        }
    } else if (op == DATA) {
        if (!ptr[i])
            errors = True;
        strcpy(line_copy, line);

        CONSECUTIVE_COMMAS(line, lineNumber, original_line)

        COMMA_END(line, lineNumber, original_line);

        token = strtok(line_copy, ",");

        while (token != NULL) {
            errors = (isCorrectNum(token)) ? errors : True;
            token = strtok(NULL, ",");
        }
        if (errors) {
            printf("Error on line %d: %sArray of type \".data\" has to contain integer numbers, separated by commas.\n", lineNumber, original_line);
            return True;
        }
    }
    return False;
}

int count_parameters(char *str)
{
    int count = 0;
    char *token;
    token = strtok(str, ", ");
    while (token != NULL) {
        count++;
        token = strtok(NULL, ", ");
    }
    return count;
}

bool errors_in_addr_method(char* original_line, char* line, char* first_word, char* second_word, int lineNumber, line_info_ptr_t instruction, opcode op)
{
    if (get_dst_addr(instruction) == IMMEDIATE) {
        ERR_IMMEDIATE(second_word, lineNumber, original_line);
    } else if (get_dst_addr(instruction) == DIRECT) {
            ERR_DIRECT(second_word, lineNumber, original_line);
    }
        
    if (first_word) {
        if (get_src_addr(instruction) == IMMEDIATE) {
            ERR_IMMEDIATE(first_word, lineNumber, original_line); 
        } else if (get_src_addr(instruction) == DIRECT) {
            ERR_DIRECT(line, lineNumber, original_line);
        }
    }

    if (!is_legal_lba(get_opcode(instruction), get_src_addr(instruction), get_dst_addr(instruction))) {                                                                   \
            printf("Error on line %d: %sInvalid addressing method.\n", lineNumber, original_line);
            return True;                                                 
        }
    return False;
}

bool errors_zero_operands_inst(char* original_line, char* line, int lineNumber, opcode op) {
    if (op == RTS || op == STOP)
        EXTRANEOUS_TEXT(line[0], lineNumber, original_line)
    return False;
}

int errors_one_operand_inst(char* original_line, char* line, int lineNumber, line_info_ptr_t instruction, opcode op) {
    /* Only destination matters. REG_DIRECT that is incorrect considered as DIRECT.*/
    int err = 0;

    INVALID_OPERANDS(line, op, lineNumber, original_line)

    COMMA_END(line, lineNumber, original_line);

    COMMA_START(line, lineNumber, original_line);

    return err;
}

int errors_two_operands_inst(char* original_line, char* line, char* first_word, char* second_word, int lineNumber, line_info_ptr_t instruction, opcode op) {
    int err = 0;

    INVALID_OPERANDS(line, op, lineNumber, original_line);

    COMMA_END(line, lineNumber, original_line);
    COMMA_START(line, lineNumber, original_line);

    CONSECUTIVE_COMMAS(line, lineNumber, original_line);

    return err;
}

bool errors_jmp_operand_inst(char* original_line, char* line, int lineNumber, line_info_ptr_t instruction, opcode op, line_info_ptr_t first_param, line_info_ptr_t sec_param)
{
    ERR_JMP(line, lineNumber, original_line);

    if (get_dst_addr(sec_param) == IMMEDIATE) {
        ERR_IMMEDIATE(get_jmp_sec_p(sec_param), lineNumber, original_line);
    } else if (get_dst_addr(sec_param) == DIRECT) {
            ERR_DIRECT(get_jmp_sec_p(sec_param), lineNumber, original_line);
    }
    if (get_src_addr(first_param) == IMMEDIATE) {
        ERR_IMMEDIATE(get_jmp_first_p(first_param), lineNumber, original_line);
    } else if (get_src_addr(first_param) == DIRECT) {
            ERR_DIRECT(get_jmp_first_p(first_param), lineNumber, original_line);
    }
    return False;
}

bool error_jmp_group(char* original_line)
{
    bool err = False;
    char*token;
    int i = 0;
    int len = strlen(original_line);

    if (original_line[len - 1] != ')')
        return True;
    while (original_line[i] && original_line[i] != '(')
        i++;
    if (!original_line[i])
        return True;
    
        token = strtok(original_line + 2, ",");
        if (token == NULL || strchr(token, ' ') || strchr(token, '\t'))
            return True;
    
        token = strtok(NULL, ",");
        if (token == NULL || strchr(token, ' ') || strchr(token, '\t'))
            return True;
    return err;
}

bool errors_immediate(char* str) {
    int i = 1;
    if (!isdigit(str[i]) && str[i] != '+' && str[i] != '-')
        return True;
    while (str[++i])
        if (!isdigit(str[i]))
            return True;
    return False;
}

int is_invalid_operand_num(char* str, opcode op) {
    if (opcode_in_group(op, second_group, 6) || opcode_in_group(op, jmp_group, 3))
        return count_parameters(str) == 1 ? 0 : 1;
    if (opcode_in_group(op, first_group, 5))
        return count_parameters(str) == 2 ? 0 : 1;

    return 0;
}

bool isCorrectNum(char* str) {
    int i, length;

    i = 0;

    if (!isdigit(str[i]) && str[i] != '+' && str[i] != '-')
        return False;

    length = strlen(str);

    for (i = 1; i < length; i++) {
        if (!isdigit(str[i]))
            return False;
    }
    return True;
}

bool isConsecutiveComma(char* str) {

    int i = 0;
    for (i = 0; i < strlen(str); i++) {
        if (str[i] == ',' && str[i + 1] == ',')
            return True;
    }
    return False;
}

bool isExistentFile(char* filename) {
    FILE* filePointer;

    filePointer = fopen(filename, "r");

    if (!filePointer) {
        printf("Error: File named \"%s\" not found. Continuing to the next one.\n\n", filename);
        return False;
    }
    fclose(filePointer);
    return True;
}
