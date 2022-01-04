#ifndef ARM_SYS_TIMER_C
#define ARM_SYS_TIMER_C
#endif /* ARM_SYS_TIMER_C */

#include "bsp.h"
#include "arm_sys_timer.h"
#include "trace_pub.h"


#define __weak __attribute__((weak))

/* Callback type */
typedef void (*T_SYS_TIMER_CBK)(void);

/* Default sys timer callback */
extern void systimer_handleSysTick(void);

#ifndef SYS_TIMER_DEFAULT_CBK
//#define SYS_TIMER_DEFAULT_CBK 0xAFFEDEAD
#define SYS_TIMER_DEFAULT_CBK systimer_handleSysTick
#if 1
void __weak systimer_handleSysTick(void)
{
  return;
}
#endif
#endif

/* Callback */
static T_SYS_TIMER_CBK arm_sysTimerCbk = (T_SYS_TIMER_CBK)SYS_TIMER_DEFAULT_CBK;


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void arm_initSysTimer(uint32 clock, uint32 interval)
{
  uint32 reg;

  /* This should give a periode of 1ms */
  REG32_WR_BASE_OFFS(interval, ARMV7_M_SYS_TIM_BASE, SYS_TIM_LOAD_OFFS);
  
  reg = (0
   | BF_SET(1, SYS_TIM_CLK_SRC_BF) /* Use processor clock */
   | BF_SET(1, SYS_TIM_ENA_BF)     /* Enable the timer */
//   | BF_SET(1, SYS_TIM_IRQ_ENA_BF) /* Enable timer IRQ */
   );
  
  REG32_WR_BASE_OFFS(reg, ARMV7_M_SYS_TIM_BASE, SYS_TIM_CTRL_OFFS);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void arm_setSystTimerCallback(void(*callback)(void))
{
  arm_sysTimerCbk = callback;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

uint32 arm_pollSysTimer(void)
{
  uint32 flag;

  REG32_RDBF_BASE_OFFS(flag, ARMV7_M_SYS_TIM_BASE, SYS_TIM_CTRL_OFFS, SYS_TIM_CNT_FLAG_BF);
  return flag;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

uint32 arm_getSysTimerCount(void)
{
  uint32 count;
  uint32 load;

  REG32_RD_BASE_OFFS(count, ARMV7_M_SYS_TIM_BASE, SYS_TIM_CNT_OFFS);
  REG32_RD_BASE_OFFS(load, ARMV7_M_SYS_TIM_BASE, SYS_TIM_LOAD_OFFS);
  return 1 + load - count;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void armv7m_sys_tick_handler(void)
{
  if(arm_sysTimerCbk != NULL)
  {
    arm_sysTimerCbk();
  }
}

