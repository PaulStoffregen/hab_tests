#ifndef STARTUP_C
#define STARTUP_C
#endif /* STARTUP_C */

#include "bsp.h"
#include "config.h"
#include "trace_pub.h"
#include "libc.h"
#include "reg.h"
#include "misc.h"

#define __naked __attribute__((naked))
#define __align(x) __attribute__ ((used, aligned(x)))


void __align(1024) (*imxrt_ram_vectors[IRQ_ID_MAX_IRQS + 16])(void);

//extern void (*imxrt_ram_vectors[IRQ_ID_MAX_IRQS + 16])(void);

void armv7m_init(void)
{
  uint32 reg;
  for(int i = 0; i < IRQ_ID_MAX_IRQS + 16; i++)
  {
    imxrt_ram_vectors[i] = &armv7m_default_handler;
  }

  // PIT & GPT timers to run from 24 MHz clock (independent of CPU speed)
//  CCM_CSCMR1 = (CCM_CSCMR1 & ~CCM_CSCMR1_PERCLK_PODF(0x3F)) | CCM_CSCMR1_PERCLK_CLK_SEL;
  REG32_RD_BASE_OFFS(reg, CCM_DIGITAL_BASE, CCM_CSCMR1_OFFS);
  reg &= ~( 0
          | BF_MASK(CCM_CSCMR1_PER_CLK_PODF_BF)
          );
  reg |= ( 0
//         | BF_SET(0x3F, CCM_CSCMR1_PER_CLK_PODF_BF)
         | BF_SET(1, CCM_CSCMR1_PER_CLK_SEL_BF)
         );
  REG32_WR_BASE_OFFS(reg, CCM_DIGITAL_BASE, CCM_CSCMR1_OFFS);

  // UARTs run from 24 MHz clock (works if PLL3 off or bypassed)
//  CCM_CSCDR1 = (CCM_CSCDR1 & ~CCM_CSCDR1_UART_CLK_PODF(0x3F)) | CCM_CSCDR1_UART_CLK_SEL; 
  REG32_RD_BASE_OFFS(reg, CCM_DIGITAL_BASE, CCM_CSCDR1_OFFS);
  reg &= ~( 0
          | BF_MASK(CCM_CSCDR1_UART_CLK_PODF_BF)
          );
  reg |= ( 0
//         | BF_SET(0x3F, CCM_CSCDR1_UART_CLK_PODF_BF)
         | BF_SET(1, CCM_CSCDR1_UART_CLK_SEL_BF)
         );
  REG32_WR_BASE_OFFS(reg, CCM_DIGITAL_BASE, CCM_CSCDR1_OFFS);

  REG32_WR_BASE_OFFS(0xFFFFFFFF, IOMUXC_GPR_BASE, IOMUXC_GPR26_OFFS);
  /* As GPIO2 and GPIO7 share the same mux config we can select which
   * GPIO shall be routed: Select GPIO7 here.
   */
  REG32_WR_BASE_OFFS(0xFFFFFFFF, IOMUXC_GPR_BASE, IOMUXC_GPR27_OFFS);
  /* Set GPIO output */
  REG32_WR_BASE_OFFS((7<<2), GPIO7_BASE, GPIO_GDIR_OFFS);
//  REG32_WR_BASE_OFFS((7<<2), GPIO7_BASE, GPIO_DR_SET_OFFS);
//  REG32_WR_BASE_OFFS(0xFFFFFFFF, IOMUXC_GPR_BASE, IOMUXC_GPR28_OFFS);
  REG32_WR_BASE_OFFS(0xFFFFFFFF, IOMUXC_GPR_BASE, IOMUXC_GPR29_OFFS);
}

