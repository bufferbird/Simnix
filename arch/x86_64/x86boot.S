/*
  Copyright (c) by bufferbird
  License: GPL, see: LICENSE. 
*/

.code64              
.section .text.boot
.global _start

_start:
master_core:
    leaq    _start(%rip), %rsp

    leaq    __bss_start(%rip), %rdi   
    leaq    __bss_end(%rip), %rcx     
    subq    %rdi, %rcx             
    shrq    $3, %rcx     
    
    xorq    %rax, %rax            

bss_loop:
    testq   %rcx, %rcx          
    jz      jump_to_main
    
    movq    %rax, (%rdi)              
    addq    $8, %rdi                 
    decq    %rcx                     
    jmp     bss_loop

jump_to_main:
    call    k_main

park_core:

    cli                              
1:  hlt                             
    jmp     1b                    
