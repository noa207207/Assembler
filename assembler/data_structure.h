#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include "constants.h"

typedef struct head *head_ptr_t;
typedef struct symbol *symbol_ptr_t;
typedef struct image *image_ptr_t;
typedef enum addr_method addr_method;
typedef enum bool bool;
typedef enum attributes attributes;


typedef struct head head_t;
typedef struct symbol symbol_t;
typedef struct image image_t;


head_ptr_t head_init(int tableSize, int dataSize, int codeSize);

char *get_direct_label(head_ptr_t h, int idx);
void set_direct_value(head_ptr_t h, int idx, int value);
void set_direct_era(head_ptr_t h, int idx, attributes attr);

// Getter and setter functions for the 'table' field
symbol_ptr_t get_table(head_ptr_t h);
void set_table(head_ptr_t h, symbol_ptr_t table);

// Getter and setter functions for the 'data_image' field
image_ptr_t get_data_image(head_ptr_t h);
void set_data_image(head_ptr_t h, image_ptr_t data_image);

// Getter and setter functions for the 'code_image' field
image_ptr_t get_code_image(head_ptr_t h);
void set_code_image(head_ptr_t h, image_ptr_t code_image);

// Getter and setter functions for the 'tableUsed' field
int get_table_used(head_ptr_t h);
void set_table_used(head_ptr_t h, int tableUsed);

// Getter and setter functions for the 'tableSize' field
int get_table_size(head_ptr_t h);
void set_table_size(head_ptr_t h, int tableSize);

// Getter and setter functions for the 'dataUsed' field
int get_data_used(head_ptr_t h);
void set_data_used(head_ptr_t h, int dataUsed);

// Getter and setter functions for the 'dataSize' field
int get_data_size(head_ptr_t h);
void set_data_size(head_ptr_t h, int dataSize);

// Getter and setter functions for the 'codeUsed' field
int get_code_used(head_ptr_t h);
void set_code_used(head_ptr_t h, int codeUsed);

// Getter and setter functions for the 'codeSize' field
int get_code_size(head_ptr_t h);
void set_code_size(head_ptr_t h, int codeSize);

// Symbol table getter and setter functions
char *get_symbol_name(head_ptr_t head, int idx);
void set_symbol_name(head_ptr_t head, int idx, char *symbol_name);
char *get_symbol_attributes(head_ptr_t head, int idx);
void set_symbol_attributes(head_ptr_t head, int idx, char *attributes);
int get_symbol_value(head_ptr_t head, int idx);
void set_symbol_value(head_ptr_t head, int idx, int value);
enum bool get_symbol_isExternal(head_ptr_t head, int idx);
void set_symbol_isExternal(head_ptr_t head, int idx, bool isExternal);
enum bool get_symbol_isCode(head_ptr_t head, int idx);
void set_symbol_isCode(head_ptr_t head, int idx, bool isCode);
enum bool get_symbol_isData(head_ptr_t head, int idx);
void set_symbol_isData(head_ptr_t head, int idx, bool isData);
enum bool get_symbol_isEntry(head_ptr_t head, int idx);
void set_symbol_isEntry(head_ptr_t head, int idx, bool isEntry);

/* Getter and Setter for Image structure */

// Getter function to access line field
int get_data_line(head_ptr_t head, int idx);

// Setter function to update line field
void set_data_line(head_ptr_t head, int idx, int line);

// Getter function to access label field
char* get_data_label(head_ptr_t head, int idx);

// Setter function to update label field
void set_data_label(head_ptr_t head, int idx, char* label);

// Getter function to access toDecode field
int get_data_toDecode(head_ptr_t head, int idx);

// Setter function to update toDecode field
void set_data_toDecode(head_ptr_t head, int idx, enum bool toDecode);

// Getter function to access isExtern field
enum bool get_data_isExtern(head_ptr_t head, int idx);

// Setter function to update isExtern field
void set_data_isExtern(head_ptr_t head, int idx, enum bool isExtern);

// Getter function to access type field
addr_method get_data_type(head_ptr_t head, int idx);

// Setter function to update type field
void set_data_type(head_ptr_t head, int idx, addr_method type);

/* Getter and Setter for code_image */

// Getter function to access line field
int get_code_line(head_ptr_t head, int idx);

// Setter function to update line field
void set_code_line(head_ptr_t head, int idx, int line);

// Getter function to access label field
char* get_code_label(head_ptr_t head, int idx);

// Setter function to update label field
void set_code_label(head_ptr_t head, int idx, char* label);

// Getter function to access toDecode field
int get_code_toDecode(head_ptr_t head, int idx);

// Setter function to update toDecode field
void set_code_toDecode(head_ptr_t head, int idx, int toDecode);

// Getter function to access isExtern field
enum bool get_code_isExtern(head_ptr_t head, int idx);

// Setter function to update isExtern field
void set_code_isExtern(head_ptr_t head, int idx, enum bool isExtern);

// Getter function to access type field
addr_method get_code_type(head_ptr_t head, int idx);

// Setter function to update type field
void set_code_type(head_ptr_t head, int idx, addr_method type);



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
								unsigned int dst_addr, int attribute, int line, unsigned int param1, unsigned int param2);
void insert_code_instruction(head_ptr_t, line_info_ptr_t, int);
void insert_direct_instruction(head_ptr_t arr, char *label, unsigned int mem_address, int attribute, int line, int is_jmp);
void insert_register_instruction(head_ptr_t arr, unsigned int src_register, unsigned int dst_register, int attribute, int line);
void insert_immidiate_instruction(head_ptr_t arr, unsigned int operand, int attribute, int line);


void insert_code_immediate(head_ptr_t, int, int, int, char*, int);
void free_code_image(head_ptr_t);

void free_head(head_ptr_t);

int get_attribute(head_ptr_t, char*);
bool is_symbole_exist(head_ptr_t arr, char *label);

void print_head_code_bin(head_ptr_t arr);
void print_symbols(head_ptr_t arr);
void print_data(head_ptr_t arr);

#endif
