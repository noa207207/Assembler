#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structure.h"
#include "errors.h"
#include "parse.h"
#include "utils.h"
#include "constants.h"

#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#define INITIAL_IC 100


int process_first_pass(head_ptr_t, char*);
int label_check(char*, char*);
opcode firstWord(char*);

#endif
