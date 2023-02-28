#include "data_structure.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

/* Initialization of head data structure. */
void head_init(head* arr, int tableSize, int dataSize, int codeSize) {
    arr->table = (symbol*)malloc_with_monitor(tableSize * sizeof(symbol));
    arr->data_image = (image*)malloc_with_monitor(dataSize * sizeof(image));
    arr->code_image = (image*)malloc_with_monitor(codeSize * sizeof(image));
    arr->tableUsed = arr->dataUsed = arr->codeUsed = 0;
    arr->tableSize = tableSize;
    arr->dataSize = dataSize;
    arr->codeSize = codeSize;
}

/* Initialization of symbol node. */
void symbol_init(symbol* node) {
    node->isExternal = False;
    node->isCode = False;
    node->isData = False;
    node->isEntry = False;
}

/* SYMBOLS */

/* Handles insertion into symbol table. */
void insert_symbol(head* arr, char* name, int value, int base, int offset, opcode op) {
    int idx = arr->tableUsed;
    if (arr->tableUsed == arr->tableSize) {
        arr->tableSize *= 2;
        arr->table = (symbol*)realloc_with_monitor(arr->table, arr->tableSize * sizeof(symbol));
    }

    strcpy(arr->table[idx].symbol_name, name);
    arr->table[idx].value = value;
    arr->table[idx].base = base;
    arr->table[idx].offset = offset;

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

/* Inserts into symbol table when the line is a data line. */
void insert_data_symbol(head* arr, char* name, int value, opcode op) {
    insert_symbol(arr, name, value, 0, 0, op);
}

/* Inserts into symbol table when the line is of type .extern. */
void insert_extern(head* arr, char* line, opcode op) {
    delete_spaces(line);
    insert_symbol(arr, line, 0, 0, 0, op);
}

/* Inserts into symbol table when dealing with an instruction line. */
void insert_code_symbol(head* arr, char* name, int value, opcode op) {
    insert_symbol(arr, name, value, getBase(value), getHist(value), op);
}

/* Frees the symbol table's memory allocation. */
void free_symbol_table(head* arr) {
    free(arr->table);
    arr->table = NULL;
    arr->tableUsed = arr->tableSize = 0;
}

/* DATA */

/* Handles insertion into data image. */
// void insert_data_img(head* arr, unsigned int data, int line) {
//     int idx = arr->dataUsed;
//     if (arr->dataUsed == arr->dataSize) {
//         arr->dataSize *= 2;
//         arr->data_image = (image*)realloc_with_monitor(arr->data_image, arr->dataSize * sizeof(image));
//     }
//     arr->data_image[idx].line = line;
//     arr->data_image[idx].bin.word.opcode = data;
//     arr->data_image[idx].bin.word.attribute = A;

//     arr->dataUsed++;
// }
/* This function updates the Data Count in the Symbol Table and line number in data-image after first pass. */
void update_data_count(head* arr, int inst_count) {
    int idx;
    for (idx = 0; idx < arr->dataUsed; idx++)
        arr->data_image[idx].line += inst_count;

    /* Update symbol table and set hist & base. */
    for (idx = 0; idx < arr->tableUsed; idx++)
        if (arr->table[idx].isData) {
            arr->table[idx].value += inst_count;
            arr->table[idx].base = getBase(arr->table[idx].value);
            arr->table[idx].offset = getHist(arr->table[idx].value);
        }
}

/* Frees all memory allocations of data image. */
void free_data_image(head* arr) {
    free(arr->data_image);
    arr->data_image = NULL;
    arr->dataUsed = arr->dataSize = 0;
}

/* CODE */

static int resize_img_arr(head* arr)
{
     if (arr->codeUsed == arr->codeSize) {
        arr->codeSize += 100;
        arr->code_image = (image*)realloc_with_monitor(arr->code_image, arr->codeSize * sizeof(image)); /* Add monitor */
    }
}

/* Handles insertion into code image when given an opcode. */
void insert_base_instruction(head* arr, unsigned int opcode, unsigned int src_addr,
                                unsigned int dst_addr, int attribute, int line)
{
    int idx = arr->codeUsed;
    base_instruction *base_inst_ptr = malloc(sizeof(base_instruction));
    
    resize_img_arr(arr);

    arr->code_image[idx].line = line;
    arr->code_image[idx].isExtern = False;
    base_inst_ptr->opcode = opcode;
    base_inst_ptr->src_addr = src_addr;
    base_inst_ptr->dst_addr = dst_addr;
    base_inst_ptr->era = attribute;
    arr->code_image[idx].bin.base_ptr = base_inst_ptr;
    arr->code_image[idx].toDecode = DONE;
    arr->codeUsed++;
    arr->code_image[idx].type = BASE;
}

void insert_immidiate_instruction(head* arr, unsigned int operand, int attribute, int line)
{
    int idx = arr->codeUsed;
    immidiate_instruction *imm_inst_ptr = malloc(sizeof(immidiate_instruction));
    
    resize_img_arr(arr);

    arr->code_image[idx].line = line;
    arr->code_image[idx].isExtern = False;
    arr->code_image[idx].type = IMMEDIATE;
    imm_inst_ptr->operand = operand;
    imm_inst_ptr->era = attribute;
    arr->code_image[idx].bin.immidiate_ptr = imm_inst_ptr;
    arr->code_image[idx].toDecode = DONE;
    arr->code_image[idx].type = IMMEDIATE;
    arr->codeUsed++;
}

void insert_direct_instruction(head* arr, char *label, unsigned int mem_address, int attribute, int line)
{
    int idx = arr->codeUsed;
    direct_instruction *direct_inst_ptr = malloc(sizeof(direct_instruction));
    
    resize_img_arr(arr);

    printf("%d\n", __LINE__);

    arr->code_image[idx].line = line;
    arr->code_image[idx].isExtern = False;
    direct_inst_ptr->memory_address = mem_address;
    printf("%d\n", __LINE__);
    strcpy(direct_inst_ptr->label, label);
    direct_inst_ptr->era = attribute;
    printf("%d\n", __LINE__);
    arr->code_image[idx].bin.direct_ptr = direct_inst_ptr;
    arr->code_image[idx].toDecode = 1;
    arr->code_image[idx].type = DIRECT;
    printf("%d\n", __LINE__);
    arr->codeUsed++;

    printf("%s\n",  arr->code_image[idx].bin.direct_ptr->label);
}

void insert_register_instruction(head* arr, unsigned int src_register, unsigned int dst_register, int attribute, int line)
{
    int idx = arr->codeUsed;
    register_instruction *register_inst_ptr = malloc(sizeof(register_instruction));
    
    resize_img_arr(arr);

    arr->code_image[idx].line = line;
    arr->code_image[idx].isExtern = False;
    register_inst_ptr->src_register = src_register;
    register_inst_ptr->dst_register = dst_register;
    register_inst_ptr->era = attribute;
    arr->code_image[idx].bin.register_ptr = register_inst_ptr;
    arr->code_image[idx].toDecode = 0;
    arr->code_image[idx].type = REG_DIRECT;
    arr->codeUsed++;
}

/* Frees all memory allocations in code image. */
void free_code_image(head* arr) {
    free(arr->code_image);
    arr->code_image = NULL;
    arr->codeUsed = arr->codeSize = 0;
}

/* Frees all memory allocations of main DS head. */
void free_head(head* arr) {
    free_symbol_table(arr);
    free_data_image(arr);
    free_code_image(arr);
}

/* Receives symbol name and returns its attribute (R,E) assuming BASE/HIST. If doesn't exist, returns -1. */
int get_attribute(head* headPtr, char* symbol) {
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

void print_head_code_bin(head* arr)
{
        int size = arr->codeUsed;
        int i;

        for (i = 0; i < size; i++)
        {
            printf("[%d]: line = %d, is_extern = %d, to_decode = %d ", i, arr->code_image[i].line,
            arr->code_image[i].isExtern,
            arr->code_image[i].toDecode);
            if (arr->code_image[i].type == BASE)
                printf("BASE: opcode = %d, src = %u, dst = %d, param = %u, param2 = %u\n",
                (int)arr->code_image[i].bin.base_ptr->opcode,
                    arr->code_image[i].bin.base_ptr->src_addr,
                    arr->code_image[i].bin.base_ptr->dst_addr,
                    arr->code_image[i].bin.base_ptr->param_1,
                    arr->code_image[i].bin.base_ptr->param_2);
            if (arr->code_image[i].type == REG_DIRECT)
                printf("REGISTER: src_register = %u, dst_register = %u\n",
                    arr->code_image[i].bin.register_ptr->src_register,
                    arr->code_image[i].bin.register_ptr->dst_register);
             if (arr->code_image[i].type == IMMEDIATE)
                printf("IMMIDIATE: operand = 0x%x\n",
                    arr->code_image[i].bin.immidiate_ptr->operand);
             if (arr->code_image[i].type == DIRECT)
                printf("DIRECT: label = %s\n",
                    arr->code_image[i].bin.direct_ptr->label);               
        } 
}