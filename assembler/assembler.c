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

void process_file(char* filename)
{
    char *updated_filename;
    head headPointer;
    printf("hello world2\n");

    updated_filename = str_with_ext(filename, ".as");

    macro_remove(updated_filename, filename);

    head_init(&headPointer, 1, 1, 1);

    free(updated_filename);
    updated_filename = str_with_ext(filename, ".am");

    process_first_pass(&headPointer, updated_filename);

    print_head_code_bin(&headPointer);

}
int main()
{

    char* filename = "prog2";
    printf("hello world");

    process_file(filename);

    return 0;
}
