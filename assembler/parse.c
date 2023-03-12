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
opcode second_group[] = {NOT, CLR, INC, DEC, RED, PRN};
opcode third_group[] = {RTS, STOP};
opcode jmp_group[] = {JMP, BNE, JSR};

/* This function parses a data line, and add it into the Data Image table. It returns the updated data-count. It assumes no errors in line. */
int parse_data_line(head_ptr_t headPtr, char* line, int data_count, opcode op) {
    int i, idx, line_num = data_count, arrayLength;
    // printf("opppppp = %d\n", op);
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
        // printf("array len = %d lune  = %s\n", arrayLength, line);
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
    char *token = strtok(line ,",");
    // printf("token = %s\n", token);
    char *temp = token;
    while (temp && *temp && (temp = strchr(temp, ",") != NULL) ) {
        //  printf("temp = %s\n", temp);
        ++i;
    }
    return i;
}

/* Parses an instruction line, returns the updated instruction-count. */
int parse_inst_line(head_ptr_t headPtr, char* original_line, char* line, char* line_copy, int inst_count, opcode op, bool* errorsFound, int line_num) {
    addr_method sourceAddr, targetAddr;
    char *token, *first_word;
    char label[MAX_LABEL_LENGTH];
    int immediate, additionalWords;
    static line_info_ptr_t instruction, first_param_p, second_param_p;

    char *tmp, *tmp2;

    instruction = line_info_empty_init();
    first_param_p = line_info_empty_init();
    second_param_p = line_info_empty_init();

    // printf("inst2 = %d\n", inst_count);

    // printf("target = %d\n", (int)get_dst_addr(instruction));

    // printf("GGGGGGGGet = %d\n", get_dst_reg(second_param_p));

    // printf("dayyyy: %s, %s, %s\n", get_dst_label(instruction), get_src_label(instruction), get_jmp_label(instruction));

    //line_info_ptr_t *first_param_ptr = &first_param_p, *second_param_ptr = &second_param_p;

    // printf("%s: line = %s, line in num = %d, num_op = %d\n", __func__, line, *line);

    set_opcode(instruction, op);
    printf("target = %d\n", get_dst_addr(instruction));
    //  printf("opcode = %d\n",
        // get_opcode(instruction));
    if (opcode_in_group(op, third_group, 2)) {
        
        if (errors_zero_operands_inst(original_line, line, line_num, op)) {
            *errorsFound = True;
            return inst_count;
    }
        insert_base_instruction(headPtr, get_opcode(instruction), 0, 0, A, inst_count++, 0, 0);
        return inst_count;
    }
    // printf("inst33 = %d\n", inst_count);

    if (opcode_in_group(op, second_group, 6)) { /* If the operation requires one operand */

        // printf("HHHHHHHHERE\n");
        if (errors_one_operand_inst(original_line, line, line_num, &instruction)) {
            *errorsFound = True;
            return inst_count;
        }

        targetAddr = operandMethod(line, &instruction, True, NULL, NULL);

        if (targetAddr == DIRECT) {
            //TODO: to check if neccasary tp  ckeck label is valid 
            // contain a-z & 1-9
        }
    
        insert_base_instruction(headPtr, get_opcode(instruction), 0, targetAddr, A, inst_count++, 0, 0);
        inst_count = switch_and_insert(headPtr, instruction, inst_count, targetAddr, True);
    
        return inst_count;
    }

    if (opcode_in_group(op, jmp_group, 3)) {
    
        printf("AAAfirst = src = %d, dst = %d, second: first = %d, decond = %d\n",
        get_src_reg(first_param_p), get_dst_reg(first_param_p),
        get_src_reg(second_param_p), get_dst_reg(second_param_p));  
        
        targetAddr = operandMethod(line, &instruction, True, &first_param_p, &second_param_p);
        //  if (error_jmp_group(original_line, line_num)) {
        //     *errorsFound = True;
        //     return inst_count;
        // }

        if (targetAddr != JMP_PARAM)
            return inst_count;
        // printf(":))))))\n");
        // printf("opcode = %d, targetAddr = %d\n",
        //, get_first_param = %d, get_second_param = %d\n",
        //get_opcode(instruction), targetAddr);
        //, get_first_param(instruction), get_second_param(instruction));
        insert_base_instruction(headPtr, get_opcode(instruction), 0, targetAddr, A, inst_count++, get_first_param(instruction), get_second_param(instruction));
        // printf(":))))))\n");
        // insert_direct_instruction(headPtr, get_dst_label(instruction), 0,  A, inst_count++, True);
        // printf(":))))))\n");
        
        printf("first = src = %d, dst = %d, second: first = %d, decond = %d\n",
        get_src_reg(first_param_p), get_dst_reg(first_param_p),
        get_src_reg(second_param_p), get_dst_reg(second_param_p));
        if (get_first_param(instruction) == REG_DIRECT && get_second_param(instruction) == REG_DIRECT) {
            insert_register_instruction(headPtr, get_src_reg(first_param_p), get_dst_reg(second_param_p), A, inst_count++);
            return inst_count;
        }
        if (first_param_p)
            inst_count = switch_and_insert(headPtr, first_param_p, inst_count, get_first_param(instruction), False);
        if (second_param_p)
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
        printf("src = %d, target = %d\n", get_src_addr(instruction), get_dst_addr(instruction));
        if (!is_legal_lba(op, sourceAddr, targetAddr)){
            printf("**********************************\n");
            return inst_count;
        }

       // printf("original_line = %s, line copy = %s, firsty = %s, token = %s\n", original_line, line_copy, first_word, token);
        
        if (errors_two_operands_inst(original_line, line_copy, first_word, token, line_num, &instruction)) {
            *errorsFound = True;
            return inst_count;
        }

        insert_base_instruction(headPtr, get_opcode(instruction), sourceAddr, targetAddr, A, inst_count++, 0, 0);
        if (sourceAddr == REG_DIRECT && targetAddr == REG_DIRECT) {
            insert_register_instruction(headPtr, get_src_reg(instruction), get_dst_reg(instruction), A, inst_count++);
            return inst_count;
        }
        //  printf("source = %d\n", (int)sourceAddr);
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
    // printf("%s\n", __func__);
    int i = 0;
    if (arg[i++] == '#') {
        // printf("val = %d\n", atoi(arg + i));
        if (isDst) {
        set_dst_addr(instruction, IMMEDIATE);
        set_dst_imm(instruction, atoi(arg + i));
        printf("val = %d\n", get_dst_imm(instruction));

    } else {
        set_src_addr(instruction, IMMEDIATE);
        set_src_imm(instruction, atoi(arg + i));
    }
    return True;
    }
    // printf("here!!\n");
    return False;
}

/* The function returns True if the operand is of type Direct. It assumes so if it's not a register/index/immediate and does not check errors. It also updates the line_info pointer.*/
bool isDirect(char* arg, line_info_ptr_t instruction, bool isDst)
{
//    printf("direct, %s\n", arg);
    if (isDst) {
        set_dst_addr(instruction, DIRECT);
        set_dst_label(instruction, arg, MAX_LABEL_LENGTH);

    } else {
        set_src_addr(instruction, DIRECT);
        set_src_label(instruction, arg, MAX_LABEL_LENGTH);
    }
    // printf("direct\n");
    return True;
}

bool is_jmp_param(char* arg, line_info_ptr_t instruction, bool isDst, line_info_ptr_t *first_param_info, line_info_ptr_t *second_param_info) {
    int i, labelLength, reg;
    char *token, *line, *first;
    addr_method first_param_m, second_param_m;

    i = 0;

    // printf("pointerrrrrr = %p, %p\n", *first_param_info, *second_param_info);

    // printf("arg = %s\n", arg);
    if (isImmediate(arg, instruction, isDst) || isRegister(arg, instruction, isDst))
        return False;
    while (arg[i] != '(' && arg[i])
         i++;
    //  printf("i = %d\n", i);
    //  printf("%c\n", arg[i]);
     if (arg[i] == '(') {
         if (arg[strlen(arg) - 1] != ')')
             return False;
         printf("%c\n", arg[i]);
         labelLength = i;
         line = arg + labelLength + 1;
         first = token = strtok(line, ",");
        //  printf("first = %s\n", first);
        first_param_m = operandMethod(token, first_param_info, False, NULL, NULL);
        printf("BBBBBfirst = src = %d, dst = %d, second: first = %d, decond = %d\n",
        get_src_reg(*first_param_info), get_dst_reg(*first_param_info),
        get_src_reg(*second_param_info), get_dst_reg(*second_param_info));
        //    printf("ppppnnnnn = %d\n", get_src_reg(*first_param_info));

          token = strtok(NULL, ")");
          printf("token = %s\n", token);
         if (token != NULL)
             second_param_m = operandMethod(token, second_param_info, True, NULL, NULL);
             printf("CCCCCfirst = src = %d, dst = %d, second: first = %d, decond = %d\n",
        get_src_reg(*first_param_info), get_dst_reg(*first_param_info),
        get_src_reg(*second_param_info), get_dst_reg(*second_param_info));
        //   printf("%s: sec_param = %d\n", __func__, (int)second_param_m);

        //    printf("ppppnnnn = %d\n", get_dst_reg(*second_param_info));
        //    printf("ppppnnnnn = %d\n", get_src_reg(*first_param_info));

        set_first_param(instruction, first_param_m);
        set_second_param(instruction, second_param_m);

        // printf("pppp = %d\n", get_dst_reg(*second_param_info));
        // printf("pppp = %d\n", get_src_reg(*first_param_info));

        // printf("first p = %d, sec p = %d\n", get_first_param(instruction), get_second_param(instruction));

     } else {
         *first_param_info = NULL;
         *second_param_info = NULL;
     }
    // printf("p = %d\n", get_dst_reg(*second_param_info));
    // printf("p = %d\n", get_src_reg(*first_param_info));
    set_dst_addr(instruction, JMP_PARAM);
    set_dst_label(instruction, arg, i);
    // printf("p2 = %d\n", get_dst_reg(*second_param_info));
    // printf("p2 = %d\n", get_src_reg(*first_param_info));

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
        printf("atoi = %d\n", atoi(arg + i));
        if (isDst) {
            printf("dst\n");
            set_dst_addr(instruction, REG_DIRECT);
            printf("atoi = %d\n", atoi(arg + i));
            set_dst_reg(instruction, (int)atoi(arg + i));
            printf("atoi = %d\n", atoi(arg + i));
            
        } else {
            printf("src\n");
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
    //TODO; verify id immidiate / direct/  register?
    printf("%s: arg = %s\n", __func__, arg);
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
        return True;
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
        if (src_mtd != EMPTY || dst_mtd == EMPTY || dst_mtd == IMMEDIATE)
            err = False;
        break;
    case LEA:
        if ((src_mtd != DIRECT && src_mtd != JMP) || dst_mtd == IMMEDIATE)
            err = False;
        break;
    case PRN:
        if (src_mtd != EMPTY || dst_mtd == IMMEDIATE || dst_mtd == EMPTY)
            err = False;
    case RTS:
    case STOP:
        if (src_mtd != EMPTY || dst_mtd != EMPTY)
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