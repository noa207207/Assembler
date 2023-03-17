#ifndef SECOND_PASS_H
#define SECOND_PASS_H

/* Second pass encode instructions to binary uniq format and handle Entries. 
 * If success, updates the head_ptr
 * Return: Success (T) / Fail (F) */
bool process_second_pass(head_ptr_t headPtr, char* filename);

/* Searches the symbol table for the label.
 * Check if is there a duplicate external label (error)
 * Add the entry attribute to it. 
 * Return: Success (T) / Fail (F) */
bool insert_entry(head_ptr_t headPtr, char* label, char* line, int lineNumber);

/* Updates the code image with symobls values.
 * Return: Success (T) / Fail (F) */
bool update_code_symbols(head_ptr_t headPtr);

/* Returns value of symbol. 
 * In case symbol doesn't exist in symbol table, returns -1. */
int find_symbol_value(head_ptr_t headPtr, char* symbol);

/* Return: T / F */
bool is_symbol_extern(head_ptr_t headPtr, char* symbol);


#endif
