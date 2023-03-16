#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include "constants.h"

typedef struct head *head_ptr_t;
typedef struct symbol *symbol_ptr_t;
typedef struct image *image_ptr_t;
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

typedef struct head head_t;
typedef struct symbol symbol_t;
typedef struct image image_t;


head_ptr_t head_init(int tableSize, int dataSize, int codeSize);
void instructions_to_binary(image_ptr_t img, int num_instructions, char** binary_str);
void data_to_binary(image_ptr_t img, int num_data_lines, char** binary_str);

char *get_direct_label(head_ptr_t h, int idx);
void set_direct_value(head_ptr_t h, int idx, int value);
void set_direct_era(head_ptr_t h, int idx, enum attributes attr);

symbol_ptr_t get_table(head_ptr_t h);
void set_table(head_ptr_t h, symbol_ptr_t table);
image_ptr_t get_data_image(head_ptr_t h);
void set_data_image(head_ptr_t h, image_ptr_t data_image);
image_ptr_t get_code_image(head_ptr_t h);
void set_code_image(head_ptr_t h, image_ptr_t code_image);
int get_table_used(head_ptr_t h);
void set_table_used(head_ptr_t h, int tableUsed);
int get_table_size(head_ptr_t h);
void set_table_size(head_ptr_t h, int tableSize);
int get_data_used(head_ptr_t h);
void set_data_used(head_ptr_t h, int dataUsed);
int get_data_size(head_ptr_t h);
void set_data_size(head_ptr_t h, int dataSize);
int get_code_used(head_ptr_t h);
void set_code_used(head_ptr_t h, int codeUsed);
int get_code_size(head_ptr_t h);
void set_code_size(head_ptr_t h, int codeSize);
int get_image_line(image_ptr_t img, int idx);
int get_single_data_value(image_ptr_t img, int idx);
char *get_symbol_name(head_ptr_t head, int idx);
void set_symbol_name(head_ptr_t head, int idx, char *symbol_name);
char *get_symbol_attributes(head_ptr_t head, int idx);
void set_symbol_attributes(head_ptr_t head, int idx, char *attributes);
int get_symbol_value(head_ptr_t head, int idx);
void set_symbol_value(head_ptr_t head, int idx, int value);
bool get_symbol_isExternal(head_ptr_t head, int idx);
void set_symbol_isExternal(head_ptr_t head, int idx, bool isExternal);
bool get_symbol_isCode(head_ptr_t head, int idx);
void set_symbol_isCode(head_ptr_t head, int idx, bool isCode);
bool get_symbol_isData(head_ptr_t head, int idx);
void set_symbol_isData(head_ptr_t head, int idx, bool isData);
bool get_symbol_isEntry(head_ptr_t head, int idx);
void set_symbol_isEntry(head_ptr_t head, int idx, bool isEntry);
int get_data_line(head_ptr_t head, int idx);
void set_data_line(head_ptr_t head, int idx, int line);
char* get_data_label(head_ptr_t head, int idx);
void set_data_label(head_ptr_t head, int idx, char* label);
int get_data_toDecode(head_ptr_t head, int idx);
void set_data_toDecode(head_ptr_t head, int idx, bool toDecode);
bool get_data_isExtern(head_ptr_t head, int idx);
void set_data_isExtern(head_ptr_t head, int idx, bool isExtern);
addr_method get_data_type(head_ptr_t head, int idx);
void set_data_type(head_ptr_t head, int idx, addr_method type);
int get_code_line(head_ptr_t head, int idx);
void set_code_line(head_ptr_t head, int idx, int line);
char* get_code_label(head_ptr_t head, int idx);
void set_code_label(head_ptr_t head, int idx, char* label);
int get_code_toDecode(head_ptr_t head, int idx);
void set_code_toDecode(head_ptr_t head, int idx, int toDecode);
bool get_code_isExtern(head_ptr_t head, int idx);
void set_code_isExtern(head_ptr_t head, int idx, bool isExtern);
addr_method get_code_type(head_ptr_t head, int idx);
void set_code_type(head_ptr_t head, int idx, addr_method type);
void symbol_init(symbol_ptr_t);

/* Symbol table functions */
void insert_symbol(head_ptr_t arr, char* name, int value, int op);
void insert_data_symbol(head_ptr_t arr, char* name, int value, int op);
void insert_extern(head_ptr_t arr, char* line, int op);
void insert_code_symbol(head_ptr_t arr, char* name, int value, int op);
void free_symbol_table(head_ptr_t arr);

/* Data image functions */
void insert_data_img(head_ptr_t arr, unsigned int data, int line);
void update_data_count(head_ptr_t arr, int inst_count);
void free_data_image(head_ptr_t);

/* Code image functions */
void insert_base_instruction(head_ptr_t arr, unsigned int opcode, unsigned int src_addr,
								unsigned int dst_addr, int attribute, int line, unsigned int param1, unsigned int param2);
void insert_direct_instruction(head_ptr_t arr, char *label, unsigned int mem_address, int attribute, int line, addr_method method);
void insert_register_instruction(head_ptr_t arr, unsigned int src_register, unsigned int dst_register, int attribute, int line);
void insert_immidiate_instruction(head_ptr_t arr, unsigned int operand, int attribute, int line);


void insert_code_immediate(head_ptr_t, int, int, int, char*, int);
void free_code_image(head_ptr_t);

void free_head(head_ptr_t);

int get_attribute(head_ptr_t, char*);
bool is_symbole_exist(head_ptr_t arr, char *label);
int get_direct_value(head_ptr_t h, int idx);

void print_head_code_bin(head_ptr_t arr);
void print_symbols(head_ptr_t arr);
void print_data(head_ptr_t arr);
unsigned int base_to_binary(base_instruction_ptr_t inst);
unsigned int immidiate_to_binary(immidiate_instruction_ptr_t inst);
unsigned int direct_to_binary(direct_instruction_ptr_t inst);
unsigned int register_to_binary(register_instruction_ptr_t inst);
void binary_to_pattern(unsigned int binary, char* pattern);
unsigned int get_direct_instruction_value(direct_instruction_ptr_t direct_ptr);


base_instruction_ptr_t base_instruction_init(unsigned int param_1, unsigned int param_2, unsigned int opcode, unsigned int src_addr, unsigned int dst_addr, unsigned int era);
immidiate_instruction_ptr_t immidiate_instruction_init(unsigned int operand, unsigned int era);
direct_instruction_ptr_t direct_instruction_init(unsigned int memory_address, char* label, unsigned int era);
register_instruction_ptr_t register_instruction_init(unsigned int src_register, unsigned int dst_register, unsigned int era);
opcode_bin_ptr_t opcode_bin_init(unsigned int opcode, unsigned int attribute);
single_data_ptr_t single_data_init(unsigned int value);
binary_ptr_t binary_init(addr_method type, void* ptr);
line_info_ptr_t line_info_init(int opcode_val, addr_method src_addr_val, addr_method dst_addr_val, 
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
int get_opcode(line_info_ptr_t line);
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
void set_direct_instruction_era(direct_instruction_ptr_t direct_ptr, enum attributes era);
int get_data_value(single_data_ptr_t data);

/* Setter functions */
void set_opcode(line_info_ptr_t line, int opcode);
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


void *get_bin_by_type(image_ptr_t img);

#endif
