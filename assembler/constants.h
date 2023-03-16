#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "string.h"

#define MAX_LINE_LENGTH 81
#define MAX_LABEL_LENGTH 31
#define MAX_ARRTIBUTE_LENGTH 15
#define INST_AND_DIR_NUM 20

#define MAX_REGISTERS 8
#define MIN_INDEX_REG 10


/* Base instruction line */
#define BASE_INSTRUCTION_PARAM_1_MASK   0x3000
#define BASE_INSTRUCTION_PARAM_1_SHIFT  13

#define BASE_INSTRUCTION_PARAM_2_MASK   0xC00
#define BASE_INSTRUCTION_PARAM_2_SHIFT  11

#define BASE_INSTRUCTION_OPCODE_MASK    0x3C0
#define BASE_INSTRUCTION_OPCODE_SHIFT   9

#define BASE_INSTRUCTION_SRC_ADDR_MASK  0x30
#define BASE_INSTRUCTION_SRC_ADDR_SHIFT 5

#define BASE_INSTRUCTION_DST_ADDR_MASK  0xC
#define BASE_INSTRUCTION_DST_ADDR_SHIFT 3

#define BASE_INSTRUCTION_ERA_MASK  0x3
#define BASE_INSTRUCTION_ERA_SHIFT 0

/* Immidiate instruction line */
#define IMMIDIATE_INSTRUCTION_OPERAND_MASK   0x3FFC
#define IMMIDIATE_INSTRUCTION_OPERAND_SHIFT  13

#define IMMIDIATE_INSTRUCTION_ERA_MASK  0x3
#define IMMIDIATE_INSTRUCTION_ERA_SHIFT 0

/* Direct instruction line */
#define DIRECT_INSTRUCTION_ADDRESS_MASK   0x3FFC
#define DIRECT_INSTRUCTION_ADDRESS_SHIFT  13

#define DIRECT_INSTRUCTION_ERA_MASK  0x3
#define DIRECT_INSTRUCTION_ERA_SHIFT 0

/* Register instruction line */
#define REGISTER_INSTRUCTION_SRC_MASK   0x3F00
#define REGISTER_INSTRUCTION_SRC_SHIFT  13

#define DIRECT_INSTRUCTION_DST_MASK  0xFC
#define DIRECT_INSTRUCTION_DST_SHIFT 7

#define DIRECT_INSTRUCTION_ERA_MASK  0x3
#define DIRECT_INSTRUCTION_ERA_SHIFT 0

typedef enum{
    False,
    True
} bool;

typedef enum {
    MOV,
    CMP,
    ADD,
    SUB,
    NOT,
    CLR,
    LEA,
    INC,
    DEC,
    JMP,
    BNE,
    RED,
    PRN,
    JSR,
    RTS,
    STOP,
    DATA,
    STRING,
    ENTRY,
    EXTERNAL,
    OPCODE_SIZE
} opcode;

typedef enum{
    IMMEDIATE,
    DIRECT,
    JMP_PARAM, 
    REG_DIRECT,
    BASE,
    SYNGEL_DATA,
    EMPTY
} addr_method;

enum attributes {
    A = 0,
    E = 1,
    R = 2
};

extern opcode first_group[];
extern opcode second_group[];
extern opcode third_group[];
extern opcode jmp_group[];

char *opcode_to_str(opcode op);
bool opcode_in_group(opcode op, opcode group[], int count);

#endif
