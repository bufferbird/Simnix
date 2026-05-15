#include "src/init/init.h"
#include "src/kprintf.h"
#include "src/utils.h"
#include "arch/aarch64/RaspberryPi3/hardwarepi3.h"

void uart_init() {
    *UART0_CR = 0; 
     kprintf("[ OK ] Disabling UART0 Interrupts...\n\r"); 

    *GPFSEL1 &= ~((7 << 12) | (7 << 15));
    *GPFSEL1 |= (4 << 12) | (4 << 15);    

    *GPPUD = 0; 
    for(int i=0; i<150; i++) __asm__("nop");
    *GPPUDCLK0 = (1 << 14) | (1 << 15);
    for(int i=0; i<150; i++) __asm__("nop");
    *GPPUDCLK0 = 0;

    *UART0_ICR = 0x7FF; 
    *UART0_IBRD = 26;   
    *UART0_FBRD = 3;
   kprintf("[UART] Finishing UART0 initializing...\r\n"); 
    *UART0_LCRH = (3 << 5);
    *UART0_CR = 0x301;  
   kprintf("[ OK ] Finished! Enabled UART Interrupts!\r\n"); 
}
