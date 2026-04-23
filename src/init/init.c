#include "src/hardware/hardwarepi3.h"


void uart_init() {
    *UART0_CR = 0; 

  
    *GPFSEL1 &= ~((7 << 12) | (7 << 15));
    *GPFSEL1 |= (4 << 12) | (4 << 15);    

    *GPPUD = 0; 
    for(int i=0; i<150; i++) __asm__("nop");
    *GPPUDCLK0 = (1 << 14) | (1 << 15);
    for(int i=0; i<150; i++) __asm__("nop");
    *GPPUDCLK0 = 0;

    *UART0_ICR = 0x7FF; // delete all interrupts
    *UART0_IBRD = 26;   
    *UART0_FBR = 3;
    *UART0_LCRH = (3 << 5); 
    *UART0_CR = 0x301;  
}
