#include "src/init/init.h"
#include "src/hardware/hardwarepi3.h"
#include "src/stdDatatypes.h"

__init_main__(){
  uart_init();
  vga_init();
}

