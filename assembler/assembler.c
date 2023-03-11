#include <stdio.h>
#include "assembler.h"
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "data_structure.h"
#include "errors.h"
#include "first_pass.h"
#include "output.h"
#include "pre_assembler.h"
#include "second_pass.h"
#include "utils.h"

static head_ptr_t headPointer;

void process_file(char* filename)
{
    char *updated_filename;
   
    updated_filename = str_with_ext(filename, ".as");

    macro_remove(updated_filename, filename);
    headPointer = head_init(1, 1, 1);

    free(updated_filename);
    updated_filename = str_with_ext(filename, ".am");

    process_first_pass(headPointer, updated_filename);

    print_head_code_bin(headPointer);

    print_symbols(headPointer);

    print_data(headPointer);

}
int main()
{
    char* filename = "prog2";
    printf("hello world\n");

    process_file(filename);

    return 0;
}
