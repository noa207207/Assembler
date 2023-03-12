#ifndef SECOND_PASS_H
#define SECOND_PASS_H

bool insert_entry(head_ptr_t headPtr, char* label, char* line, int lineNumber);
bool process_second_pass(head_ptr_t headPtr, char* filename);
int find_symbol_value(head_ptr_t headPtr, char* symbol);
bool update_code_symbols(head_ptr_t headPtr);
bool find_symbol_is_extern(head_ptr_t headPtr, char* symbol);
#endif
