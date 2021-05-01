#include "pulp.h"

static void dvsi_setup(dvsi_cfg_t* config)
{
  plp_dvsi_setup(config);
}

int dvsi_open()
{
  int periph_id = ARCHI_UDMA_DVSI_ID(0);
  //int channel = UDMA_EVENT_ID(periph_id);

  plp_udma_cg_set(plp_udma_cg_get() | (1<<periph_id));

  soc_eu_fcEventMask_setEvent(0); //channel
  soc_eu_fcEventMask_setEvent(1); //channel+1

  dvsi_setup();

  return 0;
}

void dvsi_close()
{
  int periph_id = ARCHI_UDMA_UART_ID(0);
  //int channel = UDMA_EVENT_ID(periph_id);

  plp_dvsi_disable();      

  plp_udma_cg_set(plp_udma_cg_get() & ~(1<<periph_id));
}