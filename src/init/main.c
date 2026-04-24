#include "src/init/init.h"
#include "src/hardware/hardwarepi3.h"
#include "src/srdDatatypes.h"

__init_main__(){
  uart_init();
  vga_init();
}

