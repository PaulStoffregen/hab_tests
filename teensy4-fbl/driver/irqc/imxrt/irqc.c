#ifndef IRQC_C
#define IRQC_C
#endif /* IRQC_C */

#include "bsp.h"
#include "arm_nvic.h"

#include "irqc.h"

/* Forward decalration of default handler */
extern void armv7m_default_handler(void);

/* */
#define __vect_sect __attribute__((section(".m7vect"), used, aligned(1024)))


/* Definition of RAM vector table */
__vect_sect static T_IRQ_HANDLER irqc_ramVectors[IRQ_ID_MAX_IRQS + CM7_NUM_EXCS];


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void irqc_setupSingleIrq(const T_IRQC_IRQ_CFG* irqCfg)
{
  uint32 irqPrio;
  uint32 priGrp;

  priGrp = nvic_getPriorityGrouping();
  irqPrio = nvic_encodeIrqPrio(irqCfg->irqPrio, irqCfg->subPrio, priGrp);

  nvic_setIrqPrio(irqCfg->irqNum, irqPrio);
  nvic_enableIrq(irqCfg->irqNum);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void irqc_setupMultipleIrqs(const T_IRQC_IRQ_CFG* irqCfgArray)
{
  /* Setup IRQs */
  while(irqCfgArray->irqPrio != -1)
  {
    irqc_setupSingleIrq(irqCfgArray);
    irqCfgArray++;
  }
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void irqc_init(void)
{
  int vectNum;
  int irqNum;
  uint32 vtor;
  T_IRQ_HANDLER* excVectors;

  /* Get pointer to the current vector table in flash */
  REG32_RD_BASE_OFFS(vtor, ARMV7_M_SCB_BASE, SCB_VTOR_OFFS);
  excVectors = (T_IRQ_HANDLER*)vtor;
  
  /* Setup IRQ vector table.
   * Start with the exception vectors.
   */
  for(vectNum = 0; vectNum < CM7_NUM_EXCS; vectNum++)
  {
    /* Copy exception vectors from current vector table in flash */
    irqc_ramVectors[vectNum] = excVectors[vectNum];
  }

  /* Proceed with the interrupt vectors */
  for(vectNum = CM7_NUM_EXCS; vectNum < (CM7_NUM_EXCS + IRQ_ID_MAX_IRQS); vectNum++)
  {
    /* Set every interrupt vector to the default hadler initially */
    irqc_ramVectors[vectNum] = &armv7m_default_handler;
  }

  /* Set initial IRQ priorities */
  for(irqNum = 0; irqNum < IRQ_ID_MAX_IRQS; irqNum++)
  {
    nvic_setIrqPrio(irqNum, 128);
  }
  /* Set vector table offset register to the address of the RAM vector table */
  REG32_WR_BASE_OFFS((uint32)(void*)irqc_ramVectors, ARMV7_M_SCB_BASE, SCB_VTOR_OFFS);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void irqc_registerHandler(sint16 irqNum, T_IRQ_HANDLER irqHandler)
{
  if(irqNum > IRQ_ID_MAX_IRQS)
  {
    /* Invalid IRQ number */
  }
  else
  {
    irqc_ramVectors[CM7_NUM_EXCS + irqNum] = irqHandler;
  }
}

