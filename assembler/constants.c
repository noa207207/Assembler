#include "constants.h"
#include <stdlib.h>
#include <stdio.h>
#include "data_structure.h"
#include "utils.h"


struct base_instruction{
    unsigned int param_1;
    unsigned int param_2;
    unsigned int opcode;
    unsigned int src_addr;
    unsigned int dst_addr;
    unsigned int era;
};

struct single_data{
    unsigned int value;
};

struct immidiate_instruction{
    unsigned int operand;
    unsigned int era;
};

struct direct_instruction{
    unsigned int memory_address;
    char label[MAX_LABEL_LENGTH];
    unsigned int era;
};

struct register_instruction{
    unsigned int src_register;
    unsigned int dst_register;
    unsigned int era;
};

struct opcode_bin{
    unsigned int opcode : 16;
    unsigned int attribute : 4;
};

struct line_info{
    opcode opcode;

    addr_method src_addr;
    addr_method dst_addr;

    int src_reg;
    int dst_reg;
    int dst_reg2;

    int src_imm;
    int dst_imm;

    char src_label[MAX_LABEL_LENGTH];
    char dst_label[MAX_LABEL_LENGTH];
    char jmp_label[MAX_LABEL_LENGTH];

    addr_method first_param;
    addr_method second_param;

};

unsigned int base_to_binary(base_instruction_ptr_t inst) {
    unsigned int binary = 0;
    binary |= (inst->era & 0x3) << 0; // set bits 0-1 to era[0-1]
    binary |= (inst->dst_addr & 0x3) << 2; // set bits 2-3 to dst_addr[2-3]
    binary |= (inst->src_addr & 0x3) << 4; // set bits 4-5 to src_addr[4-5]
    binary |= (inst->opcode & 0xF) << 6; // set bits 6-9 to opcode[6-9]
    binary |= (inst->param_2 & 0x3) << 10; // set bits 10-11 to param2[10-11]
    binary |= (inst->param_1 & 0x3) << 12; // set bits 12-13 to param1[12-13]
    return binary;
}

unsigned int immidiate_to_binary(immidiate_instruction_ptr_t inst) {
    unsigned int binary = 0;
    binary |= (inst->operand & 0xFFF) << 2; // set bits 2-13 to operand[2-13]
    binary |= (inst->era & 0x3) << 0; // set bits 0-1 to era[0-1]
    return binary;
}

unsigned int direct_to_binary(direct_instruction_ptr_t inst) {
    unsigned int binary = 0;
    binary |= (inst->memory_address & 0xFFF) << 2; // set bits 2-12 to memory_address[2-12]
    binary |= (inst->era & 0x3) << 0; // set bits 0-1 to era[0-1]
    return binary;
}

unsigned int register_to_binary(register_instruction_ptr_t inst) {
    unsigned int binary = 0;
    binary |= (inst->src_register & 0x3F) << 8; // set bits 8-13 to src_register[8-13]
    binary |= (inst->dst_register & 0x3F) << 2; // set bits 2-7 to dst_register[2-7]
    binary |= (inst->era & 0x3) << 0; // set bits 0-1 to era[0-1]
    return binary;
}

void binary_to_pattern(unsigned int binary, char* pattern) {
    // initialize pattern with 14 '.' characters
    memset(pattern, '.', 14);
    pattern[14] = '\0'; // null-terminate the string

    // convert binary to a 14-bit binary string
    char binary_str[15];
    binary_str[14] = '\0'; // null-terminate the string
    for (int i = 0; i < 14; i++) {
        binary_str[i] = ((binary >> (13 - i)) & 0x1) ? '/' : '.';
    }

    // copy the 14-bit binary string to the pattern string
    strncpy(pattern, binary_str, 14);
}

char *get_direct_instruction_label(direct_instruction_ptr_t direct_ptr)
{
    return direct_ptr->label;
}

void set_direct_instruction_value(direct_instruction_ptr_t direct_ptr, int value)
{
    direct_ptr->memory_address = value;
}

void set_direct_instruction_era(direct_instruction_ptr_t direct_ptr, attributes era)
{
    direct_ptr->era = era;
}

// Getters
opcode get_opcode(line_info_ptr_t line_info_ptr) {
    return line_info_ptr->opcode;
}

addr_method get_src_addr(line_info_ptr_t line_info_ptr) {
    return line_info_ptr->src_addr;
}

addr_method get_dst_addr(line_info_ptr_t line_info_ptr) {
    return line_info_ptr->dst_addr;
}

int get_src_reg(line_info_ptr_t line_info_ptr) {
    return line_info_ptr->src_reg;
}

int get_dst_reg(line_info_ptr_t line_info_ptr) {
    return line_info_ptr->dst_reg2;
}

int get_src_imm(line_info_ptr_t line_info_ptr) {
    return line_info_ptr->src_imm;
}

int get_dst_imm(line_info_ptr_t line_info_ptr) {
    return line_info_ptr->dst_imm;
}

char* get_src_label(line_info_ptr_t line_info_ptr) {
    return line_info_ptr->src_label;
}

char* get_jmp_label(line_info_ptr_t line_info_ptr) {
    return line_info_ptr->jmp_label;
}

char* get_dst_label(line_info_ptr_t line_info_ptr) {
    return line_info_ptr->dst_label;
}

addr_method get_first_param(line_info_ptr_t line_info_ptr) {
    return line_info_ptr->first_param;
}

addr_method get_second_param(line_info_ptr_t line_info_ptr) {
    return line_info_ptr->second_param;
}

// Setters
void set_opcode(line_info_ptr_t line_info_ptr, opcode opc) {
    line_info_ptr->opcode = opc;
}

void set_src_addr(line_info_ptr_t line_info_ptr, addr_method addr) {
    line_info_ptr->src_addr = addr;
}

void set_dst_addr(line_info_ptr_t line_info_ptr, addr_method addr) {
    line_info_ptr->dst_addr = addr;
}

void set_src_reg(line_info_ptr_t line_info_ptr, int reg) {
    line_info_ptr->src_reg = reg;
}

void set_dst_reg(line_info_ptr_t line_info_ptr, int reg) {
    line_info_ptr->dst_reg2 = reg;
}

void set_src_imm(line_info_ptr_t line_info_ptr, int imm) {
    line_info_ptr->src_imm = imm;
}

void set_dst_imm(line_info_ptr_t line_info_ptr, int imm) {
    line_info_ptr->dst_imm = imm;
}

void set_src_label(line_info_ptr_t line_info_ptr, char* label, int size) {
    strncpy(line_info_ptr->src_label, label, size);
}

void set_jmp_label(line_info_ptr_t line_info_ptr, char* label, int size) {
    strncpy(line_info_ptr->jmp_label, label, size);
}

void set_dst_label(line_info_ptr_t line_info_ptr, char* label, int size) {
    strncpy(line_info_ptr->dst_label, label, size);
}

void set_first_param(line_info_ptr_t line_info_ptr, addr_method addr) {
    line_info_ptr->first_param = addr;
}

void set_second_param(line_info_ptr_t line_info_ptr, addr_method addr) {
    line_info_ptr->second_param = addr;
}

int get_data_value(single_data_ptr_t data)
{
    return data->value;
}

base_instruction_ptr_t base_instruction_init(unsigned int param_1, unsigned int param_2, unsigned int opcode, unsigned int src_addr, unsigned int dst_addr, unsigned int era) {
    base_instruction_ptr_t inst = (base_instruction_ptr_t)malloc_with_monitor(sizeof(struct base_instruction));
    if (inst != NULL) {
        inst->param_1 = param_1;
        inst->param_2 = param_2;
        inst->opcode = opcode;
        inst->src_addr = src_addr;
        inst->dst_addr = dst_addr;
        inst->era = era;
    }
    return inst;
}

single_data_ptr_t single_data_init(unsigned int value) {
    single_data_ptr_t data = (single_data_ptr_t)malloc_with_monitor(sizeof(struct single_data));
    if (data != NULL) {
        data->value = value;
    }
    return data;
}

immidiate_instruction_ptr_t immidiate_instruction_init(unsigned int operand, unsigned int era) {
    immidiate_instruction_ptr_t inst = (immidiate_instruction_ptr_t)malloc_with_monitor(sizeof(struct immidiate_instruction));
    if (inst != NULL) {
        inst->operand = operand;
        inst->era = era;
    }
    return inst;
}

direct_instruction_ptr_t direct_instruction_init(unsigned int memory_address, char* label, unsigned int era) {
    direct_instruction_ptr_t inst = (direct_instruction_ptr_t)malloc_with_monitor(sizeof(struct direct_instruction));
    if (inst != NULL) {
        inst->memory_address = memory_address;
        strncpy(inst->label, label, MAX_LABEL_LENGTH);
        inst->era = era;
    }
    return inst;
}

register_instruction_ptr_t register_instruction_init(unsigned int src_register, unsigned int dst_register, unsigned int era) {
    register_instruction_ptr_t inst = (register_instruction_ptr_t)malloc_with_monitor(sizeof(struct register_instruction));
    if (inst != NULL) {
        inst->src_register = src_register;
        inst->dst_register = dst_register;
        inst->era = era;
    }
    return inst;
}

opcode_bin_ptr_t opcode_bin_init(unsigned int opcode, unsigned int attribute) {
    opcode_bin_ptr_t bin = (opcode_bin_ptr_t)malloc_with_monitor(sizeof(struct opcode_bin));
    if (bin != NULL) {
        bin->opcode = opcode;
        bin->attribute = attribute;
    }
    return bin;
}

binary_ptr_t binary_init(addr_method type, void* ptr) {
    binary_ptr_t bin = (binary_ptr_t)malloc_with_monitor(sizeof(union binary));
    if (bin != NULL) {
        switch (type) {
            case BASE:
                bin->base_ptr = (base_instruction_ptr_t)ptr;
                break;
            case IMMEDIATE:
                bin->immidiate_ptr = (immidiate_instruction_ptr_t)ptr;
                break;
            case DIRECT:
                bin->direct_ptr = (direct_instruction_ptr_t)ptr;
                break;
            case REG_DIRECT:
                bin->register_ptr = (register_instruction_ptr_t)ptr;
                break;
            case SYNGEL_DATA:
                bin->data_ptr = (single_data_ptr_t)ptr;
                break;
            default:
                free(bin);
                return NULL;
        }
    }
    return bin;
}

line_info_ptr_t line_info_init(opcode opcode_val, addr_method src_addr_val, addr_method dst_addr_val, int src_reg_val, int dst_reg_val, int src_imm_val, int dst_imm_val, char* src_label_val, char* dst_label_val, char* jmp_label_val, addr_method first_param_val, addr_method second_param_val) {
    line_info_ptr_t tmp = (line_info_ptr_t)malloc_with_monitor(sizeof(struct line_info));
    tmp->opcode = opcode_val;
    tmp->src_addr = src_addr_val;
    tmp->dst_addr = dst_addr_val;
    tmp->src_reg = src_reg_val;
    tmp->dst_reg2 = dst_reg_val;
    tmp->src_imm = src_imm_val;
    tmp->dst_imm = dst_imm_val;
    strncpy(tmp->src_label, src_label_val, MAX_LABEL_LENGTH);
    strncpy(tmp->dst_label, dst_label_val, MAX_LABEL_LENGTH);
    strncpy(tmp->jmp_label, jmp_label_val, MAX_LABEL_LENGTH);
    tmp->first_param = first_param_val;
    tmp->second_param = second_param_val;
    return tmp;
}

line_info_ptr_t line_info_empty_init(void) {
    line_info_ptr_t tmp = (line_info_ptr_t)malloc_with_monitor(sizeof(struct line_info));
    if (tmp != NULL) {
        tmp->opcode = 0;
        tmp->src_addr = 0;
        tmp->dst_addr = 0;
        tmp->src_reg = 0;
        tmp->dst_reg2 = 0;
        tmp->src_imm = 0;
        tmp->dst_imm = 0;
        memset(tmp->src_label, 0, MAX_LABEL_LENGTH);
        memset(tmp->dst_label, 0, MAX_LABEL_LENGTH);
        memset(tmp->jmp_label, 0, MAX_LABEL_LENGTH);
        tmp->first_param = 0;
        tmp->second_param = 0;
    }
    return tmp;
}

void print_base_instruction(base_instruction_ptr_t instr) {
    printf("param_1: %u, param_2: %u, opcode: %u, src_addr: %u, dst_addr: %u, era: %u\n",
          instr->param_1, instr->param_2, instr->opcode, instr->src_addr, instr->dst_addr, instr->era);
}

void print_immidiate_instruction(immidiate_instruction_ptr_t instr) {
    printf("operand: %u, era: %u\n", instr->operand, instr->era);
}

void print_direct_instruction(direct_instruction_ptr_t instr) {
    printf("memory_address: %u, label: %s, era: %u\n",
          instr->memory_address, instr->label, instr->era);
}

void print_register_instruction(register_instruction_ptr_t instr) {
    printf("src_register: %u, dst_register: %u, era: %u\n",
          instr->src_register, instr->dst_register, instr->era);
}

void print_single_data(single_data_ptr_t data) {
    printf("single_data - value: %u\n", data->value);
}

int switch_and_insert(head_ptr_t arr, line_info_ptr_t instruction, int inst_count, addr_method address_method, bool is_dst)
{
    if (!is_dst) {
        switch (address_method)
        {
            case REG_DIRECT:
            insert_register_instruction(arr, instruction->src_reg, 0, A, inst_count++);
            break;

            case IMMEDIATE:
            insert_immidiate_instruction(arr,instruction->src_imm, A, inst_count++);
            break;

            case DIRECT:
            insert_direct_instruction(arr, instruction->src_label, 0, A, inst_count++, address_method);
            break;

        default:
            break;
        }

    } else {
            switch (address_method)
        {
            case REG_DIRECT:
            insert_register_instruction(arr, 0, instruction->dst_reg2, A, inst_count++);
            break;
                
            case IMMEDIATE:
            insert_immidiate_instruction(arr,instruction->dst_imm, A, inst_count++);
            break;

            case DIRECT:
            insert_direct_instruction(arr, instruction->dst_label, 0, A, inst_count++, address_method);
            break;

        default:
            break;
        }
    }

    return inst_count;
}



char *opcode_to_str(opcode op)
{
    switch (op)
    {
    case MOV:
    	return "mov"; break;
    case CMP:
		return "cmp"; break;
    case ADD:
		return "add"; break;
    case SUB:
		return "sub"; break;
    case NOT:
		return "not"; break;
    case CLR:
		return "clr"; break;
    case LEA:
		return "lea"; break;
    case INC:
		return "inc"; break;
    case DEC:
		return "dec"; break;
    case JMP:
		return "jmp"; break;
    case BNE:
		return "bne"; break;
    case RED:
		return "red"; break;
    case PRN:
		return "prn"; break;
    case JSR:
		return "jsr"; break;
    case RTS:
		return "rts"; break;
    case STOP:
		return "stop"; break;
    case DATA:
		return ".data"; break;
    case STRING:
		return ".string"; break;
    case ENTRY:
		return ".entry"; break;
    case EXTERNAL:
		return ".extern"; break;
    case OPCODE_SIZE:
		return "unknown opcode"; break;
    default:
        return "unknown opcode"; break;
    }
}

    bool opcode_in_group(opcode op, opcode group[], int count)
    {
		int i;
		for (i = 0; i < count; i++) {
			if (op == group[i]){
        return True;
      }
				
		}
		return False;
    }

