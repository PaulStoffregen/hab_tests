#ifndef ARM_NVIC_C
#define ARM_NVIC_C
#endif /* ARM_NVIC_C */

#include "bsp.h"
#include "arm_cpu.h"
#include "arm_nvic.h"


#define LOG2_BITS_PER_BYTE       (3)
#define LOG2_BITS_PER_HALFWORD   (4)
#define LOG2_BITS_PER_WORD       (5)
#define LOG2_BYTES_PER_HALFWORD  (LOG2_BITS_PER_WORD - LOG2_BITS_PER_BYTE)
#define LOG2_BYTES_PER_WORD      (LOG2_BITS_PER_WORD - LOG2_BITS_PER_BYTE)


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void nvic_setPriorityGrouping(uint8 priGrp)
{
  uint32 reg;

  /* Read old register configuration */
  REG32_RD_BASE_OFFS(reg, ARMV7_M_SCB_BASE, SCB_AIRCR_OFFS);
  /* Clear bits to change */
  reg &= ~( 0
          | BF_MASK(SCB_AIRCR_VECTKEY_BF)
          | BF_MASK(SCB_AIRCR_PRIGROP_BF)
          );
  /* Insert write key and priorty group */
  reg |= ( 0
         | BF_SET(SCB_AIRCR_VECTKEY_VALUE, SCB_AIRCR_VECTKEY_BF)
         | BF_SET(priGrp, SCB_AIRCR_PRIGROP_BF)
         );
  REG32_WR_BASE_OFFS(reg, ARMV7_M_SCB_BASE, SCB_AIRCR_OFFS);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

uint8 nvic_getPriorityGrouping(void)
{
  uint32 reg;
  
  REG32_RD_BASE_OFFS(reg, ARMV7_M_SCB_BASE, SCB_AIRCR_OFFS);
  return BF_GET(reg, SCB_AIRCR_PRIGROP_BF);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void nvic_enableIrq(sint16 irqNum)
{
  uint8  wordNum;
  uint8  bitPos;
  uint32 offs;
  uint32 mask;

  wordNum = BW_GET(irqNum, LOG2_BITS_PER_WORD, 4);
  bitPos = BW_GET(irqNum, 0, LOG2_BITS_PER_WORD);

  offs = NVIC_ISER_BASE_OFFS + (wordNum << LOG2_BYTES_PER_WORD);

  mask = BW_MASK(bitPos, 1);
  REG32_WR_BASE_OFFS(mask, ARMV7_M_NVIC_BASE, offs);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void nvic_disableIrq(sint16 irqNum)
{
  uint8  wordNum;
  uint8  bitPos;
  uint32 offs;
  uint32 mask;

  wordNum = BW_GET(irqNum, LOG2_BITS_PER_WORD, 4);
  bitPos = BW_GET(irqNum, 0, LOG2_BITS_PER_WORD);

  offs = NVIC_ICER_BASE_OFFS + (wordNum << LOG2_BYTES_PER_WORD);

  mask = BW_MASK(bitPos, 1);
  REG32_WR_BASE_OFFS(mask, ARMV7_M_NVIC_BASE, offs);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

boolean nvic_isIrqPending(sint16 irqNum)
{
   uint8  wordNum;
   uint8  bitPos;
   uint32 offs;
   uint32 mask;
   
   wordNum = BW_GET(irqNum, LOG2_BITS_PER_WORD, 4);
   bitPos = BW_GET(irqNum, 0, LOG2_BITS_PER_WORD);
   
   offs = NVIC_ISPR_BASE_OFFS + (wordNum << LOG2_BYTES_PER_WORD);
   
   REG32_RD_BASE_OFFS(mask, ARMV7_M_NVIC_BASE, offs);
   return (0 != (mask & BW_MASK(bitPos, 1)));
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void nvic_setIrqPending(sint16 irqNum)
{
   uint8  wordNum;
   uint8  bitPos;
   uint32 offs;
   uint32 mask;
   
   wordNum = BW_GET(irqNum, LOG2_BITS_PER_WORD, 4);
   bitPos = BW_GET(irqNum, 0, LOG2_BITS_PER_WORD);
   
   offs = NVIC_ISPR_BASE_OFFS + (wordNum << LOG2_BYTES_PER_WORD);
   
   mask = BW_MASK(bitPos, 1);
   REG32_WR_BASE_OFFS(mask, ARMV7_M_NVIC_BASE, offs);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void nvic_clrIrqPending(sint16 irqNum)
{
   uint8  wordNum;
   uint8  bitPos;
   uint32 offs;
   uint32 mask;
   
   wordNum = BW_GET(irqNum,LOG2_BITS_PER_WORD, 4);
   bitPos = BW_GET(irqNum, 0, LOG2_BITS_PER_WORD);
   
   offs = NVIC_ICPR_BASE_OFFS + (wordNum << LOG2_BYTES_PER_WORD);
   
   mask = BW_MASK(bitPos, 1);
   REG32_WR_BASE_OFFS(mask, ARMV7_M_NVIC_BASE, offs);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

boolean nvic_isIrqActive(sint16 irqNum)
{
   uint8  wordNum;
   uint8  bitPos;
   uint32 offs;
   uint32 mask;
   
   wordNum = BW_GET(irqNum, LOG2_BITS_PER_WORD, 4);
   bitPos = BW_GET(irqNum, 0, LOG2_BITS_PER_WORD);
   
   offs = NVIC_IABR_BASE_OFFS + (wordNum << LOG2_BYTES_PER_WORD);
   
   REG32_RD_BASE_OFFS(mask, ARMV7_M_NVIC_BASE, offs);
   return (0 != (mask & BW_MASK(bitPos, 1)));
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void nvic_setIrqPrio(sint16 irqNum, uint32 irqPrio)
{
  uint32 priGrp = nvic_getPriorityGrouping();
  uint32 numGrpPrioBits = (BF_WIDTH_MASK(SCB_AIRCR_PRIGROP_BF) - priGrp);
  uint32 numSubPrioBits = priGrp + 1;
  uint32 offs;
  uint8  mask;

  if(irqNum < 0)
  {
    /* System interrupt */
    offs = SCB_SHPR_BASE_OFFS + irqNum + 12;
    mask = BW_SET(irqPrio, numSubPrioBits, numGrpPrioBits);
    REG8_WR_BASE_OFFS(mask, ARMV7_M_SCB_BASE, offs);
  }
  else
  {
    /* Peripheral interrupt */
    offs = NVIC_IPR_BASE_OFFS + irqNum;
    mask = BW_SET(irqPrio, numSubPrioBits, numGrpPrioBits);
    REG8_WR_BASE_OFFS(mask, ARMV7_M_NVIC_BASE, offs);
  }
}


/*
 * priGrp | grpPrio | subPrio | bit-map
 *      0 |  [7:1]  |   [0]   | GGGGGGGs
 *      1 |  [7:2]  |  [1:0]  | GGGGGGss
 *      2 |  [7:3]  |  {2:0]  | GGGGGsss
 *      3 |  {7:4]  |  [3:0]  | GGGGssss
 *      4 |  [7:5]  |  [4:0]  | GGGsssss
 *      5 |  [7:6]  |  [5:0]  | GGssssss
 *      6 |   [7]   |  [6:0]  | Gsssssss
 *      7 |    -    |  [7:0]  | ssssssss
 *
 * numSubPrioBits = 1 + priGrp
 * numGrpPrioBits = 7 - priGrp
 * grpPrioLsb = numSubPrioBits
 */
 
/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

uint32 nvic_getIrqPrio(sint16 irqNum)
{
  uint32 priGrp = nvic_getPriorityGrouping();
  uint32 numGrpPrioBits = (BF_WIDTH_MASK(SCB_AIRCR_PRIGROP_BF) - priGrp);
  uint32 numSubPrioBits = priGrp + 1;
  uint32 grpPrio;
  uint32 offs;
  uint8  mask;

  if(irqNum < 0)
  {
    /* System interrupt */
    offs = SCB_SHPR_BASE_OFFS + irqNum + 12;
    REG8_RD_BASE_OFFS(mask, ARMV7_M_SCB_BASE, offs);
    grpPrio = BW_GET(mask, numSubPrioBits, numGrpPrioBits);
  }
  else
  {
    /* Peripheral interrupt */
    offs = NVIC_IPR_BASE_OFFS + irqNum;
    REG8_RD_BASE_OFFS(mask, ARMV7_M_NVIC_BASE, offs);
    grpPrio = BW_SET(mask, numSubPrioBits, numGrpPrioBits);
  }
  return grpPrio;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

uint32 nvic_encodeIrqPrio(uint32 grpPrio, uint32 subPrio, uint32 priGrp)
{
  uint32 numGrpPrioBits;
  uint32 numSubPrioBits;
  uint32 result;

  /* Only values 0..7 allowed */
  priGrp &= BF_WIDTH_MASK(SCB_AIRCR_PRIGROP_BF);

  numGrpPrioBits = (BF_WIDTH_MASK(SCB_AIRCR_PRIGROP_BF) - priGrp);
  numSubPrioBits = priGrp + 1;

  result = ( 0
           | BW_SET(grpPrio, numSubPrioBits, numGrpPrioBits)
           | BW_SET(subPrio, 0, numSubPrioBits)
           );
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void nvic_decodeIrqPrio(uint32* grpPrio, uint32* subPrio, uint32 priGrp, uint32 irqPrio)
{
  uint32 numGrpPrioBits;
  uint32 numSubPrioBits;

  /* Only values 0..7 allowed */
  priGrp &= BF_WIDTH_MASK(SCB_AIRCR_PRIGROP_BF);

  numGrpPrioBits = (BF_WIDTH_MASK(SCB_AIRCR_PRIGROP_BF) - priGrp);
  numSubPrioBits = priGrp + 1;

  *grpPrio = BW_GET(irqPrio, numSubPrioBits, numGrpPrioBits);
  *subPrio = BW_SET(irqPrio, 0, numSubPrioBits);
}


