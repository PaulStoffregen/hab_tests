#ifndef WIN_SYS_TIMER_H
#define WIN_SYS_TIMER_H

/* Sys-Tick callback type */
typedef void (T_SYS_TICK_CBK)(void);


extern void win_initSysTimer(uint32 clock, uint32 interval);
extern uint32 win_pollSysTimer(void);
extern uint32 win_getSysTimerCount(void);

extern void win_setSystTimerCallback( void(*)(void));

#endif /* WIN_SYS_TIMER_H */

