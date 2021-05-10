#ifndef __HAL_UDMA_UDMA_DVSI_V1_H__
#define __HAL_UDMA_UDMA_DVSI_V1_H__

#include "archi/udma/dvsi/udma_dvsi_v1.h"
#include "stdint.h"

#define UDMA_DVSI_OFFSET 0x300

#define DVSI_FROM_TIMER      0X01
#define DVSI_FROM_REGISTER   0x00

typedef struct {
  uint8_t   frame_req_src;
  uint32_t  buffer_addr;
} dvsi_cfg_t;

typedef struct {
  uint8_t on_off_data;
  uint16_t xy_addr;
} dvsi_event_t;

static inline void plp_dvsi_setup();
static inline void plp_dvsi_disable();
//static inline void plp_dvsi_start();

static inline void plp_dvsi_setup(dvsi_cfg_t* config)
{

  uint32_t frame_req_src;
  uint32_t cfg_glob_reg;
  
  frame_req_src = 0x01 & config->frame_req_src;
  cfg_glob_reg = 0x00010136 | (frame_req_src << 19);

  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_CONTROL_OFFSET          , 0xFFFFFFFF);
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_DST_OFFSET              , 0x000000FF); //0x00
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_CROP_TOP_BOTTOM_OFFSET  , 0x00000000); // todo
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_CROP_LEFT_RIGHT_OFFSET  , 0x00000000); // todo
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_CFG_XYRES_OFFSET        , 0x00004D1D);
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_CFG_TIMER_OFFSET        , 0x08FF008F);
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_L2_START_ADDR_OFFSET    , config->buffer_addr); // write here a pointer to the buffer where you want to read
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_L2_ADDR_STEP_OFFSET     , 0x00000004);
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_TRIGGER_THRESHOLD_OFFSET, 0x000000FF);
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_TRIGGER_TDELTA_OFFSET   , 0x0000FFFF);
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_INT_MODE_OFFSET         , 0x00000002);
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_BIAS0_OFFSET            , 100000    );
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_BIAS1_OFFSET            , 0x00000000);
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_BIAS2_OFFSET            , 0x00000000);
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_BIAS3_OFFSET            , 10000);
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_BIAS4_OFFSET            , 400);
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_BIAS5_OFFSET            , 10000);
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_BIAS6_OFFSET            , 100000);
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_BIAS7_OFFSET            , 100000);
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_BIAS8_OFFSET            , 100000);
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_BIAS9_OFFSET            , 10000);
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_BIAS10_OFFSET           , 0x00000000);
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_PARAMS_OFFSET           , 0x00000002);
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_FB_CFG0_OFFSET          , 0x00000000);  // todo
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_FB_CFG1_OFFSET          , 0x00000000);  // todo

  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_CFG_GLOB_OFFSET         , cfg_glob_reg); // 0x00010196 -> set power (16) to 0 

}

static inline void plp_dvsi_disable(){
  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_CFG_GLOB_OFFSET, 0x00000000); // todo
}

//static inline void plp_dvsi_start(){
//  pulp_write32(ARCHI_UDMA_ADDR + UDMA_DVSI_OFFSET + DVSI_CFG_GLOB_OFFSET, 0x00000000);
//}

#endif