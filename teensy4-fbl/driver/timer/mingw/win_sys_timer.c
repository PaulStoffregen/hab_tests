#ifndef WIN_SYS_TIMER_C
#define WIN_SYS_TIMER_C
#endif /* WIN_SYS_TIMER_C */

#include "bsp.h"
#include <windows.h>
#include <stdio.h>
#include "win_sys_timer.h"


typedef struct
{
  HANDLE hTimer;      /* Handle of the cyclic base timer */
  HANDLE hTimerQueue; /* Handle of the queue containing the cyclic base timer */
  DWORD  lastTickCnt;
  uint32 flag;
}T_SYS_TIMER_DATA;

static T_SYS_TIMER_DATA win_sys_timerData;


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
static T_SYS_TIMER_CBK win_sysTimerCbk = (T_SYS_TIMER_CBK)SYS_TIMER_DEFAULT_CBK;

VOID CALLBACK win_sys_tick_handler(PVOID lpParam, BOOLEAN timerOrWaitFired);


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void win_initSysTimer(uint32 clock, uint32 interval)
{
  T_SYS_TIMER_DATA* timerData = &win_sys_timerData;

  (void)clock;

  memset(timerData, 0, sizeof(T_SYS_TIMER_DATA));
  
  /* Create windows timer queue */
  timerData->hTimerQueue = CreateTimerQueue();
  if(NULL == timerData->hTimerQueue)
  {
    fprintf(stderr, "Error: %s:%d", __FILE__, __LINE__);
	 exit(-1);
  }
  else if(FALSE == CreateTimerQueueTimer(&timerData->hTimer, timerData->hTimerQueue, (WAITORTIMERCALLBACK)&win_sys_tick_handler, NULL, 0, interval, WT_EXECUTELONGFUNCTION))
  {
    fprintf(stderr, "Error: %s:%d", __FILE__, __LINE__);
    exit(-1);
  }
  else
  {
    timerData->lastTickCnt = GetTickCount();
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

void win_deinitSysTimer(void)
{
  T_SYS_TIMER_DATA* timerData = &win_sys_timerData;
  
  /* Stop and delete the base timer if not already deleted */
  if(NULL == timerData->hTimer)
  {
  }
  else if(0 != DeleteTimerQueueTimer(timerData->hTimerQueue, timerData->hTimer, NULL))
  {
    exit(-1);
  }
  
  if(NULL == timerData->hTimerQueue)
  {
  }
  else if(0 != DeleteTimerQueue(timerData->hTimerQueue))
  {
    exit(-1);
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

void win_setSystTimerCallback(void(*callback)(void))
{
  win_sysTimerCbk = callback;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

uint32 win_pollSysTimer(void)
{
  T_SYS_TIMER_DATA* timerData = &win_sys_timerData;
  uint32 flag = timerData->flag;

  timerData->flag = 0;

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

uint32 win_getSysTimerCount(void)
{
  T_SYS_TIMER_DATA* timerData = &win_sys_timerData;
  uint32 currTicks;

  currTicks = GetTickCount();
  return currTicks - timerData->lastTickCnt;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

VOID CALLBACK win_sys_tick_handler(PVOID lpParam, BOOLEAN timerOrWaitFired)
{
  T_SYS_TIMER_DATA* timerData = &win_sys_timerData;
  
  (void)lpParam;
  (void)timerOrWaitFired;
  
  timerData->lastTickCnt = GetTickCount();
  timerData->flag = 1;
  if(NULL != win_sysTimerCbk)
  {
    win_sysTimerCbk();
  }
}

