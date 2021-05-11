#ifndef __HAL_UDMA_UDMA_DVSI_V1_H__
#define __HAL_UDMA_UDMA_DVSI_V1_H__

#include "archi/udma/dvsi/udma_dvsi_v1.h"
#include "stdint.h"

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
static inline void plp_dvsi_wait(uint32_t cycles);
static inline void plp_dvsi_trigger_saer_reg();
static inline void plp_dvsi_trigger_fb_reg();

static inline void plp_dvsi_setup(dvsi_cfg_t* config)
{

  uint32_t frame_req_src;
  uint32_t cfg_glob_reg;

  // Configure global register:
  // Bit 0 = 0 - don't trigger frame req
  // Bit 1 = 1 - manual flush mode
  // Bit 2 = 1 - execute flush
  // Bit 3 = 0 - disable test mode
  // Bit 4 = 1 - init L2 Address
  // Bit 5 = 1 - addressing mode: dense-
  // Bit 6 = 1 - use synchronized data from SAER control
  // Bit 7 = 1 - use DVS X/Y address decoder
  // Bits 15:8 = 1 - don't divide clock in SAER control
  // Bit 16 = 0 - don't power up yet
  // Bits 18:17 = 0 - uDMA datasize -> 2 for 4-byte-transfers
  // Bit 19 = 0 - trigger frame_req with config register
  // Bit 20 = 0 - don't trigger framebuffer readout
  // Bit 22:21 = 0/1/2 - trigger source for framebuffer readout
  // Bits 26:23 = 0 - no write padding
  // Bit 27 = 1 - CUTIE trigger enabled
  
  frame_req_src = 0x01 & config->frame_req_src;
  cfg_glob_reg = 0x00010136 | (frame_req_src << 19);

  // bias configuration
  pulp_write32(DVSI_ADDRESS + DVSI_BIAS0_OFFSET            , 100000    );
  pulp_write32(DVSI_ADDRESS + DVSI_BIAS1_OFFSET            , 0x00000000);
  pulp_write32(DVSI_ADDRESS + DVSI_BIAS2_OFFSET            , 0x00000000);
  pulp_write32(DVSI_ADDRESS + DVSI_BIAS3_OFFSET            , 10000);
  pulp_write32(DVSI_ADDRESS + DVSI_BIAS4_OFFSET            , 400);
  pulp_write32(DVSI_ADDRESS + DVSI_BIAS5_OFFSET            , 10000);
  pulp_write32(DVSI_ADDRESS + DVSI_BIAS6_OFFSET            , 100000);
  pulp_write32(DVSI_ADDRESS + DVSI_BIAS7_OFFSET            , 100000);
  pulp_write32(DVSI_ADDRESS + DVSI_BIAS8_OFFSET            , 100000);
  pulp_write32(DVSI_ADDRESS + DVSI_BIAS9_OFFSET            , 10000);
  pulp_write32(DVSI_ADDRESS + DVSI_BIAS10_OFFSET           , 0x00000000);
  pulp_write32(DVSI_ADDRESS + DVSI_PARAMS_OFFSET           , 0x00000002);

  // L2 memory address configuration
  pulp_write32(DVSI_ADDRESS + DVSI_L2_START_ADDR_OFFSET    , config->buffer_addr); // write here a pointer to the buffer where you want to read
  pulp_write32(DVSI_ADDRESS + DVSI_L2_ADDR_STEP_OFFSET     , 0x00000004);

  // config global register and trigger L2 address initialization
  pulp_write32(DVSI_ADDRESS + DVSI_CFG_GLOB_OFFSET         , cfg_glob_reg); // 0x00010196 -> set power (16) to 0 

  // XEND and YEND configuration
  pulp_write32(DVSI_ADDRESS + DVSI_CFG_XYRES_OFFSET        , 0x00004D1D);

  // timer configuration
  pulp_write32(DVSI_ADDRESS + DVSI_CFG_TIMER_OFFSET        , 0x08FF008F);

  pulp_write32(DVSI_ADDRESS + DVSI_TRIGGER_THRESHOLD_OFFSET, 0x000000FF); // 0x0f ?
  pulp_write32(DVSI_ADDRESS + DVSI_TRIGGER_TDELTA_OFFSET   , 0x0000FFFF); // 0x0fff ?
  pulp_write32(DVSI_ADDRESS + DVSI_INT_MODE_OFFSET         , 0x00000002);


  pulp_write32(DVSI_ADDRESS + DVSI_CONTROL_OFFSET          , 0xFFFFFFFF);
  pulp_write32(DVSI_ADDRESS + DVSI_DST_OFFSET              , 0x000000FF); //0x00
  pulp_write32(DVSI_ADDRESS + DVSI_CROP_TOP_BOTTOM_OFFSET  , 0x00000000); // todo
  pulp_write32(DVSI_ADDRESS + DVSI_CROP_LEFT_RIGHT_OFFSET  , 0x00000000); // todo

  pulp_write32(DVSI_ADDRESS + DVSI_FB_CFG0_OFFSET          , 0x00000000);  // todo
  pulp_write32(DVSI_ADDRESS + DVSI_FB_CFG1_OFFSET          , 0x00000000);  // todo

  // stop initializing L2 address
  cfg_glob_reg &= (~(1 << 4));
  pulp_write32(DVSI_ADDRESS + DVSI_CFG_GLOB_OFFSET         , cfg_glob_reg);

}

static inline void plp_dvsi_wait(uint32_t cycles){
  for (volatile uint32_t i=0; i<cycles; i++)
    asm volatile ("nop");
}

static inline void plp_dvsi_trigger_saer_reg(void) {
  // write "1" to DVSI global register to trigger SAER frame readout, wait a few cycles for the value to propagate, then write a 0
  uint32_t glob_cfg = pulp_read32(DVSI_ADDRESS + DVSI_CFG_GLOB_OFFSET);
  glob_cfg |= 1;
  pulp_write32(DVSI_ADDRESS + DVSI_CFG_GLOB_OFFSET, glob_cfg);
  plp_dvsi_wait(10);
  glob_cfg &= ~1;
  pulp_write32(DVSI_ADDRESS + DVSI_CFG_GLOB_OFFSET, glob_cfg);
}

static inline void plp_dvsi_trigger_fb_reg(void) {
  // write "1" to DVSI global register to trigger framebuffer readout, wait a few cycles for the value to propagate, then write a 0
  uint32_t glob_cfg = pulp_read32(DVSI_ADDRESS + DVSI_CFG_GLOB_OFFSET);
  glob_cfg |= (1 << 20);
  pulp_write32(DVSI_ADDRESS + DVSI_CFG_GLOB_OFFSET, glob_cfg);
  plp_dvsi_wait(10);
  glob_cfg &= ~(1 << 20);
  pulp_write32(DVSI_ADDRESS + DVSI_CFG_GLOB_OFFSET, glob_cfg);
}

static inline void plp_dvsi_disable(){
  pulp_write32(DVSI_ADDRESS + DVSI_CFG_GLOB_OFFSET, 0x00000000); // todo
}

#endif