/* 
 *Copyright (c) 2026 by bufferbird
 * License - see LICENSE
*/

#include "src/simnixIO/font.h"
#include "src/simnixIO/kstdio.h"
#include "src/init/init.h"
#include "src/kernel/kernelfunc.h"
#include "src/kernel/interrupttable.h"
#include "src/kernel/syscalls/syscalls.h"
#include "src/hardware/hardwarepi3.h"

/* I write this, if it isn't declared in hardware. */ 

#ifndef MAILBOX_READ
#define MAILBOX_READ 0x3F00B880
#endif

#ifndef MAILBOX_STATUS
#define MAILBOX_STATUS 0x3F00B898
#endif

#ifndef MAILBOX_WRITE
#define MAILBOX_WRITE 0x3F00B8A0
#endif

/* global ptr so it exists also after vga_init */

uint32_t* fb_ptr = 0;
__attribute__((aligned(16))) uint32_t mailbox[32];


static void __initscreen__(){
  kclear_screen(0x00000000);
  kprint("Kernel - Simnix v3.6");
  kprint("Distribution - Saumix");
  kprint("Ready for input...");


}

static void vga_init(void){
    /* Yes. we have to do this. */
    mailbox[0] = 35 * 4;  
    mailbox[1] = 0;        

    mailbox[2] = 0x00048003; 
    mailbox[3] = 8;
    mailbox[4] = 8;
    mailbox[5] = 1024; // width
    mailbox[6] = 768;  // height

    mailbox[7] = 0x00048004;
    mailbox[8] = 8;
    mailbox[9] = 8;
    mailbox[10] = 1024;
    mailbox[11] = 768;

    mailbox[12] = 0x00048005;
    mailbox[13] = 4;
    mailbox[14] = 4;
    mailbox[15] = 32;

    mailbox[16] = 0x00040001;
    mailbox[17] = 8;
    mailbox[18] = 8;
    mailbox[19] = 4096;
    mailbox[20] = 0; 

    mailbox[21] = 0; 

    // Send
    while (*(volatile uint32_t*)MAILBOX_STATUS & 0x80000000);
    *(volatile uint32_t*)MAILBOX_WRITE = ((uint32_t)(uintptr_t)mailbox & ~0xF) | 8;

    // Wait
    while (1) {
        while (*(volatile uint32_t*)MAILBOX_STATUS & 0x40000000);
        uint32_t response = *(volatile uint32_t*)MAILBOX_READ;
        if ((response & 0xF) == 8) break;
    }


    fb_ptr = (uint32_t*)(uintptr_t)(mailbox[20] & 0x3FFFFFFF);
}

/*
 * Sooo, now we have the Pointer, and we can do funny things with it. 
 * But let's init UART first, and then wie can use the beautyful 
 * __initscreen__ to use fbr_ptr thee very first time. 
*/

void k_main(void){
  uart_init(); 
  vga_init(); 
  
  if (fb_ptr != 0){
    __initscreen__(); 
    kprint("[OK] Successfully loaded Framebuffer.");
  }
  else {
    kprint("[fb_ptr] Framebuffer failed."); 
    __asm__("wfe");
  }
  
  kprint("[OK] Loaded UART successfully.");
  
  while (1){
    __asm__("wfe");
  }
}

