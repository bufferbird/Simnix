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

