#include "src/kprintf.h"
#include "src/input.h"

void k_input(char prompt[50], char* buffer, int max_size) {
    uint32_t index = 0; 
    kprintf("%s", prompt); 
    while (index < (max_size - 1)) {
        char c = k_getc(); 
        if (c == '\r' || c == '\n') {
            break;
        }
        buffer[index] = c; 
        index++;
        kprintf("%c", c);
    }
    buffer[index] = '\0';
}
