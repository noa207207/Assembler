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

/* Check if opcode is in opcodes range */
bool err_in_opcode(head_ptr_t headPtr, char* original_line, opcode op, int lineNumber);

/* Check label validation */
bool err_label(head_ptr_t headPtr, char* original_line, int label_len, char* line, int lineNumber, bool is_entry);

/* Checks if the label is correct. Assumes line has a label. */
bool errors_in_label_format(int label_len, char* line);

/* count instruction line params num */
int count_parameters(char *str);

/* Detect errors in a data line, where line points to first element after ".string"/".data". */
bool errors_in_data_line(char *, char *, int, int opcode);

/* Detect errors in an instruction line with zero operands. */
bool errors_zero_operands_inst(char *, char *, int, opcode);

/* Detect errors in an instruction line with one operand */
int errors_one_operand_inst(char* original_line, char* line, int lineNumber, line_info_ptr_t instruction, opcode op);

/* Detect errors in an instruction line with two operands */
int errors_two_operands_inst(char *, char *, char *, char *, int, line_info_ptr_t, opcode op);

/* Detect and print errors in an jmp with params instruction line */
bool error_jmp_group(char* original_line);

/* Detect errors in an jmp with params instruction line */
bool errors_jmp_operand_inst(char* original_line, char* line, int lineNumber, line_info_ptr_t instruction, opcode op);

/* Detect and print errors after decoding address method */
bool errors_in_addr_method(char* original_line, char* line, char* first_word, char* second_word, int lineNumber, line_info_ptr_t instruction, opcode op);

/* Check immediate format validation */
bool errors_immediate(char *);

/* Detect and print error is num operands not match opcode */
int is_invalid_operand_num(char *, opcode);

/* Checks if a string is a correct number - starts with '+' or '-' followed by digits or all digits. */
bool isCorrectNum(char *);

/* Check if there are two (or more) consecutive commas in the string provided. */
bool isConsecutiveComma(char *);

/* Check if the filename provided exists, otherwise returns False. */
bool isExistentFile(char *);

#endif
