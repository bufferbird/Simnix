#include "src/simnixIO/font.h"
#include "src/simnixIO/kstdio.h"
#include "src/init/init.h"
#include "src/kernel/kernelfunc.h"
#include "src/kernel/interrupttable.h"
#include "srrc/kernel/syscalls/syscalls.h"
#include "src/hardware/hardwarepi3.h"

#ifndef MAILBOX_READ
#define MAILBOX_READ 0x3F00B880
#endif

#ifndef MAILBOX_STATUS
#define MAILBOX_STATUS 0x3F00B898
#endif

#ifndef MAILBOX_WRITE
#define MAILBOX_WRITE 0x3F00B8A0
#endif

/* global ptr so it exists even after vga_init */

uint32_t* fb_ptr = 0;
__attribute__((aligned(16))) uint32_t mailbox[32];


static void __initscreen(){
  kclear_screen(0x00000000);
  kprint("Kernel - Simnix v3.6");
  kprint("Distribution - Saumix");
  kprint("Ready for input...");


}

static void vga_init(void){
    mailbox[0] = 35 * 4;  
    mailbox[1] = 0;        

    mailbox[2] = 0x00048003; 
    mailbox[3] = 8;
    mailbox[4] = 8;
    mailbox[5] = 1024; // Breite
    mailbox[6] = 768;  // Höhe

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



void k_main(){
  if ( fb_ptr != 0){
    vga_init();
    kprint("[OK] Successfully loaded Framebuffer.");
  }
  else
  {
    __asm__("wfe");
  }
  
  while (1){
    __asm__("wfe");
}
