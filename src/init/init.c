#include "src/hardware/hardwarepi3.h"


void uart_init() {
    *UART0_CR = 0; // UART0 kurz ausschalten zum Konfigurieren

    // GPIO 14 & 15 auf UART0 umstellen (Alt Function 0)
    *GPFSEL1 &= ~((7 << 12) | (7 << 15)); // Bits löschen
    *GPFSEL1 |= (4 << 12) | (4 << 15);    // Bits auf 100 setzen (Alt0)

    *GPPUD = 0; // Pull-up/down deaktivieren
    for(int i=0; i<150; i++) __asm__("nop");
    *GPPUDCLK0 = (1 << 14) | (1 << 15);
    for(int i=0; i<150; i++) __asm__("nop");
    *GPPUDCLK0 = 0;

    *UART0_ICR = 0x7FF; // Alle Interrupts löschen
    *UART0_IBRD = 26;   // Baudrate auf 115200 (bei 48MHz)
    *UART0_FBR = 3;
    *UART0_LCRH = (3 << 5); // 8-bit Modus, kein Parity
    *UART0_CR = 0x301;  // UART wieder an (RX, TX, Enable)
}
