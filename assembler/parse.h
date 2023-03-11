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

int parse_data_line(head *, char *, int, opcode);
int parse_inst_line(head *, char *, char *, char *, int, opcode, bool *, int);

bool is_comment(char *);
int getArrayLength(char *);
char *nextNum(char *);

bool isImmediate(char *, line_info *, bool);
bool isDirect(char *, line_info *, bool);
bool isRegister(char *, line_info *, bool);
bool is_jmp_param(char* arg, line_info* instruction, bool isDst, line_info **first_param_info, line_info **second_param_info);

void updateFunctAndOpcode(opcode, line_info *);
addr_method operandMethod(char* arg, line_info* instruction, bool isDst, line_info **first_param_info, line_info **second_param_info);
int howManyWords(addr_method, addr_method);
bool is_legal_lba(opcode op, addr_method src_mtd, addr_method dst_mtd);
int switch_and_insert(head* arr, line_info *instruction, int inst_count, addr_method address_method, bool is_dst);

#endif