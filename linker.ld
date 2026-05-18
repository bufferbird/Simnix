/*
  Copyright (c) by bufferbird
  License: GPL, see: LICENSE. 
*/

ENTRY(_start)

SECTIONS
{
    #ifdef ARCH_X86_64
    . = 0x100000;
    #else
    . = 0x80000;
    #endif

    .init : {
        __init_start = .;
        
        #ifdef ARCH_X86_64
        KEEP(*(.multiboot))    
        #endif
        
        KEEP(*(.text.boot))   
        *(.init.text)         
        *(.init.data)        
        . = ALIGN(8);
        __init_end = .;      
    }

    .text : {
        *(.text)
        . = ALIGN(8);
    }

    .rodata : {
        *(.rodata)
        . = ALIGN(8);
    }

    .data : {
        *(.data)
        . = ALIGN(8);
    }

    .bss (NOLOAD) : {
        . = ALIGN(16);
        __bss_start = .;
        *(.bss)
        *(COMMON)
        __bss_end = .;
    }
}

