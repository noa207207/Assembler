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

/* Parses a data line, and add it into the Data Image table. 
 * Return the updated data-count. (assuming no errors in line) */
int parse_data_line(head_ptr_t, char *, int, opcode);

/* Parses an instruction line.
 * Return the updated instruction-count. */
int parse_inst_line(head_ptr_t headPtr, char* original_line, char *wordPointer_cpy, char* line, char* line_copy, int inst_count, opcode op, bool* errorsFound, int line_num);

/* Returns True if the line is a comment. Otherwise returns False. */
bool is_comment(char *);

/* Returns array length. Assumes no errors in line 
 * and at least one value in array. */
int getArrayLength(char *);

/* Returns a pointer to the next number in the string.
 * It assumes that a correct string of type ".data" is received (array). */
char *nextNum(char *);

/* Checks if an operand is an immediate and update line_info pointer accordingly
 * it assumes so if first character is '#' */
bool isImmediate(char *, line_info_ptr_t, bool);

/* Checks if the operand is of type Direct. and update line_info pointer accordingly
 * It assumes so if it's not a register/jmp/immediate and does not check errors */
bool isDirect(char *, line_info_ptr_t, bool);

/* Returns true if correct register. and update line_info pointer accordingly.*/
bool isRegister(char *, line_info_ptr_t, bool);

/* Checks if the operand is of type jmp with parameters. and update line_info pointer accordingly
 * Fill parameters line info wit their address method and values */
bool is_jmp_param(char* arg, line_info_ptr_t instruction, bool isDst, line_info_ptr_t *first_param_info, line_info_ptr_t *second_param_info);

/* Returns the address method of string arg. Assumes arg is not a NULL pointer. */
addr_method operandMethod(char* arg, line_info_ptr_t *instruction, bool isDst, line_info_ptr_t *first_param_info, line_info_ptr_t *second_param_info);

/* Check if operands address method is illegal according language settings */
bool is_legal_lba(opcode op, addr_method src_mtd, addr_method dst_mtd);

#endif
