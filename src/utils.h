#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>

void remove_newline(char *str);
char get_action(void);
int read_int_safe(const char* prompt, int min, int max);
void print_current_time(void);
FILE* open_file_for_reading(const char* filename);
FILE* open_file_for_writing(const char* filename);

#endif