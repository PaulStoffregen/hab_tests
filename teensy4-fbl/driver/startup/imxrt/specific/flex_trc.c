#ifndef FLEX_TRC_C
#define FLEX_TRC_C
#endif /* FLEX_TRC_C */

#include "bsp.h"
#include "trace_flex.h"

#include "misc.h"

T_STATUS flex_trc_init(void)
{
  return STATUS_eOK;
}


T_STATUS flex_trc_deinit(void)
{
  return STATUS_eOK;
}


void flex_trc_sendString(const char* str)
{
  earlyDbg_sendStr(str);
}

