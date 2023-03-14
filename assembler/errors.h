#ifndef ERRORS_H
#define ERRORS_H

#include "constants.h"
#include "data_structure.h"

#define MAX_ALNUM 128

/* Macros for common errors: */
#define CONSECUTIVE_COMMAS(str, line, original)                                       \
    if (isConsecutiveComma(str)) {                                                    \
        printf("Error on line %d: %sMultiple consecutive commas.\n", line, original); \
        return True;                                                                  \
    }

#define EXTRANEOUS_TEXT(ch, line, original)                                                 \
    if (ch) {                                                                               \
        printf("Error on line %d: %sExtraneous text after end of line.\n", line, original); \
        return True;                                                                        \
    }

#define COMMA_END(str, line, original)                                                      \
    if (str[strlen(str) - 1] == ',') {                                                      \
        printf("Error on line %d: %sExtraneous text after end of line.\n", line, original); \
        return 1;                                                                        \
    }

#define COMMA_START(str, line, original)                                                      \
    if (str[0] == ',') {                                                      \
        printf("Error on line %d: %sExtraneous text before start of line.\n", line, original); \
        return 1;                                                                        \
    }                                                                                        

#define ERR_LONG_LINE(line, original)                                                                                               \
        printf("Error on line %d: %sLine over length.\n", line, original); \


#define ERR_DIRECT(str, line, original)                                                                                               \
    if (errors_in_label_format(strlen(str), str)) {                                                                                 \
        printf("Error on line %d: %sIncorrect direct label format.\n", line, original); \
        return True;                                                                                                                     \
    }

#define ERR_IMMEDIATE(str, line, original)                                                                                               \
    if (errors_immediate(str)) {                                                                                                         \
        printf("Error on line %d: %sIncorrect immediate number. Must be '#' followed by '+', '-' and integers only.\n", line, original); \
        return True;                                                                                                                     \
    }

#define ERR_JMP(str, line, original)                                                                                                                              \
    if (error_jmp_group(str)) {                                                                                                                                        \
        printf("Error on line %d: %sJmp format incorrect. Must be a label name followed by 2 operands surrounded by brackets ().\n", line, original); \
        return True;                                                                                                                                                \
    }

#define INVALID_OPERANDS(str, op, line, original)                                    \
    if (is_invalid_operand_num(str, op)) {                                           \
        printf("Error on line %d: %sInvalid number of operands.\n", line, original); \
        return 1;                                                                 \
    }

#define INVALID_ADDR_METHOD(op, src, dst, line, original)                                 \
    if (!is_legal_lba(op, src, dst)) {                                                                   \
        printf("Error on line %d: %sInvalid addressing method.\n", line, original); \
        return True;                                                                \
    }

bool err_in_opcode(head_ptr_t headPtr, char* original_line, opcode op, int lineNumber);
bool err_label(head_ptr_t headPtr, char* original_line, int label_len, char* line, int lineNumber, bool is_entry);
bool errors_in_label_format(int label_len, char* line);
bool errors_in_extern_label(head_ptr_t headPtr, char* original_line, char* line, int lineNumber);
bool is_duplicate_label(head_ptr_t, char *, int);
int count_parameters(char *str);

bool errors_in_data_line(char *, char *, int, opcode);
bool errors_zero_operands_inst(char *, char *, int, opcode);
int errors_one_operand_inst(char* original_line, char* line, int lineNumber, line_info_ptr_t instruction, opcode op);
int check_one_operand_num(char* str, opcode op);
int errors_two_operands_inst(char *, char *, char *, char *, int, line_info_ptr_t, opcode op);
int check_two_operand_num(char* str, opcode op);
bool error_jmp_group(char* original_line);
bool errors_jmp_operand_inst(char* original_line, char* line, int lineNumber, line_info_ptr_t instruction, opcode op);
bool errors_in_addr_method(char* original_line, char* line, char* first_word, char* second_word, int lineNumber, line_info_ptr_t instruction, opcode op);
bool errors_immediate(char *);
bool errors_index(char *);
int is_invalid_operand_num(char *, opcode);
bool is_one_operand_num(char* str, opcode op);
bool is_two_operand_num(char* str, opcode op);
bool illegal_two_operands(addr_method, addr_method, opcode);

bool isCorrectNum(char *);
bool isConsecutiveComma(char *);
bool isExistentFile(char *);

#endif
