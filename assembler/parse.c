#include "parse.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "data_structure.h"
#include "errors.h"
#include "utils.h"

opcode first_group[5] = {MOV, CMP, ADD, SUB, LEA};
opcode second_group[6] = {NOT, CLR, INC, DEC, RED, PRN};
opcode third_group[2] = {RTS, STOP};
opcode jmp_group[3] = {JMP, BNE, JSR};

/* This function parses a data line, and add it into the Data Image table. It returns the updated data-count. It assumes no errors in line. */
int parse_data_line(head_ptr_t headPtr, char* line, int data_count, opcode op) {
    int i, idx, line_num = data_count, arrayLength;
    if (op == STRING) {
        i = 1;
        while (line[i] != '\"') {
            insert_data_img(headPtr, (unsigned int)line[i], line_num++);
            i++;
        }
        insert_data_img(headPtr, 0, line_num++);
    } else {
        i = 0;
        arrayLength = getArrayLength(line);
        for (idx = 0; idx < arrayLength; idx++) {
            insert_data_img(headPtr, atoi(line), line_num++);
            line = nextNum(line);
        }
    }

    return line_num;
}

int get_num_operands(char *line)
{
    unsigned int i = 0;
    char *token = strtok(line, ",");
    while (token && *token && (strchr(token, ',') != NULL)) {
        ++i;
        token = strtok(NULL, ",");
    }
    return i;
}

/* Parses an instruction line, returns the updated instruction-count. */
int parse_inst_line(head_ptr_t headPtr, char* original_line, char *wordPointer_cpy, char* line, char* line_copy, int inst_count, opcode op, bool* errorsFound, int line_num) {
    addr_method sourceAddr, targetAddr;
    char *token, *first_word;
    static line_info_ptr_t instruction, first_param_p, second_param_p;

    instruction = line_info_empty_init();
    first_param_p = line_info_empty_init();
    second_param_p = line_info_empty_init();

    set_opcode(instruction, op);
    if (opcode_in_group(op, third_group, 2)) {
        if (errors_zero_operands_inst(original_line, line, line_num, op)) {
            *errorsFound = True;
            return inst_count;
    }
        insert_base_instruction(headPtr, get_opcode(instruction), 0, 0, A, inst_count++, 0, 0);
        return inst_count;
    }
    if (opcode_in_group(op, second_group, 6)) { /* If the operation requires one operand */

        if (errors_one_operand_inst(original_line, line, line_num, instruction, op)) {
            *errorsFound = True;
            return inst_count;
        }
        targetAddr = operandMethod(line, &instruction, True, NULL, NULL);

        errors_in_addr_method(original_line, line, NULL, line, line_num, instruction, op);

        insert_base_instruction(headPtr, get_opcode(instruction), 0, targetAddr, A, inst_count++, 0, 0);
        inst_count = switch_and_insert(headPtr, instruction, inst_count, targetAddr, True);
    
        return inst_count;
    }

    if (opcode_in_group(op, jmp_group, 3)) {
        
        targetAddr = operandMethod(line, &instruction, True, &first_param_p, &second_param_p);

        if (targetAddr != DIRECT && targetAddr != JMP_PARAM){
            INVALID_ADDR_METHOD(op, 0, targetAddr, line_num, original_line);
            return inst_count;
        }
            
        if (targetAddr == JMP_PARAM) {
                if (errors_jmp_operand_inst(original_line, wordPointer_cpy, line_num, instruction, op)) {
                    return inst_count;
                }
        }

        if (targetAddr == DIRECT) {
                if (errors_one_operand_inst(original_line, wordPointer_cpy, line_num, instruction, op)) {
                    return inst_count;
                }
        }

        insert_base_instruction(headPtr, get_opcode(instruction), 0, targetAddr, A, inst_count++, get_first_param(instruction), get_second_param(instruction));
        insert_direct_instruction(headPtr, get_dst_label(instruction), 0,  A, inst_count++, targetAddr);
                  
        if (!first_param_p)
            return inst_count;
        
        if (get_first_param(instruction) == REG_DIRECT && get_second_param(instruction) == REG_DIRECT) {
            insert_register_instruction(headPtr, get_src_reg(first_param_p), get_dst_reg(second_param_p), A, inst_count++);
            return inst_count;
        }
        inst_count = switch_and_insert(headPtr, first_param_p, inst_count, get_first_param(instruction), False);
        inst_count = switch_and_insert(headPtr, second_param_p, inst_count, get_second_param(instruction), True);

        return inst_count;
    }

    if (opcode_in_group(op, first_group, 5)) { /* If the operation requires two operand */        
        token = strtok(line, ",");
        sourceAddr = operandMethod(token, &instruction, False, NULL, NULL);
        first_word = token;
        token = strtok(NULL, ",");
        if (token != NULL)
            targetAddr = operandMethod(token, &instruction, True, NULL, NULL);
       
        if (errors_two_operands_inst(original_line, line_copy, first_word, token, line_num, instruction, op)) {
            *errorsFound = True;
            return inst_count;
        }

        if (errors_in_addr_method(original_line, line, first_word, token, line_num, instruction, op)) {
            *errorsFound = True;
            return inst_count;
        }

        insert_base_instruction(headPtr, get_opcode(instruction), sourceAddr, targetAddr, A, inst_count++, 0, 0);
        if (sourceAddr == REG_DIRECT && targetAddr == REG_DIRECT) {
            insert_register_instruction(headPtr, get_src_reg(instruction), get_dst_reg(instruction), A, inst_count++);
            return inst_count;
        }
        inst_count = switch_and_insert(headPtr, instruction, inst_count, sourceAddr, False);
        inst_count = switch_and_insert(headPtr, instruction, inst_count, targetAddr, True);
        return inst_count;
    }
    return inst_count;
}

/* Returns True if the line is a comment. Otherwise returns False. */
bool is_comment(char* line) {
    char* p = skip_spaces(line);
    if (*p == ';')
        return True;
    return False;
}

/* Returns array length. Assumes no errors in line and at least one value in array. */
int getArrayLength(char* line) {
    int i = 0, count = 0;
    while (line[i] != '\n' && line[i]) {
        if (line[i] == ',')
            count++;
        i++;
    }
    return count + 1;
}

/* Returns a pointer to the next number in the string. It assumes that a correct string of type ".data" is received (array). */
char* nextNum(char* line) {
    int i = 0;
    while (line[i] != ',' && line[i] != '\n')
        i++;
    return line + i + 1;
}

/* Checks if an operand is an immediate, it assumes so if first character is '#'. It also updates the line_info pointer. */
bool isImmediate(char* arg, line_info_ptr_t instruction, bool isDst) {
    int i = 0;
    if (arg[i++] == '#') {
        if (isDst) {
        set_dst_addr(instruction, IMMEDIATE);
        set_dst_imm(instruction, atoi(arg + i));

    } else {
        set_src_addr(instruction, IMMEDIATE);
        set_src_imm(instruction, atoi(arg + i));
    }
    return True;
    }
    return False;
}

/* The function returns True if the operand is of type Direct. It assumes so if it's not a register/index/immediate and does not check errors. It also updates the line_info pointer.*/
bool isDirect(char* arg, line_info_ptr_t instruction, bool isDst)
{
    if (isDst) {
        set_dst_addr(instruction, DIRECT);
        set_dst_label(instruction, arg, MAX_LABEL_LENGTH);

    } else {
        set_src_addr(instruction, DIRECT);
        set_src_label(instruction, arg, MAX_LABEL_LENGTH);
    }
    return True;
}

bool is_jmp_param(char* arg, line_info_ptr_t instruction, bool isDst, line_info_ptr_t *first_param_info, line_info_ptr_t *second_param_info) {
    int i, labelLength;
    char *token, *line;
    addr_method first_param_m, second_param_m;

    second_param_m = IMMEDIATE;
    i = 0;
    if (isImmediate(arg, instruction, isDst) || isRegister(arg, instruction, isDst))
        return False;
    while (arg[i] != '(' && arg[i])
         i++;
    if (arg[i] == '(') {
        if (arg[strlen(arg) - 1] != ')')
            return False;
            labelLength = i;
            line = arg + labelLength + 1;
            token = strtok(line, ",");
            first_param_m = operandMethod(token, first_param_info, False, NULL, NULL);
            token = strtok(NULL, ")");
            if (token != NULL)
                second_param_m = operandMethod(token, second_param_info, True, NULL, NULL);
            set_first_param(instruction, first_param_m);
            set_second_param(instruction, second_param_m);

     } else {
        *first_param_info = NULL;
        *second_param_info = NULL;
        return False;
     }
    set_dst_addr(instruction, JMP_PARAM);
    set_dst_label(instruction, arg, i);

    return True;
}

/* Returns true if correct register, within range 0EMPTY5. It also updates the line_info pointer.*/
bool isRegister(char* arg, line_info_ptr_t instruction, bool isDst) {
    int i = 0;
    if (arg[i++] == 'r') {
        /* Error check? */
        if (!isdigit(arg[i]) || !(atoi(arg + i) >= 0 && atoi(arg + i) <= MAX_REGISTERS))
            return False;
        if (arg[i + 1] && arg[i + 2])
            return False;
        if (isDst) {
            set_dst_addr(instruction, REG_DIRECT);
            set_dst_reg(instruction, (int)atoi(arg + i));
            
        } else {
            set_src_addr(instruction, REG_DIRECT);
            set_src_reg(instruction, (int)atoi(arg + i));
        }
        arg = arg + 2;
        return True;
    }
    return False;
}

/* Returns the address method of string arg. Assumes arg is not a NULL pointer. */
addr_method operandMethod(char* arg, line_info_ptr_t *instruction, bool isDst, line_info_ptr_t *first_param_info, line_info_ptr_t *second_param_info) {
    if (isRegister(arg, *instruction, isDst))
        return REG_DIRECT;
    else if (isImmediate(arg, *instruction, isDst))
        return IMMEDIATE;
    else if (first_param_info && second_param_info && is_jmp_param(arg, *instruction, isDst, first_param_info, second_param_info))
         return JMP_PARAM;
    else if (isDirect(arg, *instruction, isDst))
        return DIRECT;
    return EMPTY; /* Never reaches here. */
}

bool is_legal_lba(opcode op, addr_method src_mtd, addr_method dst_mtd)
{
    bool err = True;

    switch (op)
    {
    case CMP:
        err = False;
        break;
    case MOV:
    case ADD:
    case SUB:
        if (dst_mtd == IMMEDIATE)
            err = False;
        break;
    case NOT:
    case CLR:
    case INC:
    case DEC:
    case JMP:
    case BNE:
    case RED:
    case JSR:
        if (src_mtd != 0 || dst_mtd == IMMEDIATE)
            err = False;
        break;
    case LEA:
        if ((src_mtd != DIRECT && src_mtd != JMP_PARAM) || dst_mtd == IMMEDIATE)
            err = False;
        break;
    case PRN:
        if (src_mtd != 0)
            err = False;
    case RTS:
    case STOP:
        if (src_mtd != 0 || dst_mtd != 0)
            err = False;
        break;    
    default:
        err = False;
    }
    return err;
}

/* The function returns how many additional words are needed according to the addressing methods of source/target. */
int howManyWords(addr_method sourceAddr, addr_method targetAddr) {
    if (sourceAddr == REG_DIRECT && targetAddr == REG_DIRECT)
        return ZERO_WORDS;
    else if ((sourceAddr == IMMEDIATE && targetAddr == REG_DIRECT) || (sourceAddr == REG_DIRECT && targetAddr == IMMEDIATE)) {
        return ONE_WORD;
    } else if (sourceAddr == IMMEDIATE && targetAddr == IMMEDIATE) {
        return TWO_WORDS_IMMEDIATE;
    } else if ((sourceAddr == REG_DIRECT && (targetAddr == DIRECT || targetAddr == JMP_PARAM)) || (targetAddr == REG_DIRECT && (sourceAddr == DIRECT || sourceAddr == JMP_PARAM))) {
        return TWO_WORDS;
    } else if ((sourceAddr == IMMEDIATE && (targetAddr == DIRECT || targetAddr == JMP_PARAM)) || (targetAddr == IMMEDIATE && (sourceAddr == DIRECT || sourceAddr == JMP_PARAM))) {
        return THREE_WORDS;
    } else if ((sourceAddr == DIRECT || sourceAddr == JMP_PARAM) && (targetAddr == DIRECT || targetAddr == JMP_PARAM)) {
        return FOUR_WORDS;
    }
    return EMPTY; /* Should never reach here. */
}
