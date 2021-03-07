/*!
 ******************************************************************************
 * @file trace_stdout.c
 ******************************************************************************
 *
 * @brief  Trace StdOut module
 *
 * @par Purpose
 *      Implementation of the trace functions for the trace to standard output.
 *
 ******************************************************************************
 */
 
#ifndef TRACE_STDOUT_C
#define TRACE_STDOUT_C
#endif /* TRACE_STDOUT_C */

#include "bsp.h"
#include "trace_prv.h"


#if (TRACE_MODE == TRACE_MODE_STDOUT)

#include <stdio.h>

/*
 ******************************************************************************
 * Global functions
 ******************************************************************************
 */


T_STATUS trace_initStdOut(void)
{
  return STATUS_eOK;
}


T_STATUS trace_deinitStdOut(void)
{
  return STATUS_eOK;
}


void trace_sendTraceStdOut(const char* fmt, va_list args)
{
  (void)vfprintf(stdout, fmt, args);
  (void)fflush(stdout);
}

#endif /* (TRACE_MODE == TRACE_MODE_STDOUT) */

