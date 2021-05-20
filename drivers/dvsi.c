#include "pulp.h"

static void dvsi_setup(dvsi_cfg_t *config)
{
  plp_dvsi_setup(config);
}

void dvsi_config_udma(int periph_id) {
  uint32_t udma_cg_cfg = plp_udma_cg_get();
  udma_cg_cfg |= (1 << periph_id);
  plp_udma_cg_set(udma_cg_cfg);
}

void dvsi_config_interrupts(void) {
  // unmask DVSI events in SoC event unit
  //soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_DVSI0_RX);
  soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_DVSI0_SAER_DONE);
  soc_eu_fcEventMask_setEvent(ARCHI_SOC_EVENT_DVSI0_FB_DONE);

  // enable interrupts from SoC event unit in APB interrupt controller
  hal_itc_enable_set(1<<26);
}

void dvsi_wait_evt_id(uint32_t id) {  // id should be ARCHI_SOC_EVENT_DVSI0_SAER_DONE
  hal_itc_wait_for_interrupt();
  uint32_t trigger_event = hal_itc_fifo_pop();
  while(trigger_event != id) {
    hal_itc_wait_for_interrupt();
    trigger_event = hal_itc_fifo_pop();
  }
}

void dvsi_wait_saer(){

  dvsi_wait_evt_id(ARCHI_SOC_EVENT_DVSI0_SAER_DONE);
  
}

void dvsi_wait_fb(){

  dvsi_wait_evt_id(ARCHI_SOC_EVENT_DVSI0_FB_DONE);
  
}

int dvsi_open(dvsi_cfg_t *config)
{

  int periph_id = ARCHI_UDMA_DVSI_ID(0);
  //int channel = UDMA_EVENT_ID(periph_id);

  // configure uDMA
  dvsi_config_udma(periph_id);
  //plp_udma_cg_set(plp_udma_cg_get() | (1<<periph_id));

  // configure dvsi registers
  dvsi_setup(config);

  // configure interrupts
  dvsi_config_interrupts();
  //soc_eu_fcEventMask_setEvent(0); //channel
  //soc_eu_fcEventMask_setEvent(1); //channel+1

  return 0;
}

void dvsi_close()
{
  int periph_id = ARCHI_UDMA_UART_ID(0);
  //int channel = UDMA_EVENT_ID(periph_id);

  plp_dvsi_disable();

  plp_udma_cg_set(plp_udma_cg_get() & ~(1<<periph_id));
}