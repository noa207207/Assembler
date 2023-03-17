#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include "constants.h"

/* Data structures encapsulation*/

typedef struct head *head_ptr_t;
typedef struct symbol *symbol_ptr_t;
typedef struct image *image_ptr_t;
typedef struct base_instruction *base_instruction_ptr_t;
typedef struct single_data *single_data_ptr_t;
typedef struct immidiate_instruction *immidiate_instruction_ptr_t;
typedef struct direct_instruction *direct_instruction_ptr_t;
typedef struct register_instruction *register_instruction_ptr_t;
typedef struct line_info *line_info_ptr_t;
typedef union binary *binary_ptr_t;

union binary {
    base_instruction_ptr_t base_ptr;
    immidiate_instruction_ptr_t immidiate_ptr;
    direct_instruction_ptr_t direct_ptr;
    register_instruction_ptr_t register_ptr;
    single_data_ptr_t data_ptr;
};

/* Initialization of head data structure. */
head_ptr_t head_init(int table_size, int data_size, int code_size);

/* Initialization of an empty single line info */
line_info_ptr_t line_info_empty_init(void);

/* Initialization of an empty single line info */

char *get_direct_label(head_ptr_t h, int idx);
void set_direct_value(head_ptr_t h, int idx, int value);
void set_direct_era(head_ptr_t h, int idx, enum attributes attr);

/* Head Getter and Setter */

image_ptr_t get_data_image(head_ptr_t h);
image_ptr_t get_code_image(head_ptr_t h);
int get_table_used(head_ptr_t h);
int get_table_size(head_ptr_t h);
int get_data_used(head_ptr_t h);
int get_code_used(head_ptr_t h);
int get_code_size(head_ptr_t h);
int get_image_line(image_ptr_t img, int idx);
int get_single_data_value(image_ptr_t img, int idx);

/* Getter and Setter for Symbol structure */
char *get_symbol_name(head_ptr_t head, int idx);
int get_symbol_value(head_ptr_t head, int idx);
bool get_symbol_is_external(head_ptr_t head, int idx);
bool get_symbol_is_entry(head_ptr_t head, int idx);
void set_symbol_is_entry(head_ptr_t head, int idx, bool is_entry);


/* Getter and Setter for code_image */

int get_code_line(head_ptr_t head, int idx);
int get_code_to_decode(head_ptr_t head, int idx);
bool get_code_is_extern(head_ptr_t head, int idx);
void set_code_is_extern(head_ptr_t head, int idx, bool is_extern);

/* Symbol table functions */
void symbol_init(symbol_ptr_t);

/* Handles insertion into symbol table. */
void insert_symbol(head_ptr_t arr, char* name, int value, int op);

/* Inserts into symbol table when the line is a data line. */
void insert_data_symbol(head_ptr_t arr, char* name, int value, int op);

/* Inserts into symbol table when the line is of type .extern. */
void insert_extern(head_ptr_t arr, char* line, int op);

/* Inserts into symbol table when dealing with an instruction line. */
void insert_code_symbol(head_ptr_t arr, char* name, int value, int op);

/* Frees the symbol table's memory allocation. */
void free_symbol_table(head_ptr_t arr);

/* Returns True if the label already exists in the Symbol Table. */
bool is_duplicate_label(head_ptr_t headPtr, char* line, int length);

/* Data image functions */

/* Handles insertion into data image. */
void insert_data_img(head_ptr_t arr, unsigned int data, int line);

/* Updates the Data Count in the Symbol Table and line number in data-image after first pass. */
void update_data_count(head_ptr_t arr, int inst_count);

/* Frees all memory allocations of data image. */
void free_data_image(head_ptr_t);

/* Handles insertion into code image when given an opcode. */
void insert_base_instruction(head_ptr_t arr, unsigned int opcode, unsigned int src_addr,
								unsigned int dst_addr, int attribute, int line, unsigned int param1, unsigned int param2);

/* Insert direct instruction into code image. */
void insert_direct_instruction(head_ptr_t arr, char *label, unsigned int mem_address, int attribute, int line, addr_method method);

/* Insert register instruction into code image. */
void insert_register_instruction(head_ptr_t arr, unsigned int src_register, unsigned int dst_register, int attribute, int line);

/* Insert immidiate instruction into code image. */
void insert_immidiate_instruction(head_ptr_t arr, unsigned int operand, int attribute, int line);

/* Switch operand address method and instruction into code image. */
int switch_and_insert(head_ptr_t arr, line_info_ptr_t instruction, int inst_count, addr_method address_method, bool is_dst);

/* Frees all memory allocations in code image. */
void free_code_image(head_ptr_t);

/* Frees all memory allocations of main DS head. */
void free_head(head_ptr_t);

/* Direct instruction getter and setter */

void set_direct_instruction_value(direct_instruction_ptr_t direct_ptr, int value);
void set_direct_instruction_era(direct_instruction_ptr_t direct_ptr, enum attributes era);

/* Line info Getter */

int get_opcode(line_info_ptr_t line);
addr_method get_src_addr(line_info_ptr_t line);
addr_method get_dst_addr(line_info_ptr_t line);
int get_src_reg(line_info_ptr_t line);
int get_dst_reg(line_info_ptr_t line);
char* get_dst_label(line_info_ptr_t line);
addr_method get_first_param(line_info_ptr_t line);
addr_method get_second_param(line_info_ptr_t line);
char *get_direct_instruction_label(direct_instruction_ptr_t direct_ptr);
void set_direct_instruction_value(direct_instruction_ptr_t direct_ptr, int value);
void set_direct_instruction_era(direct_instruction_ptr_t direct_ptr, enum attributes era);

/* Line info Setters */

void set_opcode(line_info_ptr_t line, int opcode);
void set_src_addr(line_info_ptr_t line, addr_method src_addr);
void set_dst_addr(line_info_ptr_t line, addr_method dst_addr);
void set_src_reg(line_info_ptr_t line, int src_reg);
void set_dst_reg(line_info_ptr_t line, int dst_reg);
void set_src_imm(line_info_ptr_t line, int src_imm);
void set_dst_imm(line_info_ptr_t line, int dst_imm);
void set_src_label(line_info_ptr_t line, char* src_label, int size);
void set_dst_label(line_info_ptr_t line, char* dst_label, int size);
void set_first_param(line_info_ptr_t line, addr_method first_param);
void set_second_param(line_info_ptr_t line, addr_method second_param);
int get_data_value(single_data_ptr_t data);

/* Instructions Init*/

base_instruction_ptr_t base_instruction_init(unsigned int param_1, unsigned int param_2, unsigned int opcode, unsigned int src_addr, unsigned int dst_addr, unsigned int era);
immidiate_instruction_ptr_t immidiate_instruction_init(unsigned int operand, unsigned int era);
direct_instruction_ptr_t direct_instruction_init(unsigned int memory_address, char* label, unsigned int era);
register_instruction_ptr_t register_instruction_init(unsigned int src_register, unsigned int dst_register, unsigned int era);
single_data_ptr_t single_data_init(unsigned int value);
binary_ptr_t binary_init(addr_method type, void* ptr);

/* Print Functions (Only for testing)*/
void print_head_code_bin(head_ptr_t arr);
void print_symbols(head_ptr_t arr);
void print_data(head_ptr_t arr);
void print_base_instruction(base_instruction_ptr_t base_ptr);
void print_immidiate_instruction(immidiate_instruction_ptr_t immidiate_ptr);
void print_direct_instruction(direct_instruction_ptr_t direct_ptr);
void print_register_instruction(register_instruction_ptr_t register_ptr);
void print_single_data(single_data_ptr_t data_ptr);

/* Binary Encode */
/* Encode instructions to binary format according the mask and shift */

void instructions_to_binary(image_ptr_t img, int num_instructions, char** binary_str);
unsigned int base_to_binary(base_instruction_ptr_t inst);
unsigned int immidiate_to_binary(immidiate_instruction_ptr_t inst);
unsigned int direct_to_binary(direct_instruction_ptr_t inst);
unsigned int register_to_binary(register_instruction_ptr_t inst);
void binary_to_pattern(unsigned int binary, char* pattern);
void data_to_binary(image_ptr_t img, int num_data_lines, char** binary_str);
void *get_bin_by_type(image_ptr_t img);


#endif
