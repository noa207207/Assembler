#include "data_structure.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "constants.h"

struct symbol{
    char symbol_name[MAX_LABEL_LENGTH];
    char atrributes[MAX_ARRTIBUTE_LENGTH]; /* Maybe not needed considering the bool */
    int value;

    bool isExternal;
    bool isCode;
    bool isData;
    bool isEntry;
};

struct image{
    int line;
    char label[MAX_LABEL_LENGTH];
    int toDecode;
    bool isExtern;
    binary_ptr_t bin;
    addr_method type;
};

struct head{
    symbol_ptr_t table;
    image_ptr_t data_image;
    image_ptr_t code_image;

    int tableUsed;
    int tableSize;

    int dataUsed;
    int dataSize;

    int codeUsed;
    int codeSize;
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

/* Initialization of head data structure. */
head_ptr_t head_init(int tableSize, int dataSize, int codeSize) {
    head_ptr_t tmp = (head_ptr_t)malloc_with_monitor(sizeof(struct head));
    tmp->table = (symbol_ptr_t)malloc_with_monitor(tableSize * sizeof(struct symbol));
    tmp->data_image = (image_ptr_t)malloc_with_monitor(dataSize * sizeof(struct image));
    tmp->code_image = (image_ptr_t)malloc_with_monitor(codeSize * sizeof(struct image));
    tmp->tableUsed = tmp->dataUsed = tmp->codeUsed = 0;
    tmp->tableSize = tableSize;
    tmp->dataSize = dataSize;
    tmp->codeSize = codeSize;
    return tmp;
}

/* Initialization of a single line info */
line_info_ptr_t line_info_init(int opcode_val, addr_method src_addr_val, addr_method dst_addr_val, int src_reg_val, int dst_reg_val, int src_imm_val, int dst_imm_val, char* src_label_val, char* dst_label_val, char* jmp_label_val, addr_method first_param_val, addr_method second_param_val) {
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

void print_binary(unsigned int num) {
    int i;
    for (i = 13; i >= 0; i--) {
        if (num & (1 << i)) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("\n");
}

/* Direct instruction Getter and Setter */

int get_direct_value(head_ptr_t h, int idx)
{
    return get_direct_instruction_value(h->code_image[idx].bin->direct_ptr);
}

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
void set_table(head_ptr_t h, symbol_ptr_t table) {
    h->table = table;
}

symbol_ptr_t get_table(head_ptr_t h) {
    return h->table;
}

image_ptr_t get_data_image(head_ptr_t h) {
    return h->data_image;
}

void set_data_image(head_ptr_t h, image_ptr_t data_image) {
    h->data_image = data_image;
}

image_ptr_t get_code_image(head_ptr_t h) {
    return h->code_image;
}

void set_code_image(head_ptr_t h, image_ptr_t code_image) {
    h->code_image = code_image;
}

int get_table_used(head_ptr_t h) {
    return h->tableUsed;
}

void set_table_used(head_ptr_t h, int tableUsed) {
    h->tableUsed = tableUsed;
}

int get_table_size(head_ptr_t h) {
    return h->tableSize;
}

void set_table_size(head_ptr_t h, int tableSize) {
    h->tableSize = tableSize;
}

int get_data_used(head_ptr_t h) {
    return h->dataUsed;
}

void set_data_used(head_ptr_t h, int dataUsed) {
    h->dataUsed = dataUsed;
}

int get_data_size(head_ptr_t h) {
    return h->dataSize;
}

void set_data_size(head_ptr_t h, int dataSize) {
    h->dataSize = dataSize;
}

int get_code_used(head_ptr_t h) {
    return h->codeUsed;
}

void set_code_used(head_ptr_t h, int codeUsed) {
    h->codeUsed = codeUsed;
}

int get_code_size(head_ptr_t h) {
    return h->codeSize;
}

void set_code_size(head_ptr_t h, int codeSize) {
    h->codeSize = codeSize;
}

/* Getter and Setter for Symbol structure */

char* get_symbol_name(head_ptr_t head, int idx) {
    return head->table[idx].symbol_name;
}

void set_symbol_name(head_ptr_t head, int idx, char* name) {
    strcpy(head->table[idx].symbol_name, name);
}

char* get_symbol_attributes(head_ptr_t head, int idx) {
    return head->table[idx].atrributes;
}

void set_symbol_attributes(head_ptr_t head, int idx, char* attributes) {
    strcpy(head->table[idx].atrributes, attributes);
}

int get_symbol_value(head_ptr_t head, int idx) {
    return head->table[idx].value;
}

void set_symbol_value(head_ptr_t head, int idx, int value) {
    head->table[idx].value = value;
}

bool get_symbol_isExternal(head_ptr_t head, int idx) {
    return head->table[idx].isExternal;
}

void set_symbol_isExternal(head_ptr_t head, int idx, bool isExternal) {
    head->table[idx].isExternal = isExternal;
}

bool get_symbol_isCode(head_ptr_t head, int idx) {
    return head->table[idx].isCode;
}

void set_symbol_isCode(head_ptr_t head, int idx, bool isCode) {
    head->table[idx].isCode = isCode;
}

bool get_symbol_isData(head_ptr_t head, int idx) {
    return head->table[idx].isData;
}

void set_symbol_isData(head_ptr_t head, int idx, bool isData) {
    head->table[idx].isData = isData;
}

bool get_symbol_isEntry(head_ptr_t head, int idx) {
    return head->table[idx].isEntry;
}

void set_symbol_isEntry(head_ptr_t head, int idx, bool isEntry) {
    head->table[idx].isEntry = isEntry;
}

/* Getter and Setter for Image structure */

int get_data_line(head_ptr_t head, int idx) {
    return head->data_image[idx].line;
}

void set_data_line(head_ptr_t head, int idx, int line) {
    head->data_image[idx].line = line;
}

char* get_data_label(head_ptr_t head, int idx) {
    return head->data_image[idx].label;
}

void set_data_label(head_ptr_t head, int idx, char* label) {
    strcpy(head->data_image[idx].label, label);
}

int get_data_toDecode(head_ptr_t head, int idx) {
    return head->data_image[idx].toDecode;
}

void set_data_toDecode(head_ptr_t head, int idx, bool toDecode) {
    head->data_image[idx].toDecode = toDecode;
}

bool get_data_isExtern(head_ptr_t head, int idx) {
    return head->data_image[idx].isExtern;
}

void set_data_isExtern(head_ptr_t head, int idx, bool isExtern) {
    head->data_image[idx].isExtern = isExtern;
}

addr_method get_data_type(head_ptr_t head, int idx) {
    return head->data_image[idx].type;
}

void set_data_type(head_ptr_t head, int idx, addr_method type) {
    head->data_image[idx].type = type;
}

/* Getter and Setter for code_image */

int get_code_line(head_ptr_t head, int idx) {
    return head->code_image[idx].line;
}

void set_code_line(head_ptr_t head, int idx, int line) {
    head->code_image[idx].line = line;
}

char* get_code_label(head_ptr_t head, int idx) {
    return head->code_image[idx].label;
}

void set_code_label(head_ptr_t head, int idx, char* label) {
    strcpy(head->code_image[idx].label, label);
}

int get_code_toDecode(head_ptr_t head, int idx) {
    return head->code_image[idx].toDecode;
}

void set_code_toDecode(head_ptr_t head, int idx, int toDecode) {
    head->code_image[idx].toDecode = toDecode;
}

bool get_code_isExtern(head_ptr_t head, int idx) {
    return head->code_image[idx].isExtern;
}

void set_code_isExtern(head_ptr_t head, int idx, bool isExtern) {
    head->code_image[idx].isExtern = isExtern;
}

addr_method get_code_type(head_ptr_t head, int idx) {
    return head->code_image[idx].type;
}

void set_code_type(head_ptr_t head, int idx, addr_method type) {
    head->code_image[idx].type = type;
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
    node->isExternal = False;
    node->isCode = False;
    node->isData = False;
    node->isEntry = False;
}

/* Handles insertion into symbol table. */
void insert_symbol(head_ptr_t arr, char* name, int value, int op) {
    
    int idx = arr->tableUsed;

    if (is_symbole_exist(arr, name)){
         printf("Label %s is already exist\n", name);
    }

    if (arr->tableUsed == arr->tableSize) {
        arr->tableSize *= 2;
        arr->table = (symbol_ptr_t)realloc_with_monitor(arr->table, arr->tableSize * sizeof(struct symbol));
    }

    strcpy(arr->table[idx].symbol_name, name);
    arr->table[idx].value = value;

    symbol_init(arr->table + idx);

    if (op == DATA || op == STRING)
        arr->table[idx].isData = True;
    else if (op == ENTRY)
        arr->table[idx].isEntry = True;
    else if (op == EXTERNAL)
        arr->table[idx].isExternal = True;
    else /* Assume it is fine */
        arr->table[idx].isCode = True;

    arr->tableUsed++;
}

bool is_symbole_exist(head_ptr_t arr, char *label)
{
    int size = arr->tableUsed;
    int i = 0;

    for (i = 0; i < size; i++) {
        if (!strcmp(arr->table[i].symbol_name, label))
            return True;
    }
    
    return False;

}

/* Inserts into symbol table when the line is a data line. */
void insert_data_symbol(head_ptr_t arr, char* name, int value, int op) {
    insert_symbol(arr, name, value, op);
}

/* Inserts into symbol table when the line is of type .extern. */
void insert_extern(head_ptr_t arr, char* line, int op) {

    delete_spaces(line);
    insert_symbol(arr, line, 0, op);
}

/* Inserts into symbol table when dealing with an instruction line. */
void insert_code_symbol(head_ptr_t arr, char* name, int value, int op) {
    insert_symbol(arr, name, value, op);
}

/* Frees the symbol table's memory allocation. */
void free_symbol_table(head_ptr_t arr) {
    free(arr->table);
    arr->table = NULL;
    arr->tableUsed = arr->tableSize = 0;
}

/* DATA */

/* Handles insertion into data image. */
void insert_data_img(head_ptr_t arr, unsigned int data, int line) {

    int idx = arr->dataUsed;
    single_data_ptr_t data_ptr = single_data_init(data);
    binary_ptr_t bin_ptr = binary_init(SYNGEL_DATA, data_ptr);
    
    if (arr->dataUsed == arr->dataSize) {
        arr->dataSize *= 2;
        arr->data_image = (image_ptr_t)realloc_with_monitor(arr->data_image, arr->dataSize * sizeof(struct image));
    }
    arr->data_image[idx].line = line;
    arr->data_image[idx].bin = bin_ptr;
    arr->data_image[idx].type = SYNGEL_DATA;
    arr->dataUsed++;
}

/* This function updates the Data Count in the Symbol Table and line number in data-image after first pass. */
void update_data_count(head_ptr_t arr, int inst_count) {
    int idx;
    for (idx = 0; idx < arr->dataUsed; idx++)
        arr->data_image[idx].line += inst_count;

    /* Update symbol table and set hist & base. */
    for (idx = 0; idx < arr->tableUsed; idx++)
        if (arr->table[idx].isData) {
            arr->table[idx].value += inst_count;
        }
}

/* Frees all memory allocations of data image. */
void free_data_image(head_ptr_t arr) {
    free(arr->data_image);
    arr->data_image = NULL;
    arr->dataUsed = arr->dataSize = 0;
}

/* CODE */

void resize_img_arr(head_ptr_t arr)
{
     if (arr->codeUsed == arr->codeSize) {
        arr->codeSize += 100;
        arr->code_image = (image_ptr_t)realloc_with_monitor(arr->code_image, arr->codeSize * sizeof(struct image)); /* Add monitor */
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
    arr->code_image[idx].isExtern = False;
    arr->code_image[idx].bin = bin_ptr;
    arr->code_image[idx].toDecode = 0;
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
    arr->code_image[idx].isExtern = False;
    arr->code_image[idx].type = IMMEDIATE;
    arr->code_image[idx].bin = bin_ptr;
    arr->code_image[idx].toDecode = 0;
    arr->codeUsed++;
}

void insert_direct_instruction(head_ptr_t arr, char *label, unsigned int mem_address, int attribute, int line, addr_method method)
{
    int idx = arr->codeUsed;
    direct_instruction_ptr_t direct_inst_ptr = direct_instruction_init(mem_address, label, attribute);
    binary_ptr_t bin_ptr = binary_init(DIRECT, direct_inst_ptr);

    resize_img_arr(arr);
    arr->code_image[idx].line = line;
    arr->code_image[idx].isExtern = False;
    arr->code_image[idx].bin = bin_ptr;
    arr->code_image[idx].toDecode = 1;
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
    arr->code_image[idx].isExtern = False;
    arr->code_image[idx].bin = bin_ptr;
    arr->code_image[idx].toDecode = 0;
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

/* Frees all memory allocations in code image. */
void free_code_image(head_ptr_t arr) {
    free(arr->code_image);
    arr->code_image = NULL;
    arr->codeUsed = arr->codeSize = 0;
}

/* Frees all memory allocations of main DS head. */
void free_head(head_ptr_t arr) {
    free_symbol_table(arr);
    free_data_image(arr);
    free_code_image(arr);
}

/* Receives symbol name and returns its attribute (R,E) assuming BASE/HIST. If doesn't exist, returns -1. */
int get_attribute(head_ptr_t headPtr, char* symbol) {
    int i, length;

    length = headPtr->tableUsed;

    for (i = 0; i < length; i++) {
        if (!strcmp(headPtr->table[i].symbol_name, symbol)) {
            if (headPtr->table[i].isExternal)
                return E;
            else
                return R;
        }
    }

    return -1;
}

/* Returns True if the label already exists in the Symbol Table. */
bool is_duplicate_label(head_ptr_t headPtr, char* line, int length) {
    int i;
    for (i = 0; i < headPtr->tableUsed; i++)
        if (strlen(headPtr->table[i].symbol_name) == length && !strncmp(line, headPtr->table[i].symbol_name, length))
            return True;

    return False;
}

void instructions_to_binary(image_ptr_t img, int num_instructions, char** binary_str) {
    base_instruction_ptr_t base_inst;
    immidiate_instruction_ptr_t imm_inst;
    direct_instruction_ptr_t dir_inst;
    register_instruction_ptr_t reg_inst;
    unsigned int binary;
    char pattern[15];
    char line_num[7];
    int i;

    *binary_str = malloc((num_instructions * 22) + 1);
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
    binary |= (inst->operand & 0xFFF) << 2; /* set bits 2-13 to operand[2-13] */
    binary |= (inst->era & 0x3) << 0; /* set bits 0-1 to era[0-1] */
    return binary;
}

unsigned int direct_to_binary(direct_instruction_ptr_t inst) {
    unsigned int binary = 0;
    binary |= (inst->memory_address & 0xFFF) << 2; /* set bits 2-12 to memory_address[2-12] */
    binary |= (inst->era & 0x3) << 0; /* set bits 0-1 to era[0-1] */
    return binary;
}

unsigned int base_to_binary(base_instruction_ptr_t inst) {
    unsigned int binary = 0;
    binary |= (inst->era & 0x3) << 0; /* set bits 0-1 to era[0-1] */ 
    binary |= (inst->dst_addr & 0x3) << 2; /* set bits 2-3 to dst_addr[2-3] */
    binary |= (inst->src_addr & 0x3) << 4; /* set bits 4-5 to src_addr[4-5] */
    binary |= (inst->opcode & 0xF) << 6; /* set bits 6-9 to opcode[6-9] */
    binary |= (inst->param_2 & 0x3) << 10; /* set bits 10-11 to param2[10-11] */
    binary |= (inst->param_1 & 0x3) << 12; /* set bits 12-13 to param1[12-13] */
    return binary;
}

unsigned int register_to_binary(register_instruction_ptr_t inst) {
    unsigned int binary = 0;
    binary |= (inst->src_register & 0x3F) << 8; /* set bits 8-13 to src_register[8-13] */
    binary |= (inst->dst_register & 0x3F) << 2; /* set bits 2-7 to dst_register[2-7] */
    binary |= (inst->era & 0x3) << 0; /* set bits 0-1 to era[0-1] */
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

    *binary_str = malloc((num_data_lines * 22) + 1);
    memset(*binary_str, 0, (num_data_lines * 22) + 1); /* initialize binary_str with null bytes */

     for (i = 0; i < num_data_lines; i++) {
        sprintf(line_num, "%04d  ", get_image_line(img, i));
        strcat(*binary_str, line_num);
        binary_to_pattern(get_single_data_value(img, i), pattern);
        strcat(*binary_str, pattern);
        strcat(*binary_str, "\n");
     }
}

char *get_direct_instruction_label(direct_instruction_ptr_t direct_ptr)
{
    return direct_ptr->label;
}

unsigned int get_direct_instruction_value(direct_instruction_ptr_t direct_ptr)
{
    return direct_ptr->memory_address;
}

void set_direct_instruction_value(direct_instruction_ptr_t direct_ptr, int value)
{
    direct_ptr->memory_address = value;
}

void set_direct_instruction_era(direct_instruction_ptr_t direct_ptr, enum attributes era)
{
    direct_ptr->era = era;
}

/* Getters */
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

/* Setters */
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

/* PRINT functions*/

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
            arr->code_image[i].isExtern,
            arr->code_image[i].toDecode);

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

            printf("[%d]: s_name = %s, atr = %s, val = %d , ext = %d, code = %d, data  = %d,  entry = %d\n", 
            i,
            arr->table[i].symbol_name,
            arr->table[i].atrributes,
            arr->table[i].value,
            (int)arr->table[i].isExternal,
            (int)arr->table[i].isCode,
            (int)arr->table[i].isData,
            (int)arr->table[i].isEntry);              
        } 
}

void print_data(head_ptr_t arr)
{
    int size = arr->dataUsed;
    int i;

    printf("data size = %d\n", arr->dataSize);

        for (i = 0; i < size; i++)
        {
            printf("[%d]: line = %d", 
            i,
            arr->data_image[i].line);
            print_single_data(arr->data_image[i].bin->data_ptr);
            printf("\n");          
        } 
}



