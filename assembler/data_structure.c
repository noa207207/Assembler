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

char* format_integer(int n) {
    /* Create a char array with space for 5 characters */
    char* str = (char*) malloc(5 * sizeof(char));
    sprintf(str, "%04d", n);
    return str;
}

void data_to_binary(image_ptr_t img, int num_data_lines, char** binary_str)
{
    int i;
    char line_num[7];
    char pattern[15];

    *binary_str = malloc((num_data_lines * 22) + 1);
    memset(*binary_str, 0, (num_data_lines * 22) + 1); /* initialize binary_str with null bytes */

     for (i = 0; i < num_data_lines; i++) {
        sprintf(line_num, "%04d\t\t", get_image_line(img, i));
        strcat(*binary_str, line_num);
        binary_to_pattern(get_single_data_value(img, i), pattern);
        strcat(*binary_str, pattern);
        strcat(*binary_str, "\n");
     }
}

void instructions_to_binary(image_ptr_t img, int num_instructions, char** binary_str) {
    base_instruction_ptr_t base_inst;
    immidiate_instruction_ptr_t imm_inst;
    direct_instruction_ptr_t dir_inst;
    register_instruction_ptr_t reg_inst;
    unsigned int binary;
    char pattern[15];
    char line_num[7];

    *binary_str = malloc((num_instructions * 22) + 1);
    memset(*binary_str, 0, (num_instructions * 22) + 1); /* initialize binary_str with null bytes */
    for (int i = 0; i < num_instructions; i++) {
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
        break;
    }
}

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

void set_direct_era(head_ptr_t h, int idx, attributes attr)
{
    set_direct_instruction_era(h->code_image[idx].bin->direct_ptr, attr);
}

/* Setter function for the 'table' field */
void set_table(head_ptr_t h, symbol_ptr_t table) {
    h->table = table;
}

/* Getter function for the 'table' field */
symbol_ptr_t get_table(head_ptr_t h) {
    return h->table;
}

/* Getter function for the 'data_image' field */
image_ptr_t get_data_image(head_ptr_t h) {
    return h->data_image;
}

/* Setter function for the 'data_image' field */
void set_data_image(head_ptr_t h, image_ptr_t data_image) {
    h->data_image = data_image;
}

/* Getter function for the 'code_image' field */
image_ptr_t get_code_image(head_ptr_t h) {
    return h->code_image;
}

/* Setter function for the 'code_image' field */
void set_code_image(head_ptr_t h, image_ptr_t code_image) {
    h->code_image = code_image;
}

/* Getter function for the 'tableUsed' field */
int get_table_used(head_ptr_t h) {
    return h->tableUsed;
}

/* Setter function for the 'tableUsed' field */
void set_table_used(head_ptr_t h, int tableUsed) {
    h->tableUsed = tableUsed;
}

/* Getter function for the 'tableSize' field */
int get_table_size(head_ptr_t h) {
    return h->tableSize;
}

/* Setter function for the 'tableSize' field */
void set_table_size(head_ptr_t h, int tableSize) {
    h->tableSize = tableSize;
}

/* Getter function for the 'dataUsed' field */
int get_data_used(head_ptr_t h) {
    return h->dataUsed;
}

/* Setter function for the 'dataUsed' field */
void set_data_used(head_ptr_t h, int dataUsed) {
    h->dataUsed = dataUsed;
}

/* Getter function for the 'dataSize' field */
int get_data_size(head_ptr_t h) {
    return h->dataSize;
}

/* Setter function for the 'dataSize' field */
void set_data_size(head_ptr_t h, int dataSize) {
    h->dataSize = dataSize;
}

/* Getter function for the 'codeUsed' field */
int get_code_used(head_ptr_t h) {
    return h->codeUsed;
}

/* Setter function for the 'codeUsed' field */
void set_code_used(head_ptr_t h, int codeUsed) {
    h->codeUsed = codeUsed;
}

/* Getter function for the 'codeSize' field */
int get_code_size(head_ptr_t h) {
    return h->codeSize;
}

/* Setter function for the 'codeSize' field */
void set_code_size(head_ptr_t h, int codeSize) {
    h->codeSize = codeSize;
}

/* Getter and Setter for Symbol structure */

/* Getter function to access symbol_name field */
char* get_symbol_name(head_ptr_t head, int idx) {
    return head->table[idx].symbol_name;
}

/* Setter function to update symbol_name field */
void set_symbol_name(head_ptr_t head, int idx, char* name) {
    strcpy(head->table[idx].symbol_name, name);
}

/* Getter function to access attributes field */
char* get_symbol_attributes(head_ptr_t head, int idx) {
    return head->table[idx].atrributes;
}

/* Setter function to update attributes field */
void set_symbol_attributes(head_ptr_t head, int idx, char* attributes) {
    strcpy(head->table[idx].atrributes, attributes);
}

/* Getter function to access value field */
int get_symbol_value(head_ptr_t head, int idx) {
    return head->table[idx].value;
}

/* Setter function to update value field */
void set_symbol_value(head_ptr_t head, int idx, int value) {
    head->table[idx].value = value;
}

/* Getter function to access isExternal field */
bool get_symbol_isExternal(head_ptr_t head, int idx) {
    return head->table[idx].isExternal;
}

/* Setter function to update isExternal field */
void set_symbol_isExternal(head_ptr_t head, int idx, bool isExternal) {
    head->table[idx].isExternal = isExternal;
}

/* Getter function to access isCode field */
bool get_symbol_isCode(head_ptr_t head, int idx) {
    return head->table[idx].isCode;
}

/* Setter function to update isCode field */
void set_symbol_isCode(head_ptr_t head, int idx, bool isCode) {
    head->table[idx].isCode = isCode;
}

/* Getter function to access isData field */
bool get_symbol_isData(head_ptr_t head, int idx) {
    return head->table[idx].isData;
}

/* Setter function to update isData field */
void set_symbol_isData(head_ptr_t head, int idx, bool isData) {
    head->table[idx].isData = isData;
}

/* Getter function to access isEntry field */
bool get_symbol_isEntry(head_ptr_t head, int idx) {
    return head->table[idx].isEntry;
}

/* Setter function to update isEntry field */
void set_symbol_isEntry(head_ptr_t head, int idx, bool isEntry) {
    head->table[idx].isEntry = isEntry;
}


/* Getter and Setter for Image structure */

/* Getter function to access line field */
int get_data_line(head_ptr_t head, int idx) {
    return head->data_image[idx].line;
}

/* Setter function to update line field */
void set_data_line(head_ptr_t head, int idx, int line) {
    head->data_image[idx].line = line;
}

/* Getter function to access label field */
char* get_data_label(head_ptr_t head, int idx) {
    return head->data_image[idx].label;
}

/* Setter function to update label field */
void set_data_label(head_ptr_t head, int idx, char* label) {
    strcpy(head->data_image[idx].label, label);
}

/* Getter function to access toDecode field */
int get_data_toDecode(head_ptr_t head, int idx) {
    return head->data_image[idx].toDecode;
}

/* Setter function to update toDecode field */
void set_data_toDecode(head_ptr_t head, int idx, bool toDecode) {
    head->data_image[idx].toDecode = toDecode;
}

/* Getter function to access isExtern field */
bool get_data_isExtern(head_ptr_t head, int idx) {
    return head->data_image[idx].isExtern;
}

/* Setter function to update isExtern field */
void set_data_isExtern(head_ptr_t head, int idx, bool isExtern) {
    head->data_image[idx].isExtern = isExtern;
}

/* Getter function to access type field */
addr_method get_data_type(head_ptr_t head, int idx) {
    return head->data_image[idx].type;
}

/* Setter function to update type field */
void set_data_type(head_ptr_t head, int idx, addr_method type) {
    head->data_image[idx].type = type;
}

/* Getter and Setter for code_image */
/* Getter function to access line field */
int get_code_line(head_ptr_t head, int idx) {
    return head->code_image[idx].line;
}

/* Setter function to update line field */
void set_code_line(head_ptr_t head, int idx, int line) {
    head->code_image[idx].line = line;
}

/* Getter function to access label field */
char* get_code_label(head_ptr_t head, int idx) {
    return head->code_image[idx].label;
}

/* Setter function to update label field */
void set_code_label(head_ptr_t head, int idx, char* label) {
    strcpy(head->code_image[idx].label, label);
}

/* Getter function to access toDecode field */
int get_code_toDecode(head_ptr_t head, int idx) {
    return head->code_image[idx].toDecode;
}

/* Setter function to update toDecode field */
void set_code_toDecode(head_ptr_t head, int idx, int toDecode) {
    head->code_image[idx].toDecode = toDecode;
}

/* Getter function to access isExtern field */
bool get_code_isExtern(head_ptr_t head, int idx) {
    return head->code_image[idx].isExtern;
}

/* Setter function to update isExtern field */
void set_code_isExtern(head_ptr_t head, int idx, bool isExtern) {
    head->code_image[idx].isExtern = isExtern;
}

/* Getter function to access type field */
addr_method get_code_type(head_ptr_t head, int idx) {
    return head->code_image[idx].type;
}

/* Setter function to update type field */
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




/* Initialization of symbol node. */
void symbol_init(symbol_ptr_t node) {
    node->isExternal = False;
    node->isCode = False;
    node->isData = False;
    node->isEntry = False;
}

/* SYMBOLS */

void tmp_insert(head_ptr_t arr, char* name, int value, int op)
{
    printf("bla\n");
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

    printf("name = %s, val = %d\n", name, value);

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

    /* printf("name = %s, val = %s\n", name, value); */

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


int resize_img_arr(head_ptr_t arr)
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

    printf("Code Bin size = %d\n", size);
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