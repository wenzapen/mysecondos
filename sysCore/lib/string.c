#include <string.h>

size_t strlen(const char* str) {
    size_t len = 0;
    while(str[len++]);
    return len;

}

void *memset(void *dest, char val, size_t count) {
    unsigned char *temp = (unsigned char *)dest;
    for(; count != 0; count--, temp[count] = val);
    return dest;

}

int strcmp(const char* str1, const char* str2) {
    const unsigned char *s1 = str1;
    const unsigned char *s2 = str2;
    unsigned char c1, c2;
    do {
        c1 = *s1++;
        c2 = *s2++;
        if(c1 == '\0')
            return c1 - c2;
    } while(c1 == c2);
    return c1 - c2;

}
