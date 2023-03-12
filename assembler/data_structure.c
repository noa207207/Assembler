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
    printf("code codeSize = %d\n", tmp->codeSize);
    return tmp;
}

/* Initialization of symbol node. */
void symbol_init(symbol_ptr_t node) {
    node->isExternal = False;
    node->isCode = False;
    node->isData = False;
    node->isEntry = False;
}

/* SYMBOLS */

/* Handles insertion into symbol table. */
void insert_symbol(head_ptr_t arr, char* name, int value, opcode op) {
    printf("%s: name = %s, val = %d\n", __func__, name, value);
    int idx = arr->tableUsed;
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

/* Inserts into symbol table when the line is a data line. */
void insert_data_symbol(head_ptr_t arr, char* name, int value, opcode op) {
insert_symbol(arr, name, value, op);
}

/* Inserts into symbol table when the line is of type .extern. */
void insert_extern(head_ptr_t arr, char* line, opcode op) {

    delete_spaces(line);
    insert_symbol(arr, line, 0, op);
}

/* Inserts into symbol table when dealing with an instruction line. */
void insert_code_symbol(head_ptr_t arr, char* name, int value, opcode op) {
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


int get_code_size(head_ptr_t arr)
{
    return arr->codeSize;
}

int resize_img_arr(head_ptr_t arr)
{
    printf("reszie\n");
    printf("code size = %d\n",arr->codeSize);
     if (arr->codeUsed == arr->codeSize) {
        arr->codeSize += 100;
        arr->code_image = (image_ptr_t)realloc_with_monitor(arr->code_image, arr->codeSize * sizeof(struct image)); /* Add monitor */
    }
    printf("code size = %d\n",arr->codeSize);
}

/* Handles insertion into code image when given an opcode. */
void insert_base_instruction(head_ptr_t arr, unsigned int opcode, unsigned int src_addr,
                                unsigned int dst_addr, int attribute, int line, unsigned int param1, unsigned int param2)
{
    printf("the 2 pointer is %p\n", arr);
    printf("222the size is = %d\n", get_code_size(arr));
    int idx = arr->codeUsed;
    base_instruction_ptr_t base_inst_ptr = base_instruction_init(param1, param2, opcode, src_addr,dst_addr, attribute); 
    binary_ptr_t bin_ptr = binary_init(BASE, base_inst_ptr);
    resize_img_arr(arr);
    printf("idx = %d\n", idx);
    arr->code_image[idx].line = line;
    arr->code_image[idx].isExtern = False;
    arr->code_image[idx].bin = bin_ptr;
    arr->code_image[idx].toDecode = DONE;
    arr->codeUsed++;
    arr->code_image[idx].type = BASE;
    printf("noa\n");
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
    arr->code_image[idx].toDecode = DONE;
    arr->code_image[idx].type = IMMEDIATE;
    arr->codeUsed++;
}

void insert_direct_instruction(head_ptr_t arr, char *label, unsigned int mem_address, int attribute, int line, int is_jmp)
{
    int idx = arr->codeUsed;
    direct_instruction_ptr_t direct_inst_ptr = direct_instruction_init(mem_address, label, attribute);
    binary_ptr_t bin_ptr = binary_init(DIRECT, direct_inst_ptr);

    resize_img_arr(arr);
    arr->code_image[idx].line = line;
    arr->code_image[idx].isExtern = False;
    arr->code_image[idx].bin = bin_ptr;
    arr->code_image[idx].toDecode = 1;
    arr->code_image[idx].type = is_jmp? JMP_PARAM : DIRECT;
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

void print_head_code_bin(head_ptr_t arr)
{
    int size = arr->codeUsed;
    int i;

    printf("code bin\n");
    for (i = 0; i < size; i++)
    {
        printf("[%d]: line = %d, is_extern = %d, to_decode = %d ", i, arr->code_image[i].line,
            arr->code_image[i].isExtern,
            arr->code_image[i].toDecode);

        binary_ptr_t bin = arr->code_image[i].bin;
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
    printf("data size = %d\n", arr->dataSize);
    int size = arr->dataUsed;
    int i;

        for (i = 0; i < size; i++)
        {
            printf("[%d]: line = %d", 
            i,
            arr->data_image[i].line);
            print_single_data(arr->data_image[i].bin->data_ptr);
            printf("\n");          
        } 
}
