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
    printf("hello world2\n");



    updated_filename = str_with_ext(filename, ".as");

    macro_remove(updated_filename, filename);
}
int main()
{

    char* filename = "prog";
    printf("hello world");

    process_file("prog");

    return 0;
}
