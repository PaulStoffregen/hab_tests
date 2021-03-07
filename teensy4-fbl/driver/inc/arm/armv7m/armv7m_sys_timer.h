#ifndef ARMV7M_SYS_TIMER_H
#define ARMV7M_SYS_TIMER_H

/* ARMv7-M System Timer */

#define SYS_TIM_CTRL_OFFS        0x000

#define SYS_TIM_CNT_FLAG_BF      16,  1
#define SYS_TIM_CLK_SRC_BF        2,  1
#define SYS_TIM_IRQ_ENA_BF        1,  1
#define SYS_TIM_ENA_BF            0,  1


#define SYS_TIM_LOAD_OFFS        0x004

#define SYS_TIM_RELOAD_BF         0, 24


#define SYS_TIM_CNT_OFFS         0x008

#define SYS_TIM_COUNT_BF          0, 24


#define SYS_TIM_CAL_OFFS         0x00C

#define SYS_TIM_NOREF_BF         31,  1
#define SYS_TIM_SKEW_BF          30,  1
#define SYS_TIM_10MS_BF           0, 24

#endif /* ARMV7M_SYS_TIMER_H */

