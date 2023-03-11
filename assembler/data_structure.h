#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include "constants.h"

typedef struct head *head_ptr_t;
typedef struct symbol *symbol_ptr_t;
typedef struct image *image_ptr_t;

typedef struct head head_t;
typedef struct symbol symbol_t;
typedef struct image image_t;


head_ptr_t head_init(int tableSize, int dataSize, int codeSize);
void symbol_init(symbol_ptr_t);

/* Symbol table functions */
void insert_symbol(head_ptr_t, char*, int, opcode);
void insert_data_symbol(head_ptr_t, char*, int, opcode);
void insert_extern(head_ptr_t, char*, opcode);
void insert_code_symbol(head_ptr_t, char*, int, opcode);
void free_symbol_table(head_ptr_t);

/* Data image functions */
void insert_data_img(head_ptr_t, unsigned int, int);
void update_data_count(head_ptr_t, int);
void free_data_image(head_ptr_t);

/* Code image functions */
void insert_base_instruction(head_ptr_t arr, unsigned int opcode, unsigned int src_addr,
								unsigned int dst_addr, int attribute, int line);
void insert_code_instruction(head_ptr_t, line_info*, int);
void insert_direct_instruction(head_ptr_t arr, char *label, unsigned int mem_address, int attribute, int line);
void insert_register_instruction(head_ptr_t arr, unsigned int src_register, unsigned int dst_register, int attribute, int line);
void insert_immidiate_instruction(head_ptr_t arr, unsigned int operand, int attribute, int line);


void insert_code_immediate(head_ptr_t, int, int, int, char*, int);
void free_code_image(head_ptr_t);

void free_head(head_ptr_t);

int get_attribute(head_ptr_t, char*);

void print_head_code_bin(head_ptr_t arr);
void print_symbols(head_ptr_t arr);
void print_data(head_ptr_t arr);

#endif
