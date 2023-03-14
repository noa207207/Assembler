#ifndef PARSE_H
#define PARSE_H

#include "constants.h"
#include "data_structure.h"

enum addiotonal_words { ZERO_WORDS,
                        ONE_WORD,
                        TWO_WORDS_IMMEDIATE,
                        TWO_WORDS,
                        THREE_WORDS,
                        FOUR_WORDS };

extern opcode first_group[5];
extern opcode second_group[6];
extern opcode third_group[2];
extern opcode jmp_group[3];

int parse_data_line(head_ptr_t, char *, int, opcode);
int parse_inst_line(head_ptr_t headPtr, char* original_line, char *wordPointer_cpy, char* line, char* line_copy, int inst_count, opcode op, bool* errorsFound, int line_num);
bool is_comment(char *);
int getArrayLength(char *);
char *nextNum(char *);

bool isImmediate(char *, line_info_ptr_t, bool);
bool isDirect(char *, line_info_ptr_t, bool);
bool isRegister(char *, line_info_ptr_t, bool);
bool is_jmp_param(char* arg, line_info_ptr_t instruction, bool isDst, line_info_ptr_t *first_param_info, line_info_ptr_t *second_param_info);

void updateFunctAndOpcode(opcode, line_info_ptr_t);
addr_method operandMethod(char* arg, line_info_ptr_t *instruction, bool isDst, line_info_ptr_t *first_param_info, line_info_ptr_t *second_param_info);
int howManyWords(addr_method, addr_method);
bool is_legal_lba(opcode op, addr_method src_mtd, addr_method dst_mtd);

#endif
