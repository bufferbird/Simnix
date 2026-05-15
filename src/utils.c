#include "src/utils.h"
#include <stddef>

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

char* strcpy(char* dest, const char* src) {
    char* start = dest; 
    while (*src) {
        *dest = *src;  
        dest++;       
        src++;         
    }
    *dest = '\0';       
    return start;     
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}
