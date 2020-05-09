#ifndef _STRING_H
#define _STRING_H

#include <size_t.h>

size_t strlen(const char* str);

void *memset(void *dest, char val, size_t count);

int strcmp(const char* str1, const char* str2);


#endif
