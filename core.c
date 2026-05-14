#include <stdint.h>
#include "src/kprintf.h"
#include "arch/hardwarepi3.h"



#define VERSION 1.2

extern uint64_t get_main_id(void);
extern uint64_t get_timer_freq(void);
extern uint64_t get_main_features(void);
extern uint64_t get_sp(void);

static void get_sys_info__(){
    uint64_t cpu_id = get_main_id(); 
    kprintf("[ OK ] ID: %x", cpu_id);
    uint64_t timer_hz = get_timer_freq(); 
    kprintf("[ OK ] Timer frequency: %x", timer_hz);   
    uint64_t features = get_main_features(); 
    kprintf("[ OK ] Main Features: %d", features); 
    uint64_t current_sp = get_sp();
    kprintf("[ OK ] Current Stack Pointer: %x", current_sp); 
}


static alignas(16) uint32_t mailbox[36];

uint32_t* fb_ptr = 0;

static void vga_init() {
    mailbox[0] = 35 * 4;        
    mailbox[1] = 0;            

    mailbox[2] = 0x00048003;   
    mailbox[3] = 8;           
    mailbox[4] = 8;            
    mailbox[5] = 1024;        
    mailbox[6] = 768;          

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

    mailbox[21] = 0;            // End Tag
    while (*(volatile uint32_t*)MBOX_STATUS & 0x80000000);
    *(volatile uint32_t*)MBOX_WRITE = ((uint32_t)(uintptr_t)mailbox & ~0xF) | 8;


    while (1) {
        while (*(volatile uint32_t*)MBOX_STATUS & 0x40000000); 
        uint32_t response = *(volatile uint32_t*)MBOX_READ;
        if ((response & 0xF) == 8) break;
    }


    if (mailbox[1] == 0x80000000 && mailbox[20] != 0) {
        fb_ptr = (uint32_t*)(uintptr_t)(mailbox[20] & 0x3FFFFFFF);
    } else {
        fb_ptr = 0; 
    }
}



static void initscreen_term(){
    if (!fb_ptr){
        volatile __asm__("wfe");
    }
    kclear_screen(0x00008B); 
    kprintf("Simnix - v1.2, Unstable");
    kprintf("\n");
    if (passw)
    {
        kprintf("[user@simnix]~$"); 
    }
    else
    {
        kprintf("Simnix Login: "); 
    }
}


void k_main(){
    vga_init();
    get_sys_info__(); 
    initscreen_term(); 
}
