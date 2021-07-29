#ifndef _STRING_H_
#define _STRING_H_

#include <stddef.h>

void itoa(int n, char str[]);
void backspace(char s[]);
void append(char s[], char n);
int strcmp(char s1[], char s2[]);


void strrev(char* s);
size_t strlen(const char* s);

#endif
