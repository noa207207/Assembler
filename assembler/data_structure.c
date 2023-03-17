#include "data_structure.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "constants.h"

struct symbol{
    char symbol_name[MAX_LABEL_LENGTH];
    int value;
    bool is_external;
    bool is_code;
    bool is_data;
    bool is_entry;
};

struct image{
    int line;
    char label[MAX_LABEL_LENGTH];
    int to_decode;
    bool is_extern;
    binary_ptr_t bin;
    addr_method type;
};

struct head{
    symbol_ptr_t table;
    image_ptr_t data_image;
    image_ptr_t code_image;

    int tableUsed;
    int table_size;

    int dataUsed;
    int data_size;

    int codeUsed;
    int code_size;
};

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

head_ptr_t head_init(int table_size, int data_size, int code_size) {
    head_ptr_t tmp = (head_ptr_t)malloc_with_monitor(sizeof(struct head));
    tmp->table = (symbol_ptr_t)malloc_with_monitor(table_size * sizeof(struct symbol));
    tmp->data_image = (image_ptr_t)malloc_with_monitor(data_size * sizeof(struct image));
    tmp->code_image = (image_ptr_t)malloc_with_monitor(code_size * sizeof(struct image));
    tmp->tableUsed = tmp->dataUsed = tmp->codeUsed = 0;
    tmp->table_size = table_size;
    tmp->data_size = data_size;
    tmp->code_size = code_size;
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

/* Direct instruction Getter and Setter */

char *get_direct_label(head_ptr_t h, int idx)
{
    return get_direct_instruction_label(h->code_image[idx].bin->direct_ptr);
}

void set_direct_value(head_ptr_t h, int idx, int value)
{
    set_direct_instruction_value(h->code_image[idx].bin->direct_ptr, value);
}

void set_direct_era(head_ptr_t h, int idx, enum attributes attr)
{
    set_direct_instruction_era(h->code_image[idx].bin->direct_ptr, attr);
}

/* Head Getter and Setter */

image_ptr_t get_data_image(head_ptr_t h) {
    return h->data_image;
}

image_ptr_t get_code_image(head_ptr_t h) {
    return h->code_image;
}

int get_table_used(head_ptr_t h) {
    return h->tableUsed;
}

int get_table_size(head_ptr_t h) {
    return h->table_size;
}

int get_data_used(head_ptr_t h) {
    return h->dataUsed;
}

int get_code_used(head_ptr_t h) {
    return h->codeUsed;
}

int get_code_size(head_ptr_t h) {
    return h->code_size;
}

/* Getter and Setter for Symbol structure */

char* get_symbol_name(head_ptr_t head, int idx) {
    return head->table[idx].symbol_name;
}

int get_symbol_value(head_ptr_t head, int idx) {
    return head->table[idx].value;
}

bool get_symbol_is_external(head_ptr_t head, int idx) {
    return head->table[idx].is_external;
}

bool get_symbol_is_entry(head_ptr_t head, int idx) {
    return head->table[idx].is_entry;
}

void set_symbol_is_entry(head_ptr_t head, int idx, bool is_entry) {
    head->table[idx].is_entry = is_entry;
}

/* Getter and Setter for code_image */

int get_code_line(head_ptr_t head, int idx) {
    return head->code_image[idx].line;
}

int get_code_to_decode(head_ptr_t head, int idx) {
    return head->code_image[idx].to_decode;
}

bool get_code_is_extern(head_ptr_t head, int idx) {
    return head->code_image[idx].is_extern;
}

void set_code_is_extern(head_ptr_t head, int idx, bool is_extern) {
    head->code_image[idx].is_extern = is_extern;
}

int get_image_line(image_ptr_t img, int idx)
{
    return img[idx].line;
}

int get_single_data_value(image_ptr_t img, int idx)
{
    return get_data_value(get_bin_by_type(&img[idx]));
}

void *get_bin_by_type(image_ptr_t img)
{
    switch (img->type)
    {
    case BASE:
        return img->bin->base_ptr;
        break;
    case IMMEDIATE:
        return img->bin->immidiate_ptr;
        break;
    case DIRECT:
    case JMP_PARAM:
        return img->bin->direct_ptr;
        break;
    case REG_DIRECT:
        return img->bin->register_ptr;
        break;
    case SYNGEL_DATA:
        return img->bin->data_ptr;
    default:
        return NULL;
    }
}

/* SYMBOLS */

/* Initialization of symbol node. */
void symbol_init(symbol_ptr_t node) {
    node->is_external = False;
    node->is_code = False;
    node->is_data = False;
    node->is_entry = False;
}

void insert_symbol(head_ptr_t arr, char* name, int value, int op) {
    
    int idx = arr->tableUsed;

    if (is_duplicate_label(arr, name, strlen(name))){
         printf("Label %s is already exist\n", name);
    }

    if (arr->tableUsed == arr->table_size) {
        arr->table_size *= 2;
        arr->table = (symbol_ptr_t)realloc_with_monitor(arr->table, arr->table_size * sizeof(struct symbol));
    }

    strcpy(arr->table[idx].symbol_name, name);
    arr->table[idx].value = value;

    symbol_init(arr->table + idx);

    if (op == DATA || op == STRING)
        arr->table[idx].is_data = True;
    else if (op == ENTRY)
        arr->table[idx].is_entry = True;
    else if (op == EXTERNAL)
        arr->table[idx].is_external = True;
    else /* Assume it is fine */
        arr->table[idx].is_code = True;

    arr->tableUsed++;
}

void insert_data_symbol(head_ptr_t arr, char* name, int value, int op) {
    insert_symbol(arr, name, value, op);
}

void insert_extern(head_ptr_t arr, char* line, int op) {

    delete_spaces(line);
    insert_symbol(arr, line, 0, op);
}

void insert_code_symbol(head_ptr_t arr, char* name, int value, int op) {
    insert_symbol(arr, name, value, op);
}

void free_symbol_table(head_ptr_t arr) {
    free(arr->table);
    arr->table = NULL;
    arr->tableUsed = arr->table_size = 0;
}

/* DATA */

void insert_data_img(head_ptr_t arr, unsigned int data, int line) {

    int idx = arr->dataUsed;
    single_data_ptr_t data_ptr = single_data_init(data);
    binary_ptr_t bin_ptr = binary_init(SYNGEL_DATA, data_ptr);
    
    if (arr->dataUsed == arr->data_size) {
        arr->data_size *= 2;
        arr->data_image = (image_ptr_t)realloc_with_monitor(arr->data_image, arr->data_size * sizeof(struct image));
    }
    arr->data_image[idx].line = line;
    arr->data_image[idx].bin = bin_ptr;
    arr->data_image[idx].type = SYNGEL_DATA;
    arr->dataUsed++;
}

void update_data_count(head_ptr_t arr, int inst_count) {
    int idx;
    for (idx = 0; idx < arr->dataUsed; idx++)
        arr->data_image[idx].line += inst_count;

    /* Update symbol table and set symbol value. */
    for (idx = 0; idx < arr->tableUsed; idx++)
        if (arr->table[idx].is_data) {
            arr->table[idx].value += inst_count;
        }
}

void free_data_image(head_ptr_t arr) {
    free(arr->data_image);
    arr->data_image = NULL;
    arr->dataUsed = arr->data_size = 0;
}

/* CODE */

void resize_img_arr(head_ptr_t arr)
{
     if (arr->codeUsed == arr->code_size) {
        arr->code_size += 100;
        arr->code_image = (image_ptr_t)realloc_with_monitor(arr->code_image, arr->code_size * sizeof(struct image)); /* Add monitor */
    }
}

/* Handles insertion into code image when given an opcode. */
void insert_base_instruction(head_ptr_t arr, unsigned int opcode, unsigned int src_addr,
                                unsigned int dst_addr, int attribute, int line, unsigned int param1, unsigned int param2)
{
    int idx = arr->codeUsed;
    base_instruction_ptr_t base_inst_ptr = base_instruction_init(param1, param2, opcode, src_addr,dst_addr, attribute); 
    binary_ptr_t bin_ptr = binary_init(BASE, base_inst_ptr);
    resize_img_arr(arr);
    arr->code_image[idx].line = line;
    arr->code_image[idx].is_extern = False;
    arr->code_image[idx].bin = bin_ptr;
    arr->code_image[idx].to_decode = 0;
    arr->codeUsed++;
    arr->code_image[idx].type = BASE;
}

void insert_immidiate_instruction(head_ptr_t arr, unsigned int operand, int attribute, int line)
{
    int idx = arr->codeUsed;
    immidiate_instruction_ptr_t imm_inst_ptr = immidiate_instruction_init(operand, attribute);
    binary_ptr_t bin_ptr = binary_init(IMMEDIATE, imm_inst_ptr);

    resize_img_arr(arr);

    arr->code_image[idx].line = line;
    arr->code_image[idx].is_extern = False;
    arr->code_image[idx].type = IMMEDIATE;
    arr->code_image[idx].bin = bin_ptr;
    arr->code_image[idx].to_decode = 0;
    arr->codeUsed++;
}

void insert_direct_instruction(head_ptr_t arr, char *label, unsigned int mem_address, int attribute, int line, addr_method method)
{
    int idx = arr->codeUsed;
    direct_instruction_ptr_t direct_inst_ptr = direct_instruction_init(mem_address, label, attribute);
    binary_ptr_t bin_ptr = binary_init(DIRECT, direct_inst_ptr);

    resize_img_arr(arr);
    arr->code_image[idx].line = line;
    arr->code_image[idx].is_extern = False;
    arr->code_image[idx].bin = bin_ptr;
    arr->code_image[idx].to_decode = 1;
    arr->code_image[idx].type = method;
    arr->codeUsed++;
}

void insert_register_instruction(head_ptr_t arr, unsigned int src_register, unsigned int dst_register, int attribute, int line)
{
    int idx = arr->codeUsed;
    register_instruction_ptr_t register_inst_ptr = register_instruction_init(src_register, dst_register, attribute);
    binary_ptr_t bin_ptr = binary_init(REG_DIRECT, register_inst_ptr);
    
    resize_img_arr(arr);

    arr->code_image[idx].line = line;
    arr->code_image[idx].is_extern = False;
    arr->code_image[idx].bin = bin_ptr;
    arr->code_image[idx].to_decode = 0;
    arr->code_image[idx].type = REG_DIRECT;
    arr->codeUsed++;
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

void free_code_image(head_ptr_t arr) {
    free(arr->code_image);
    arr->code_image = NULL;
    arr->codeUsed = arr->code_size = 0;
}

void free_head(head_ptr_t arr) {
    free_symbol_table(arr);
    free_data_image(arr);
    free_code_image(arr);
}

bool is_duplicate_label(head_ptr_t headPtr, char* line, int length) {
    int i;
    for (i = 0; i < headPtr->tableUsed; i++)
        if (strlen(headPtr->table[i].symbol_name) == length && !strncmp(line, headPtr->table[i].symbol_name, length))
            return True;

    return False;
}

/* Direct instruction getter and setter */

char *get_direct_instruction_label(direct_instruction_ptr_t direct_ptr)
{
    return direct_ptr->label;
}

void set_direct_instruction_value(direct_instruction_ptr_t direct_ptr, int value)
{
    direct_ptr->memory_address = value;
}

void set_direct_instruction_era(direct_instruction_ptr_t direct_ptr, enum attributes era)
{
    direct_ptr->era = era;
}

/* Line info Getter */

int get_opcode(line_info_ptr_t line_info_ptr) {
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

char* get_dst_label(line_info_ptr_t line_info_ptr) {
    return line_info_ptr->dst_label;
}

addr_method get_first_param(line_info_ptr_t line_info_ptr) {
    return line_info_ptr->first_param;
}

addr_method get_second_param(line_info_ptr_t line_info_ptr) {
    return line_info_ptr->second_param;
}

/* Line info Setters */
void set_opcode(line_info_ptr_t line_info_ptr, int opc) {
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

/* Instructions Init*/

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

binary_ptr_t binary_init(addr_method type, void* ptr) {
    binary_ptr_t bin = (binary_ptr_t)malloc_with_monitor(sizeof(union binary));
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
    return bin;
}

/* Print Functions (Only for testing)*/

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

void print_head_code_bin(head_ptr_t arr)
{
    int size = arr->codeUsed;
    int i;
    binary_ptr_t bin;

    printf("Code Bin size = %d\n", size);
    for (i = 0; i < size; i++)
    {
        printf("[%d]: line = %d, is_extern = %d, to_decode = %d ", i, arr->code_image[i].line,
            arr->code_image[i].is_extern,
            arr->code_image[i].to_decode);

        bin = arr->code_image[i].bin;
        switch (arr->code_image[i].type)
        {
            case BASE:
                printf("BASE: ");
                print_base_instruction(bin->base_ptr);
                printf("\n");
                break;
            case REG_DIRECT:
                printf("REGISTER: ");
                print_register_instruction(bin->register_ptr);
                printf("\n");
                break;
            case IMMEDIATE:
                printf("IMMIDIATE: ");
                print_immidiate_instruction(bin->immidiate_ptr);
                printf("\n");
                break;
            case DIRECT:
                printf("DIRECT: ");
                print_direct_instruction(bin->direct_ptr);
                printf("\n");
                break;
            case JMP_PARAM:
                printf("JMP_PARAM: ");
                print_direct_instruction(bin->direct_ptr);
                printf("\n");
                break;
            default:
                printf("Unknown instruction type\n");
                break;
        }
    }
}

void print_symbols(head_ptr_t arr)
{
    int size = arr->tableUsed;
    int i;

        for (i = 0; i < size; i++)
        {

            printf("[%d]: s_name = %s, val = %d , ext = %d, code = %d, data  = %d,  entry = %d\n", 
            i,
            arr->table[i].symbol_name,
            arr->table[i].value,
            (int)arr->table[i].is_external,
            (int)arr->table[i].is_code,
            (int)arr->table[i].is_data,
            (int)arr->table[i].is_entry);              
        } 
}

void print_data(head_ptr_t arr)
{
    int size = arr->dataUsed;
    int i;

    printf("data size = %d\n", arr->data_size);

        for (i = 0; i < size; i++)
        {
            printf("[%d]: line = %d", 
            i,
            arr->data_image[i].line);
            print_single_data(arr->data_image[i].bin->data_ptr);
            printf("\n");          
        } 
}

/* Binary Encode */

void instructions_to_binary(image_ptr_t img, int num_instructions, char** binary_str) {
    base_instruction_ptr_t base_inst;
    immidiate_instruction_ptr_t imm_inst;
    direct_instruction_ptr_t dir_inst;
    register_instruction_ptr_t reg_inst;
    unsigned int binary;
    char pattern[15];
    char line_num[7];
    int i;

    *binary_str = malloc_with_monitor((num_instructions * 22) + 1);
    memset(*binary_str, 0, (num_instructions * 22) + 1); /* initialize binary_str with null bytes */
    for (i = 0; i < num_instructions; i++) {
        sprintf(line_num, "%04d  ", get_image_line(img, i));
        strcat(*binary_str, line_num);
        switch (img[i].type) {
            case BASE:
                base_inst = (base_instruction_ptr_t) img[i].bin->immidiate_ptr;
                binary = base_to_binary(base_inst);
                binary_to_pattern(binary, pattern);
                strcat(*binary_str, pattern);
                break;
            case IMMEDIATE:
                imm_inst = (immidiate_instruction_ptr_t) img[i].bin->immidiate_ptr;
                binary = immidiate_to_binary(imm_inst);
                binary_to_pattern(binary, pattern);
                strcat(*binary_str, pattern);
                break;
            case DIRECT:
            case JMP_PARAM:
                dir_inst = (direct_instruction_ptr_t) img[i].bin->direct_ptr;
                binary = direct_to_binary(dir_inst);
                binary_to_pattern(binary, pattern);
                strcat(*binary_str, pattern);
                break;
            case REG_DIRECT:
                reg_inst = (register_instruction_ptr_t) img[i].bin->register_ptr;
                binary = register_to_binary(reg_inst);
                binary_to_pattern(binary, pattern);
                strcat(*binary_str, pattern);
                break;
            default:
                /* do nothing */
                break;
        }
        strcat(*binary_str, "\n");
    }
}

unsigned int immidiate_to_binary(immidiate_instruction_ptr_t inst) {
    unsigned int binary = 0;
    binary |= (inst->operand & IMMIDIATE_INSTRUCTION_OPERAND_MASK) << IMMIDIATE_INSTRUCTION_OPERAND_SHIFT;
    binary |= (inst->era & IMMIDIATE_INSTRUCTION_ERA_MASK) << IMMIDIATE_INSTRUCTION_ERA_SHIFT;
    return binary;
}

unsigned int direct_to_binary(direct_instruction_ptr_t inst) {
    unsigned int binary = 0;
    binary |= (inst->memory_address & DIRECT_INSTRUCTION_ADDRESS_MASK) << DIRECT_INSTRUCTION_ADDRESS_SHIFT;
    binary |= (inst->era & DIRECT_INSTRUCTION_ERA_MASK) << DIRECT_INSTRUCTION_ERA_SHIFT;
    return binary;
}

unsigned int base_to_binary(base_instruction_ptr_t inst) {
    unsigned int binary = 0;
    binary |= (inst->era & BASE_INSTRUCTION_ERA_MASK) << BASE_INSTRUCTION_ERA_SHIFT;
    binary |= (inst->dst_addr & BASE_INSTRUCTION_DST_ADDR_MASK) << BASE_INSTRUCTION_DST_ADDR_SHIFT;
    binary |= (inst->src_addr & BASE_INSTRUCTION_SRC_ADDR_MASK) << BASE_INSTRUCTION_SRC_ADDR_SHIFT;
    binary |= (inst->opcode & BASE_INSTRUCTION_OPCODE_MASK) << BASE_INSTRUCTION_OPCODE_SHIFT;
    binary |= (inst->param_2 & BASE_INSTRUCTION_PARAM_2_MASK) << BASE_INSTRUCTION_PARAM_2_SHIFT;
    binary |= (inst->param_1 & BASE_INSTRUCTION_PARAM_1_MASK) << BASE_INSTRUCTION_PARAM_1_SHIFT;
    return binary;
}

unsigned int register_to_binary(register_instruction_ptr_t inst) {
    unsigned int binary = 0;
    binary |= (inst->src_register & REGISTER_INSTRUCTION_SRC_MASK) << REGISTER_INSTRUCTION_SRC_SHIFT;
    binary |= (inst->dst_register & DIRECT_INSTRUCTION_DST_MASK) << DIRECT_INSTRUCTION_DST_SHIFT;
    binary |= (inst->era & DIRECT_INSTRUCTION_ERA_MASK) << DIRECT_INSTRUCTION_ERA_SHIFT;
    return binary;
}

void binary_to_pattern(unsigned int binary, char* pattern) {
    int i;
    char binary_str[15];

    /* initialize pattern with 14 '.' characters */
    memset(pattern, '.', 14);
    pattern[14] = '\0'; /* null-terminate the string */

    /* convert binary to a 14-bit binary string */
    binary_str[14] = '\0'; /* null-terminate the string */
    for (i = 0; i < 14; i++) {
        binary_str[i] = ((binary >> (13 - i)) & 0x1) ? '/' : '.';
    }

    /* copy the 14-bit binary string to the pattern string */
    strncpy(pattern, binary_str, 14);
}

void data_to_binary(image_ptr_t img, int num_data_lines, char** binary_str)
{
    int i;
    char line_num[7];
    char pattern[15];

    *binary_str = malloc_with_monitor((num_data_lines * 22) + 1);
    memset(*binary_str, 0, (num_data_lines * 22) + 1); /* initialize binary_str with null bytes */

     for (i = 0; i < num_data_lines; i++) {
        sprintf(line_num, "%04d  ", get_image_line(img, i));
        strcat(*binary_str, line_num);
        binary_to_pattern(get_single_data_value(img, i), pattern);
        strcat(*binary_str, pattern);
        strcat(*binary_str, "\n");
     }
}



