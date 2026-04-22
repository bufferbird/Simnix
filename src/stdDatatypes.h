#ifndef STDDATATYPES_H
#define STDDATATYPES_H

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

typedef char               int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;

#ifndef __cplusplus
  #ifndef bool
    typedef int bool;
  #endif
  #ifndef true
    #define true 1
  #endif
  #ifndef false
    #define false 0
  #endif
#endif

#define __init __attribute__((section(".init.text")))


typedef volatile uint32_t reg32;

#endif
