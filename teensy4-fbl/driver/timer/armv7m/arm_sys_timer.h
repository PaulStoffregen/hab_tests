#ifndef ARM_SYS_TIMER_H
#define ARM_SYS_TIMER_H

/* Sys-Tick callback type */
typedef void (T_SYS_TICK_CBK)(void);


extern void arm_initSysTimer(uint32 clock, uint32 interval);
extern uint32 arm_pollSysTimer(void);
extern uint32 arm_getSysTimerCount(void);

extern void arm_setSystTimerCallback( void(*)(void));

#endif /* ARM_SYS_TIMER_H */

