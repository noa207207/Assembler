#include "parse.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "data_structure.h"
#include "errors.h"
#include "utils.h"

opcode first_group[] = {MOV, CMP, ADD, SUB, LEA};
opcode second_group[] = {NOT, CLR, INC, DEC, JMP, BNE, RED, PRN, JSR};
opcode third_group[] = {RTS, STOP};

/* This function parses a data line, and add it into the Data Image table. It returns the updated data-count. It assumes no errors in line. */
int parse_data_line(head* headPtr, char* line, int data_count, opcode op) {
    int i, idx, line_num = data_count, arrayLength;
    if (op == STRING) {
        i = 1;
        while (line[i] != '\"') {
            //insert_data_img(headPtr, (unsigned int)line[i], line_num++);
            i++;
        }

        //insert_data_img(headPtr, 0, line_num++);
    } else {
        i = 0;
        arrayLength = getArrayLength(line);
        for (idx = 0; idx < arrayLength; idx++) {
            //insert_data_img(headPtr, atoi(line), line_num++);
            line = nextNum(line);
        }
    }

    return line_num;
}

/* Parses an instruction line, returns the updated instruction-count. */
int parse_inst_line(head* headPtr, char* original_line, char* line, char* line_copy, int inst_count, opcode op, bool* errorsFound, int line_num) {
    addr_method sourceAddr, targetAddr;
    char *token, *first_word;
    char label[MAX_LABEL_LENGTH];
    int immediate, additionalWords;
    line_info instruction = {0};

    //TODO:
    // if (errors_zero_operands_inst(original_line, line, line_num, op)) {
    //     *errorsFound = True;
    //     return inst_count;
    // }

    instruction.opcode = op;
    if (opcode_in_group(op, third_group, 2)) {
        insert_base_instruction(headPtr, instruction.opcode, 0, 0, A, inst_count++);
        return inst_count;
    }

    if (opcode_in_group(op, second_group, 9)) { /* If the operation requires one operand */
        targetAddr = operandMethod(line, &instruction, True);
        //TODO:
        // if (errors_one_operand_inst(original_line, line, line_num, &instruction)) {
        //     *errorsFound = True;
        //     return inst_count;
        // }

        insert_base_instruction(headPtr, instruction.opcode, 0, targetAddr, A, inst_count++);
        switch (targetAddr)
        {
            case REG_DIRECT:
                insert_register_instruction(headPtr, 0, instruction.dst_reg, A, inst_count++);

        default:
            break;
        }
    
        return inst_count;
    }

    if (opcode_in_group(op, first_group, 5)) { /* If the operation requires one operand */
        token = strtok(line, ",");
        sourceAddr = operandMethod(token, &instruction, False);
        printf("source = %d\n", (int)sourceAddr);
        first_word = token;
        token = strtok(NULL, ",");
        if (token != NULL)
        targetAddr = operandMethod(token, &instruction, True);
         printf("source = %d, target = %d\n", (int)sourceAddr, (int)targetAddr);
        //TODO:
        // if (errors_one_operand_inst(original_line, line, line_num, &instruction)) {
        //     *errorsFound = True;
        //     return inst_count;
        // }
        insert_base_instruction(headPtr, instruction.opcode, sourceAddr, targetAddr, A, inst_count++);
        if (sourceAddr == REG_DIRECT && targetAddr == REG_DIRECT) {
            insert_register_instruction(headPtr, instruction.src_reg, instruction.dst_reg, A, inst_count++);
            return inst_count;
        }
         printf("source = %d\n", (int)sourceAddr);
        switch (sourceAddr)
        {
            case REG_DIRECT:
            insert_register_instruction(headPtr, instruction.src_reg, 0, A, inst_count++);
            printf("aa = %d\n", (int)sourceAddr);
            break;

            case IMMEDIATE:
            insert_immidiate_instruction(headPtr,instruction.src_imm, A, inst_count++);
            printf("bb = %d\n", (int)sourceAddr);
            break;

            case DIRECT:
            printf("fklklkl\n");
            insert_direct_instruction(headPtr, instruction.src_label, 0, A, inst_count++);
            break;

        default:
            break;
        }

        switch (targetAddr)
        {
            case REG_DIRECT:
            insert_register_instruction(headPtr, 0, instruction.dst_reg, A, inst_count++);
            break;

            case DIRECT:
            insert_direct_instruction(headPtr, instruction.dst_label, 0, A, inst_count++);

        default:
            break;
        }
    
        return inst_count;
    }

    
    // /* By now it must have two operands. Error check to make sure it's fine. */
    // token = strtok(line, ",");
    // sourceAddr = operandMethod(token, &instruction, False);

    // first_word = token;

    // token = strtok(NULL, ",");

    // if (token != NULL)
    //     targetAddr = operandMethod(token, &instruction, True);

    // if (errors_two_operands_inst(original_line, line_copy, first_word, token, line_num, &instruction)) {
    //     *errorsFound = True;
    //     return inst_count;
    // }

    // insert_code_opcode(headPtr, A, instruction.opcode , inst_count++);
    // insert_code_instruction(headPtr, &instruction, inst_count++);

    // additionalWords = howManyWords(sourceAddr, targetAddr);

    // if (additionalWords == ZERO_WORDS)
    //     return inst_count;
    // else if (additionalWords == ONE_WORD) {
    //     immediate = (sourceAddr == IMMEDIATE) ? instruction.src_imm : instruction.dst_imm;
    //     insert_code_immediate(headPtr, immediate, A, inst_count++, label, DONE);
    // } else if (additionalWords == TWO_WORDS_IMMEDIATE) {
    //     insert_code_immediate(headPtr, instruction.src_imm, A, inst_count++, label, DONE);
    //     insert_code_immediate(headPtr, instruction.dst_imm, A, inst_count++, label, DONE);
    // } else if (additionalWords == TWO_WORDS) {
    //     strcpy(label, (sourceAddr == REG_DIRECT) ? instruction.dst_label : instruction.src_label);
    //     insert_code_immediate(headPtr, 0, 0, inst_count++, label, BASE);
    //     insert_code_immediate(headPtr, 0, 0, inst_count++, label, HIST);
    // } else if (additionalWords == THREE_WORDS) {
    //     immediate = (sourceAddr == IMMEDIATE) ? instruction.src_imm : instruction.dst_imm;
    //     strcpy(label, (sourceAddr == IMMEDIATE) ? instruction.dst_label : instruction.src_label);

    //     if (sourceAddr == IMMEDIATE) {
    //         insert_code_immediate(headPtr, immediate, A, inst_count++, label, DONE);
    //         insert_code_immediate(headPtr, 0, 0, inst_count++, label, BASE);
    //         insert_code_immediate(headPtr, 0, 0, inst_count++, label, HIST);
    //     } else {
    //         insert_code_immediate(headPtr, 0, 0, inst_count++, label, BASE);
    //         insert_code_immediate(headPtr, 0, 0, inst_count++, label, HIST);
    //         insert_code_immediate(headPtr, immediate, A, inst_count++, label, DONE);
    //     }
    // } else if (additionalWords == FOUR_WORDS) {
    //     strcpy(label, instruction.src_label);
    //     insert_code_immediate(headPtr, 0, 0, inst_count++, label, BASE);
    //     insert_code_immediate(headPtr, 0, 0, inst_count++, label, HIST);

    //     strcpy(label, instruction.dst_label);
    //     insert_code_immediate(headPtr, 0, 0, inst_count++, label, BASE);
    //     insert_code_immediate(headPtr, 0, 0, inst_count++, label, HIST);
    // }
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
bool isImmediate(char* arg, line_info* instruction, bool isDst) {
    int i = 0;
    if (arg[i++] == '#') {
        if (isDst)
            instruction->dst_imm = atoi(arg + i);
        else
            instruction->src_imm = atoi(arg + i);
        return True;
    }
    return False;
}

/* The function returns True if the operand is of type Direct. It assumes so if it's not a register/index/immediate and does not check errors. It also updates the line_info pointer.*/
bool isDirect(char* arg, line_info* instruction, bool isDst)
{
   printf("direct, %s\n", arg);
    if (isDst) {
        instruction->dst_addr = DIRECT;
        strcpy(instruction->dst_label, arg);
    } else {
        instruction->src_addr = DIRECT;
        strcpy(instruction->src_label, arg);
    }
    return True;
}

/* Returns True if Index operand, if it has character '['. No other operand method is allowed to have that character. Does not error check at this point. It also updates the line_info pointer.*/
bool isIndex(char* arg, line_info* instruction, bool isDst) {
    int i, labelLength, reg;

    i = 0;

    if (isImmediate(arg, instruction, isDst) || isRegister(arg, instruction, isDst))
        return False;
    while (arg[i] != '[' && arg[i])
        i++;
    if (arg[i] == '[') {
        /* Error check */
        labelLength = i;
        reg = atoi(arg + labelLength + 2);
        if (isDst) {
            instruction->dst_addr = JMP_M;
            instruction->dst_reg = reg;
            strncpy(instruction->dst_label, arg, labelLength);
        } else {
            instruction->src_addr = JMP_M;
            instruction->src_reg = reg;
            strncpy(instruction->src_label, arg, labelLength);
        }
        return True;
    }
    return False;
}

/* Returns true if correct register, within range 0-15. It also updates the line_info pointer.*/
bool isRegister(char* arg, line_info* instruction, bool isDst) {
    int i = 0;
    if (arg[i++] == 'r') {
        /* Error check? */
        if (!isdigit(arg[i]) || !(atoi(arg + i) >= 0 && atoi(arg + i) < MAX_REGISTERS))
            return False;
        if (arg[i + 1] && arg[i + 2])
            return False;
        if (isDst) {
            instruction->dst_addr = REG_DIRECT;
            instruction->dst_reg = atoi(arg + i);
            
        } else {
            instruction->src_addr = REG_DIRECT;
            instruction->src_reg = atoi(arg + i);
        }
        return True;
    }
    return False;
}

/* Returns the address method of string arg. Assumes arg is not a NULL pointer. */
addr_method operandMethod(char* arg, line_info* instruction, bool isDst) {
    //TODO; verify id immidiate / direct/  register?
    if (isRegister(arg, instruction, isDst))
         return REG_DIRECT;
    else if (isImmediate(arg, instruction, isDst))
        return IMMEDIATE;
    else if (isDirect(arg, instruction, isDst))
        return DIRECT;
    return -1; /* Never reaches here. */
}

/* The function returns how many additional words are needed according to the addressing methods of source/target. */
int howManyWords(addr_method sourceAddr, addr_method targetAddr) {
    if (sourceAddr == REG_DIRECT && targetAddr == REG_DIRECT)
        return ZERO_WORDS;
    else if ((sourceAddr == IMMEDIATE && targetAddr == REG_DIRECT) || (sourceAddr == REG_DIRECT && targetAddr == IMMEDIATE)) {
        return ONE_WORD;
    } else if (sourceAddr == IMMEDIATE && targetAddr == IMMEDIATE) {
        return TWO_WORDS_IMMEDIATE;
    } else if ((sourceAddr == REG_DIRECT && (targetAddr == DIRECT || targetAddr == JMP_M)) || (targetAddr == REG_DIRECT && (sourceAddr == DIRECT || sourceAddr == JMP_M))) {
        return TWO_WORDS;
    } else if ((sourceAddr == IMMEDIATE && (targetAddr == DIRECT || targetAddr == JMP_M)) || (targetAddr == IMMEDIATE && (sourceAddr == DIRECT || sourceAddr == JMP_M))) {
        return THREE_WORDS;
    } else if ((sourceAddr == DIRECT || sourceAddr == JMP_M) && (targetAddr == DIRECT || targetAddr == JMP_M)) {
        return FOUR_WORDS;
    }
    return -1; /* Should never reach here. */
}