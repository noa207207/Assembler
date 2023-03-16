#include "utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"

/* Joins two strings together. This allocates memory, and needs to be freed after use. */
char* str_with_ext(char* str1, char* ext) {
    char* united_string = (char*)malloc_with_monitor(strlen(str1) + strlen(ext) + 1);
    strcpy(united_string, str1);
    strcat(united_string, ext);
    return united_string;
}

/* Malloc with monitor - if memory allocation is failed, exits the program. */
void* malloc_with_monitor(long size) {
    void* p = malloc(size);
    if (!p) {
        printf("Error: Failed to allocate memory.\n");
        exit(1);
    }
    return p;
}

/* Realloc with monitor - if memory reallocation is failed, exits the program. */
void* realloc_with_monitor(void* start, long size) {
    void* p = realloc(start, size);
    if (!p) {
        printf("Error: Failed to reallocate memory.\n");
        exit(1);
    }
    return p;
}

/* Returns pointer right after spaces and tabs. */
char* skip_spaces(char* str) {
    int i = 0;
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    return str + i;
}

/* Counts length of word (until space-character or NULL character is reached). */
int word_length(char* str) {
    int i = 0;
    for (; !isspace(str[i]) && str[i]; i++)
        ;
    return i;
}

/* Counts length of line (until \n or NULL character is reached). */
int count_line_length(char* p) {
    int i = 0;
    while (p[i] != '\n' && p[i])
        i++;
    return i + 1;
}

/* Returns true if the string received is empty. */
bool empty_string(char* str) {
    char* p = skip_spaces(str);
    if (*p == '\n' || !*p)
        return True;
    return False;
}

/* Returns a pointer right after the current word. */
char* skip_word(char* str) {
    int idx = 0;
    while (!isspace(str[idx]) && str[idx] && str[idx] != ',') {
        idx++;
    }
    return str + idx;
}

#include <stdio.h>
#include <string.h>
#include <ctype.h>

void remove_end_spaces(char *str)
{
    int len = strlen(str);
    int i;

    for (i = len-1; i >= 0 && isspace(str[i]); i--);
    str[i+1] = '\0';
}

void removeSpacesAndTabs(char *str) {
    int len = strlen(str);
    int i, j;

    /* Remove spaces and tabs from the beginning of the string */ 
    for (i = 0; isspace(str[i]); i++);
    if (i > 0) {
        memmove(str, str+i, len-i+1);
        len -= i;
    }

    /* Remove spaces and tabs from the end of the string */ 
    for (j = len-1; j >= 0 && isspace(str[j]); j--);
    str[j+1] = '\0';

    /* Remove spaces and tabs before commas */ 
    for (i = 0; i < len; i++) {
        if (str[i] == ',' && isspace(str[i-1])) {
            for (j = i-1; j >= 0 && isspace(str[j]); j--);
            memmove(str+j+1, str+i, len-i+1);
            len -= i-j-1;
        }
    }

    /* Remove spaces and tabs after commas */ 
    for (i = 0; i < len; i++) {
        if (str[i] == ',' && isspace(str[i+1])) {
            for (j = i+1; j < len && isspace(str[j]); j++);
            memmove(str+i+1, str+j, len-j+1);
            len -= j-i-1;
        }
    }

    /* Ensure there is only one space between words (replace tabs with spaces) */ 
    for (i = 0; i < len-1; i++) {
        if (isspace(str[i]) && (isspace(str[i+1]) || str[i+1] == '\t')) {
            str[i] = ' ';
            memmove(str+i+1, str+i+2, len-i);
            len--;
            i--;
        }
    }
}

/* Deletes the spaces, tabs and newline in a given string. */
void delete_spaces(char* p) {
    int i = 0, count = 0;
    bool str_flag = False;
    while (p[i]) {
        if (p[i] == '\"' && str_flag)
            str_flag = False;
        else if (p[i] == '\"' && !str_flag)
            str_flag = True;

        if (!isspace(p[i]) || (p[i] == ' ' && str_flag))
            p[count++] = p[i];
        i++;
    }
    p[count] = '\0'; /* End the string. */
}

void delete_new_line(char* p) {
    if (p[strlen(p) - 1] == '\n')
        p[strlen(p) - 1] = 0;
}

/* Returns hist, given a number. */
int getHist(int value) {
    return value % 16;
}

/* Returns base, given a number. */
int getBase(int value) {
    return value - getHist(value);
}
