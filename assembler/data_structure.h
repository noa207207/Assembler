#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include "constants.h"

typedef struct {
    char symbol_name[MAX_LABEL_LENGTH];
    char atrributes[MAX_ARRTIBUTE_LENGTH]; /* Maybe not needed considering the bool */
    int value;
    int base;
    int offset;

    bool isExternal;
    bool isCode;
    bool isData;
    bool isEntry;
} symbol;

typedef struct {
    int line;
	int code_line;
    char label[MAX_LABEL_LENGTH];
    int toDecode;
    bool isExtern;
    binary bin;
    addr_method type;
} image;

typedef struct {
    symbol* table;
    image* data_image;
    image* code_image;

    int tableUsed;
    int tableSize;

    int dataUsed;
    int dataSize;

    int codeUsed;
    int codeSize;
} head;

void head_init(head*, int, int, int);
void image_init(image*);
void symbol_init(symbol*);

/* Symbol table functions */
void insert_symbol(head*, char*, int, int, int, opcode);
void insert_data_symbol(head*, char*, int, opcode);
void insert_extern(head*, char*, opcode);
void insert_code_symbol(head*, char*, int, opcode);
void free_symbol_table(head*);

/* Data image functions */
void insert_data_img(head*, unsigned int, int);
void update_data_count(head*, int);
void free_data_image(head*);

/* Code image functions */
void insert_base_instruction(head* arr, unsigned int opcode, unsigned int src_addr,
								unsigned int dst_addr, int attribute, int line);
void insert_code_instruction(head*, line_info*, int);
void insert_direct_instruction(head* arr, char *label, unsigned int mem_address, int attribute, int line);
void insert_register_instruction(head* arr, unsigned int src_register, unsigned int dst_register, int attribute, int line);
void insert_immidiate_instruction(head* arr, unsigned int operand, int attribute, int line);


void insert_code_immediate(head*, int, int, int, char*, int);
void free_code_image(head*);

void free_head(head*);

int get_attribute(head*, char*);

void print_head_code_bin(head* arr);

#endif
