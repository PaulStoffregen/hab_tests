/*!
 ******************************************************************************
 * @file trace_flex.c
 ******************************************************************************
 *
 * @brief  Trace flex module
 *
 * @par Purpose
 *      Implementation of the trace functions for the flexible output.
 *
 ******************************************************************************
 */
 
#ifndef TRACE_FLEX_C
#define TRACE_FLEX_C
#endif /* TRACE_FLEX_C */

#include "bsp.h"
#include "trace_prv.h"


#if (TRACE_MODE == TRACE_MODE_FLEX)

#include "libc.h" /* For vsprintf */
#include "trace_flex.h"

static uint8 trace_bufferFlex[TRACE_BUF_SIZE];


/*
 ******************************************************************************
 * Global functions
 ******************************************************************************
 */


T_STATUS trace_initFlex(void)
{
  return flex_trc_init();
}


T_STATUS trace_deinitFlex(void)
{
  return flex_trc_deinit();
}


void trace_sendTraceFlex(const char* fmt, va_list args)
{
  /* Copy data to trace buffer */
  (void)libc_vsnprintf((char*)trace_bufferFlex, TRACE_BUF_SIZE, fmt, args);
  /* Send contents of the trace buffer */
  flex_trc_sendString((char*)trace_bufferFlex);
}

#endif /* (TRACE_MODE == TRACE_MODE_FLEX) */

