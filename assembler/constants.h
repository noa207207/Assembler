#ifndef CONSTANTS_H
#define CONSTANTS_H

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

typedef enum {
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
    OPCODE_SIZE,
} opcode;

typedef enum {
    IMMEDIATE,
    DIRECT,
    JMP_PARAM, 
    REG_DIRECT,
    BASE,
    SYNGEL_DATA,
    EMPTY
} addr_method;

typedef struct {
    unsigned int param_1;
    unsigned int param_2;
    unsigned int opcode;
    unsigned int src_addr;
    unsigned int dst_addr;
    unsigned int era;
} base_instruction;

typedef struct {
    unsigned int value;
} single_data;

typedef struct {
    unsigned int operand;
    unsigned int era;
} immidiate_instruction;

typedef struct {
    unsigned int memory_address;
    char label[MAX_LABEL_LENGTH];
    unsigned int era;
} direct_instruction;

typedef struct {
    unsigned int src_register;
    unsigned int dst_register;
    unsigned int era;
} register_instruction;

typedef struct {
    unsigned int opcode : 16;
    unsigned int attribute : 4;
} opcode_bin;

typedef union binary {
    base_instruction *base_ptr;
    immidiate_instruction *immidiate_ptr;
    direct_instruction *direct_ptr;
    register_instruction *register_ptr;
    single_data *data_ptr;
} binary;

enum attributes {
    A = 0,
    E = 1,
    R = 3
};

enum decodeStatus {
    DONE,
    HIST
};

typedef enum opcodes {
    MOV_OP = 0,
    CMP_OP = 1,

    ADD_OP = 2,
    SUB_OP = 2,

    LEA_OP = 4,

    CLR_OP = 5,
    NOT_OP = 5,
    INC_OP = 5,
    DEC_OP = 5,

    JMP_OP = 9,
    BNE_OP = 9,
    JSR_OP = 9,

    RED_OP = 12,
    PRN_OP = 13,

    RTS_OP = 14,
    STOP_OP = 15
} opcodes;

typedef enum {
    ADD_FUNCT = 10,
    SUB_FUNCT = 11,

    CLR_FUNCT = 10,
    NOT_FUNCT = 11,
    INC_FUNCT = 12,
    DEC_FUNCT = 13,

    JMP_FUNCT = 10,
    BNE_FUNCT = 11,
    JSR_FUNCT = 12
} funct;

typedef struct {
    opcode opcode;

    addr_method src_addr;
    addr_method dst_addr;

    int src_reg;
    int dst_reg;

    int src_imm;
    int dst_imm;

    char src_label[MAX_LABEL_LENGTH];
    char dst_label[MAX_LABEL_LENGTH];
    char tmp_label[MAX_LABEL_LENGTH];

    addr_method first_param;
    addr_method second_param;

} line_info;

extern opcode first_group[];
extern opcode second_group[];
extern opcode third_group[];
extern opcode jmp_group[];

char *opcode_to_str(opcode op);
bool opcode_in_group(opcode op, opcode group[], int count);

#endif
