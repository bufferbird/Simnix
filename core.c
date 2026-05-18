/*
  Copyright (c) by bufferbird
  License: GPL, see: LICENSE. 
*/

#include <stdint.h>
#include "src/kprintf.h"
#include "src/input.h"
#include "src/utils.h"
#include <stdalign.h>
#include <stddef.h>
#include "src/init/init.h"

#ifdef ARCH_AARCH64
#include "arch/aarch64/RaspberryPi3/hardwarepi3.h"
#endif

uint32_t* fb_ptr = 0;
#define VERSION 1.2


extern uint64_t get_main_id(void);
extern uint64_t get_timer_freq(void);
extern uint64_t get_main_features(void);
extern uint64_t get_sp(void);

/*  Helpful function for CPU Deepsleep, so we dont need to write 
*   __asm__ volatile({sleep opcode}) every time, with the 
*   special opcode of each arch, like hlt, cli or wfi. 
*/

static inline void cpu_halt(void) {
    #ifdef ARCH_X86_64
    __asm__ volatile("hlt");
    #elif defined(ARCH_AARCH64)
    __asm__ volatile("wfi");
    #endif
}

static void get_sys_info__(){
    uint64_t cpu_id = get_main_id(); 
    kprintf("[ OK ] ID: %x\n\r", cpu_id);
    uint64_t timer_hz = get_timer_freq(); 
    kprintf("[ OK ] Timer frequency: %x\n\r", timer_hz);   
    uint64_t features = get_main_features(); 
    kprintf("[ OK ] Main Features: %d\n\r", features); 
    uint64_t current_sp = get_sp();
    kprintf("[ OK ] Current Stack Pointer: 0x%xn\r", current_sp); 
    #ifdef ARCH_X86_64
    kprintf("[ OK ] Architecture: x86_64 (PC Mode)\n\r");
    #elif defined(ARCH_AARCH64)
    kprintf("[SYS] Detected AArch64! ");
    #else
    kprintf("Unknown Architecture??");
    #endif

    kprintf("[ OK ] Checking fb_ptr...\r\n");
    kprintf("[fb] fb_ptr at 0x%x\n\r", fb_ptr); 
    if (fb_ptr != 0){
        kprintf("[fbptr] fbptr != 0: 0x%x\r\n", fb_ptr); 
        kprintf("Ready and Waiting :-)"); 
    }
}

void draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) {
    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            draw_pixel(x + j, y + i, color);
        }
    }
}

#ifdef ARCH_AARCH64
#define MBOX_REQUEST    0
#define SET_PHYS_RES    0x00048003
#define SET_VIRT_RES    0x00048004
#define SET_DEPTH       0x00048005
#define ALLOC_BUFFER    0x00040001
#define TAG_END         0x00000000

static alignas(16) uint32_t mailbox[35];
#endif

static void vga_init() {
    #ifdef ARCH_AARCH64
    mailbox[0] = 35 * sizeof(uint32_t); 
    mailbox[1] = MBOX_REQUEST;      

    mailbox[2] = SET_PHYS_RES;   
    mailbox[3] = 8;                   
    mailbox[4] = 8;                  
    mailbox[5] = 1024;     
    mailbox[6] = 768;         

    mailbox[7] = SET_VIRT_RES;   
    mailbox[8] = 8;
    mailbox[9] = 8;
    mailbox[10] = 1024;
    mailbox[11] = 768;

    mailbox[12] = SET_DEPTH;  
    mailbox[13] = 4;
    mailbox[14] = 4;
    mailbox[15] = 32;           

    mailbox[16] = ALLOC_BUFFER;   
    mailbox[17] = 8;                 
    mailbox[18] = 4;                
    mailbox[19] = 16;                   
    mailbox[20] = 0;                  

    mailbox[21] = TAG_END;             

    while (*(volatile uint32_t*)MBOX_STATUS & 0x80000000);
    *(volatile uint32_t*)MBOX_WRITE = ((uint32_t)(uintptr_t)mailbox & ~0xF) | 8;

    while (1) {
        while (*(volatile uint32_t*)MBOX_STATUS & 0x40000000); 
        uint32_t response = *(volatile uint32_t*)MBOX_READ;
        if ((response & 0xF) == 8) break;
    }

    if (mailbox[1] == 0x80000000 && mailbox[19] != 0) {
        fb_ptr = (uint32_t*)(uintptr_t)(mailbox[19] & 0x3FFFFFFF);
    } else {
        fb_ptr = 0; 
        cpu_halt();
    }

    #elif defined(__x86_64__)
    /* 
    * Under x86 i have to read the multiboot header, but for an example, im judt writing 0xFD000000 here, wich is the standard of QEMU VBE
    */
    fb_ptr = (uint32_t*)0xFD000000;
    #endif
}

static void initscreen_term(){
    if (!fb_ptr){
        #ifdef ARCH_AARCH64
        __asm__ volatile("wfe");
        #elif defined(ARCH_X86_64)
        __asm__ volatile("hlt");
        #endif
    }
    kclear_screen(0x00008B); 
    kprintf("Simnix - v1.2, Unstable");
    kprintf("\n");
    kprintf("\r\n"); 
}

void k_main(){
    vga_init();
    initscreen_term(); 
    get_sys_info__(); 
    uart_init();
    
    kprintf("Starting Command Line Interface\r\n"); 
    while (1) {
        char cmd_buffer[64]; 
        k_input("\n\r~$ ", cmd_buffer, 64);
        
        if (strcmp(cmd_buffer, "help") == 0) {
            kprintf("Available: help, clear, version, shutdown -ds\r\n");
        } 
        else if (strcmp(cmd_buffer, "version") == 0) {
            kprintf("Simnix v1.2, Unstable\r\n");
        } 
        else if (strcmp(cmd_buffer, "") == 0) {
        } 
        else if (strcmp(cmd_buffer, "shutdown -ds") == 0){
            kprintf("Simnix wird heruntergefahren...\r\n");
            while(1) {
                cpu_halt();
            }
        }
        else if (strcmp(cmd_buffer, "clear") == 0){
            initscreen_term(); 
        }
        else {
            kprintf("error: Befehl '%s' nicht gefunden.\r\n", cmd_buffer);
        }
    } 
}



