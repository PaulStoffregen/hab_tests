#ifndef CPU_IRQ_C
#define CPU_IRQ_C
#endif /* CPU_IRQ_C */

#include "bsp.h"
#include "cpu_type.h"
#include "cpu_irq.h"


/*!
 ******************************************************************************
 * Function: cpu_disableIRQs(void)
 ******************************************************************************
 * @par Description:
 *   This function globally disables all interrupts.
 *
 ******************************************************************************
 */

void cpu_disableIRQs(void)
{
  CPU_DIS_IRQS();
}


/*!
 ******************************************************************************
 * Function: cpu_enableIRQs
 ******************************************************************************
 * @par Description:
 *   This function globally enables all interrupts.
 *
 ******************************************************************************
 */

void cpu_enableIRQs(void)
{
  CPU_ENA_IRQS();
}

