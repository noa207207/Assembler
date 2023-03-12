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

    int src_imm;
    int dst_imm;

    char src_label[MAX_LABEL_LENGTH];
    char dst_label[MAX_LABEL_LENGTH];
    char jmp_label[MAX_LABEL_LENGTH];

    addr_method first_param;
    addr_method second_param;

};

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
    return line_info_ptr->dst_reg;
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
    line_info_ptr->dst_reg = reg;
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
      printf("size = %d, pointer = %s, arg = %s\n", size, line_info_ptr->jmp_label, label);

    strncpy(line_info_ptr->jmp_label, label, size);
          printf("size = %d, pointer = %s, arg = %s\n", size, line_info_ptr->jmp_label, label);

}

void set_dst_label(line_info_ptr_t line_info_ptr, char* label, int size) {
    printf("size = %d, pointer = %s, arg = %s\n", size, line_info_ptr->dst_label, label);
    strncpy(line_info_ptr->dst_label, label, size);
    printf("size = %d, pointer = %s, arg = %s\n", size, line_info_ptr->dst_label, label);

}

void set_first_param(line_info_ptr_t line_info_ptr, addr_method addr) {
    line_info_ptr->first_param = addr;
}

void set_second_param(line_info_ptr_t line_info_ptr, addr_method addr) {
    line_info_ptr->second_param = addr;
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
    tmp->dst_reg = dst_reg_val;
    tmp->src_imm = src_imm_val;
    tmp->dst_imm = dst_imm_val;
    strncpy(tmp->src_label, src_label_val, MAX_LABEL_LENGTH);
    strncpy(tmp->dst_label, dst_label_val, MAX_LABEL_LENGTH);
    strncpy(tmp->jmp_label, jmp_label_val, MAX_LABEL_LENGTH);
    tmp->first_param = first_param_val;
    tmp->second_param = second_param_val;
    return tmp;
}

line_info_ptr_t line_info_empty_init() {
    line_info_ptr_t tmp = (line_info_ptr_t)malloc_with_monitor(sizeof(struct line_info));
    tmp->opcode = 0;
    tmp->src_addr = 0;
    tmp->dst_addr = 0;
    tmp->src_reg = 0;
    tmp->dst_reg = 0;
    tmp->src_imm = 0;
    tmp->dst_imm = 0;
    memset(tmp->src_label, 0, MAX_LABEL_LENGTH);
    memset(tmp->dst_label, 0, MAX_LABEL_LENGTH);
    memset(tmp->jmp_label, 0, MAX_LABEL_LENGTH);
    tmp->first_param = 0;
    tmp->second_param = 0;
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
            insert_direct_instruction(arr, instruction->src_label, 0, A, inst_count++, False);
            break;

        default:
            break;
        }

        return inst_count;
    }

    switch (address_method)
    {
        case REG_DIRECT:
        insert_register_instruction(arr, 0, instruction->dst_reg, A, inst_count++);
        break;
            
        case IMMEDIATE:
        insert_immidiate_instruction(arr,instruction->dst_imm, A, inst_count++);
        break;

        case DIRECT:
        insert_direct_instruction(arr, instruction->dst_label, 0, A, inst_count++, False);
        break;

    default:
        break;
    }
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
			if (op == group[i])
				return True;
		}
		return False;
    }

