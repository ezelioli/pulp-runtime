#include "pulp.h"

void timer_setup(unsigned int cmp_val){

	unsigned int base_addr = timer_base_fc(0,0);

	timer_conf_set(base_addr, 0x00000094);
	timer_reset(base_addr);
	timer_cmp_set(base_addr, 0x00000020);
}

void timer_start_timer(){ // start timer (preconfigured)... you should wait for interrupts now and receive them every 1ms

	unsigned int base_addr = timer_base_fc(0,0);

	timer_start(base_addr);
} // refactor with timer_timer_t struct or similar