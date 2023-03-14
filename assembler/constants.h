#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "string.h"
#include "data_structure.h"

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

typedef struct head *head_ptr_t;


typedef struct base_instruction *base_instruction_ptr_t;
typedef struct single_data *single_data_ptr_t;
typedef struct immidiate_instruction *immidiate_instruction_ptr_t;
typedef struct direct_instruction *direct_instruction_ptr_t;
typedef struct register_instruction *register_instruction_ptr_t;
typedef struct opcode_bin *opcode_bin_ptr_t;
typedef struct line_info *line_info_ptr_t;



union binary {
    base_instruction_ptr_t base_ptr;
    immidiate_instruction_ptr_t immidiate_ptr;
    direct_instruction_ptr_t direct_ptr;
    register_instruction_ptr_t register_ptr;
    single_data_ptr_t data_ptr;
};
typedef union binary *binary_ptr_t;

typedef enum bool{
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

typedef enum addr_method{
    IMMEDIATE,
    DIRECT,
    JMP_PARAM, 
    REG_DIRECT,
    BASE,
    SYNGEL_DATA,
    EMPTY
} addr_method;

typedef enum attributes {
    A = 0,
    E = 1,
    R = 2
}attributes;

base_instruction_ptr_t base_instruction_init(unsigned int param_1, unsigned int param_2, unsigned int opcode, unsigned int src_addr, unsigned int dst_addr, unsigned int era);
immidiate_instruction_ptr_t immidiate_instruction_init(unsigned int operand, unsigned int era);
direct_instruction_ptr_t direct_instruction_init(unsigned int memory_address, char* label, unsigned int era);
register_instruction_ptr_t register_instruction_init(unsigned int src_register, unsigned int dst_register, unsigned int era);
opcode_bin_ptr_t opcode_bin_init(unsigned int opcode, unsigned int attribute);
single_data_ptr_t single_data_init(unsigned int value);
binary_ptr_t binary_init(addr_method type, void* ptr);
line_info_ptr_t line_info_init(opcode opcode_val, addr_method src_addr_val, addr_method dst_addr_val, 
    int src_reg_val, int dst_reg_val, int src_imm_val, int dst_imm_val, char* src_label_val, 
    char* dst_label_val, char* jmp_label_val, addr_method first_param_val, 
    addr_method second_param_val);
line_info_ptr_t line_info_empty_init(void);


void print_base_instruction(base_instruction_ptr_t base_ptr);
void print_immidiate_instruction(immidiate_instruction_ptr_t immidiate_ptr);
void print_direct_instruction(direct_instruction_ptr_t direct_ptr);
void print_register_instruction(register_instruction_ptr_t register_ptr);
void print_single_data(single_data_ptr_t data_ptr);
int switch_and_insert(head_ptr_t arr, line_info_ptr_t instruction, int inst_count, addr_method address_method, bool is_dst);

/* Getter functions */ 
opcode get_opcode(line_info_ptr_t line);
addr_method get_src_addr(line_info_ptr_t line);
addr_method get_dst_addr(line_info_ptr_t line);
int get_src_reg(line_info_ptr_t line);
int get_dst_reg(line_info_ptr_t line);
int get_src_imm(line_info_ptr_t line);
int get_dst_imm(line_info_ptr_t line);
char* get_src_label(line_info_ptr_t line);
char* get_jmp_label(line_info_ptr_t line);
char* get_dst_label(line_info_ptr_t line);
addr_method get_first_param(line_info_ptr_t line);
addr_method get_second_param(line_info_ptr_t line);
char *get_direct_instruction_label(direct_instruction_ptr_t direct_ptr);
void set_direct_instruction_value(direct_instruction_ptr_t direct_ptr, int value);
void set_direct_instruction_era(direct_instruction_ptr_t direct_ptr, attributes era);
int get_data_value(single_data_ptr_t data);

// Setter functions
void set_opcode(line_info_ptr_t line, opcode opcode);
void set_src_addr(line_info_ptr_t line, addr_method src_addr);
void set_dst_addr(line_info_ptr_t line, addr_method dst_addr);
void set_src_reg(line_info_ptr_t line, int src_reg);
void set_dst_reg(line_info_ptr_t line, int dst_reg);
void set_src_imm(line_info_ptr_t line, int src_imm);
void set_dst_imm(line_info_ptr_t line, int dst_imm);
void set_src_label(line_info_ptr_t line, char* src_label, int size);
void set_jmp_label(line_info_ptr_t line, char* jmp_label, int size);
void set_dst_label(line_info_ptr_t line, char* dst_label, int size);
void set_first_param(line_info_ptr_t line, addr_method first_param);
void set_second_param(line_info_ptr_t line, addr_method second_param);


extern opcode first_group[];
extern opcode second_group[];
extern opcode third_group[];
extern opcode jmp_group[];

char *opcode_to_str(opcode op);
bool opcode_in_group(opcode op, opcode group[], int count);

unsigned int base_to_binary(base_instruction_ptr_t inst);
unsigned int immidiate_to_binary(immidiate_instruction_ptr_t inst);
unsigned int direct_to_binary(direct_instruction_ptr_t inst);
unsigned int register_to_binary(register_instruction_ptr_t inst);
void binary_to_pattern(unsigned int binary, char* pattern);
unsigned int get_direct_instruction_value(direct_instruction_ptr_t direct_ptr);

#endif