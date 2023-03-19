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

    EXISTENT_FILE(updated_filename)

    PREASSEMBLER(updated_filename, filename)

    headPointer = head_init(1, 1, 1);

    free(updated_filename);
    updated_filename = str_with_ext(filename, ".am");

    printf("Assembler is processing file \"%s\". Beginning with first pass..\n", updated_filename);
    if (process_first_pass(headPointer, updated_filename)) {
        printf("Errors found in file \"%s\" during first pass!\n\n", updated_filename);
        ERROR_FREE_AND_REMOVE(updated_filename, headPointer)
        return;
    }

    printf("First pass finished with no errors.\nContinuing with second pass..\n");

    if (process_second_pass(headPointer, updated_filename)) {
        printf("Errors found in file \"%s\" during second pass!\n\n", updated_filename);
        ERROR_FREE_AND_REMOVE(updated_filename, headPointer)
        return;
    }

    printf("Second pass finished with no errors.\nCreating output files..\n");

    create_output(headPointer, filename);

    printf("Output files successfully created for file \"%s\"!\n\n", updated_filename);

    print_head_code_bin(headPointer);

    print_symbols(headPointer);

    print_data(headPointer);

    free(updated_filename);
    free_head(headPointer);

}
int main()
{
    char* filename = "test1";

    process_file(filename);

    return 0;
}
