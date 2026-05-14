#include <stdint.h>
#include <stdarg.h>

void kprintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char* s;
    int64_t n;        // Auf 64-Bit erweitert für %d
    uint64_t val;     // Auf 64-Bit erweitert für %x
    int nibble;
    char buf[1024];
    int i;
    const char* p;

    for (p = fmt; *p != '\0'; p++) {
        if (*p == '%') {
            p++;
            switch (*p) {
                case 's':
                    s = va_arg(args, char*);
                    if (s == 0) s = "(null)";
                    while (*s) kputc(*s++);
                    break;

                case 'c':
                    kputc((char)va_arg(args, int));
                    break;

                case 'd':
                    // va_arg holt jetzt die vollen 64 Bit eines int64_t
                    n = va_arg(args, int64_t);
                    if (n == 0) {
                        kputc('0');
                    } else {
                        if (n < 0) {
                            kputc('-');
                            n = -n;
                        }
                        i = 0;
                        while (n > 0) {
                            buf[i++] = (n % 10) + '0';
                            n /= 10;
                        }
                        while (i--) kputc(buf[i]);
                    }
                    break;

                case 'x':
                    // va_arg holt jetzt die vollen 64 Bit eines uint64_t
                    val = va_arg(args, uint64_t);
                    // Schleife läuft nun von Bit 60 runter zu 0 (16 Nibbles für 64 Bit)
                    for (i = 60; i >= 0; i -= 4) {
                        nibble = (val >> i) & 0xF;
                        kputc(nibble < 10 ? nibble + '0' : nibble - 10 + 'A');
                    }
                    break;

                case '%':
                    kputc('%');
                    break;

                default:
                    kputc('?');
                    break;
            }
        } else {
            kputc(*p);
        }
    }
    va_end(args);
}
