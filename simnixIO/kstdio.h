/* Copyright (C) bufferbird */

#ifndef KSTDIO_H
#define KSTDIO_H



#include <stdarg.h>


void kputc(char c);


void kprintf(const char* format, ...);

// helpfunction for instant output

void kputs(const char* s);

/**
 * Gives a number as hex
 */

void kprint_hex(unsigned int d);

#endif // KPRINTF_H


#ifndef
