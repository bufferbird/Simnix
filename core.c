#include <stdint.h>


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

static void vga_init(){

}





void k_main(){

}
