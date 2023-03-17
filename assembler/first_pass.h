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

/* Identifies the type of instruction and handles it accordingly. 
 * Return true in case errors found */
bool process_first_pass(head_ptr_t, char*);

/* Checks if there's a label in line. If so, it copies it into label, 
 * and returns the index of the character after ':'. Otherwise returns -1. */
int label_check(char*, char*);

/* Returns specific opcode / string / data / entry / extern */
opcode firstWord(char*);

#endif
