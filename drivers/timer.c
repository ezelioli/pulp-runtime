#include "pulp.h"
#include "stdint.h"
#include <stdio.h>

void timer_setup(uint32_t cmp_val){

	uint32_t base_addr = timer_base_fc(0,0);

	uint32_t timer_cfg = 0x00000094;

	timer_conf_set(base_addr, timer_cfg);

	timer_cmp_set(base_addr, cmp_val);

	timer_reset(base_addr);

}

void timer_start_timer(){

	unsigned int base_addr = timer_base_fc(0,0);

	timer_start(base_addr);
}

void timer_stop_timer(){

	unsigned int base_addr = timer_base_fc(0,0);

	uint32_t timer_cfg = timer_conf_get(base_addr);
	// printf("timer running: %d\n", timer_cfg);
	timer_cfg &= ~(0x1);
	// printf("timer stopped: %d (&= %d)\n", timer_cfg, ~(0x1));
	timer_conf_set(base_addr, timer_cfg);
}