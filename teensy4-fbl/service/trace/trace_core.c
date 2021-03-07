/*!
 ******************************************************************************
 * @file trace_core.c
 ******************************************************************************
 *
 * @brief  Trace module core functionality
 *
 * @par Purpose
 *   Implements functions of the trace interface and the interface to
 *   configure trace filter settings at runtime. Depending on selected trace
 *   mode the trace module prepares the routing of the trace (stdout, file,
 *   ip connection)
 *
 ******************************************************************************
 */

#ifndef TRACE_CORE_C
#define TRACE_CORE_C
#endif /* TRACE_CORE_C */

#include "bsp.h"
#include "trace_prv.h"
#include "libc.h"


#if (TRACE_CLASS_FEATURE != STD_ON)
#define TRACE_FEATURE_MASK_INIT /* empty */
#endif /* (TRACE_CLASS_FEATURE == STD_ON) */


/*!
 * Internal trace filter, used if no external filter address is passed to trace_init().
 */
static T_TRACE_DATA trace_data =
{
  .pTraceFilter = &trace_data.traceFilter,
  .traceFilter =
  {
    .traceClassFilter =
#ifdef TRACE_MASK_INIT
      TRACE_MASK_INIT,
#else /* !TRACE_MASK_INIT */
      0,
#endif /* TRACE_MASK_INIT */

#if (TRACE_CLASS_FEATURE == STD_ON)
    .traceFeatureFilter =
#ifdef TRACE_FEATURE_MASK_INIT
      TRACE_FEATURE_MASK_INIT,
#else /* !TRACE_FEATURE_MASK_INIT */
      { 0 },
#endif /* TRACE_FEATURE_MASK_INIT */
#endif /* (TRACE_CLASS_FEATURE) */
  },
};


/*
 ******************************************************************************
 * Static Functions
 ******************************************************************************
 */


/*!
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Send trace to configured trace interface
 *
 * @param[I] fmt - Format string
 * @param[I] args - Argument list
 *
 ******************************************************************************
 */

static void trace_sendTrace(const char* fmt, va_list args)
{
#if (TRACE_MODE == TRACE_MODE_STDOUT)
  trace_sendTraceStdOut(fmt, args);
#elif (TRACE_MODE == TRACE_MODE_FLEX)
  trace_sendTraceFlex(fmt, args);
#endif
}


/*
 ******************************************************************************
 * Public Functions
 ******************************************************************************
 */


/*!
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Initialize trace module and trace filters, including set up of trace
 *   routing depending on TRACE_MODE.
 *
 * @return
 *    Standard Error code
 *
 ******************************************************************************
 */

T_STATUS trace_init(void)
{
  T_STATUS result;

#if (TRACE_MODE == TRACE_MODE_STDOUT)
  result = trace_initStdOut();
#elif (TRACE_MODE == TRACE_MODE_FLEX)
  result = trace_initFlex();
#endif
  return result;
}


/*!
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Deinitialize trace module, including shut down of trace routing,
 *   depending on TRACE_MODE
 *
 * @return
 *    Standard Error code
 *
 ******************************************************************************
 */

T_STATUS trace_deinit(void)
{
  T_STATUS result;

#if (TRACE_MODE == TRACE_MODE_STDOUT)
  result = trace_deinitStdOut();
#elif (TRACE_MODE == TRACE_MODE_FLEX)
  result = trace_deinitFlex();
#endif
  return result;
}


/*!
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Output a trace via the configured trace interface without any filtering
 *
 * @param[I] fmt
 * @param[I] vargs - Trace content to be sent
 *
 ******************************************************************************
 */

void trace_send(const char* fmt, ...)
{
  va_list args;

  /* Get variable arguments list */
  va_start(args, fmt);
  /* Send trace data */
  trace_sendTrace(fmt, args);
  va_end(args);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * \par Description:
 *      Output a message via the configured trace interface without any filtering
 *
 * \param[in] message
 *      Message string to be sent
 *
 ******************************************************************************
 */

void trace_message(const char* fmt, ...)
{
  va_list args;

  /* Get variable arguments list */
  va_start(args, fmt);
  /* Send trace data */
  trace_sendTrace(fmt, args);
  va_end(args);
}


/*!
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Output a string via the configured trace interface without any filtering
 *
 * @param[I] str - String to be sent
 *
 ******************************************************************************
 */

void trace_string(const char* str)
{
  trace_send("%s", str);
}


/*!
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Output a character via the configured trace interface without any filtering
 *
 * @param[I] c - Character to be sent
 *
 ******************************************************************************
 */

void trace_char(const char c)
{
  trace_send("%c", c);
}


/*!
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Output memory dump via the configured trace interface without any filtering
 *
 * @param[I] mem - Pointer to the memory to be dumped
 * @param[I] len - Length in bytes of the memory to be dumped
 * @param[I] width - Number of columns used for the dump
 *
 ******************************************************************************
 */

void trace_dump(const void* const mem, uint16 len, uint8 width, uint8 indent)
{
  const uint8* const buf = (const uint8*)mem;
  int i;
  int k;
  int col = 0;

  for(k = 0; k < indent; k++)
  {
    trace_send("%c", ' ');
  }

  for(i = 0; i < len; i++)
  {
    col++;
    if(i == (len - 1))
    {
      /* We reached the last byte to be dumped */
      trace_send("%02X\n", buf[i]);
    }
    else if(col < width)
    {
      /* We didn't yet reach the last byte to be dumped and
       * even did not reach the max width.
       */
      trace_send("%02X ", buf[i]);
    }
    else
    {
      /* We didn't yet reach the last byte to be dumped,
       * but reached the myximum width.
       */
      col = 0;
      trace_send("%02X\n", buf[i]);
      for(k = 0; k < indent; k++)
      {
        trace_send("%c", ' ');
      }
    }
  }
}


/*!
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Enable the given trace class (INFO, EVENT, FUNCTION, FEATURE, ERROR)
 *
 * @param[I] traceClass - Trace class to be enabled
 *
 ******************************************************************************
 */

void trace_enaTraceClass(uint8 traceClass)
{
  trace_data.pTraceFilter->traceClassFilter |= traceClass;
}


/*!
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Disable the given trace class (INFO, EVENT, FUNCTION, FEATURE, ERROR)
 *
 * @param[I] traceClass - Trace class to be disabled
 *
 ******************************************************************************
 */

void trace_disTraceClass(uint8 traceClass)
{
  trace_data.pTraceFilter->traceClassFilter &= (uint8)~(traceClass);
}


/*!
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Get the current trace mask (INFO,EVENT,FUNCTION,FEATURE,ERROR)
 *
 ******************************************************************************
 */

uint8 trace_getTraceClass(void)
{
  return trace_data.pTraceFilter->traceClassFilter;
}


/*!
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Set the new trace mask (INFO,EVENT,FUNCTION,FEATURE,ERROR)
 *
 ******************************************************************************
 */

void trace_setTraceClass(uint8 traceClass)
{
  trace_data.pTraceFilter->traceClassFilter = traceClass;
}


#if (TRACE_CLASS_FEATURE == STD_ON)

/*!
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Set new filter class for dedicated trace feature
 *
 * @param[in] trcFeature
 *      Trace feature to set the new class for
 * @param[in] trcFeatureClass
 *      New filter class to be set
 *
 ******************************************************************************
 */

void trace_enaFeatureTraceClass(uint16 trcFeature, uint8 trcFeatureClass)
{
  trace_data.pTraceFilter->traceFeatureFilter[trcFeature] |= trcFeatureClass;
}


/*!
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Clear filter class for dedicated trace feature
 *
 * @param[in] trcFeature
 *      Trace feature to clear the class for
 * @param[in] trcFeatureClass
 *      Filter class to be cleared
 *
 ******************************************************************************
 */

void trace_disFeatureTraceClass(uint16 trcFeature, uint8 trcFeatureClass)
{
  trace_data.pTraceFilter->traceFeatureFilter[trcFeature] &= (uint8)~(trcFeatureClass);
}


/*!
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

boolean trace_isFeatureTraceClassEnabled(uint16 trcFeature, uint8 trcFeatureClass)
{
  boolean result;
  
  if(trcFeatureClass == (uint8)(trace_data.pTraceFilter->traceFeatureFilter[trcFeature] & trcFeatureClass) )
  {
    result = !FALSE;
  }
  else
  {
    result = FALSE;
  }
  return result;
}


/*!
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

uint8 trace_getFeatureTraceFilter(uint16 trcFeature)
{
  return trace_data.pTraceFilter->traceFeatureFilter[trcFeature];
}


/*!
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void trace_setFeatureTraceFilter(uint16 trcFeature, uint8 trcFeatureFilter)
{
  trace_data.pTraceFilter->traceFeatureFilter[trcFeature] = trcFeatureFilter;
}
#endif /* (TRACE_CLASS_FEATURE == STD_ON) */


#if (TRACE_CLASS_ERROR == STD_ON)

/*!
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Output an error message via the configured trace interface
 *
 * @param[in] error
 *      Error string to be sent
 *
 ******************************************************************************
 */

void trace_error(const char* fmt, ...)
{
  if(0 != (ERROR_TRACE & trace_data.pTraceFilter->traceClassFilter) )
  {
    va_list args;

    /* Get variable arguments list */
    va_start(args, fmt);
    /* Send trace data */
    trace_sendTrace(fmt, args);
    va_end(args);
  }
}
#endif /* (TRACE_CLASS_ERROR == STD_ON) */


#if (TRACE_CLASS_EVENT == STD_ON)

/*!
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Output an event message via the configured trace interface
 *
 * @param[in] event
 *      Event string to be sent
 *
 ******************************************************************************
 */

void trace_event(const char* fmt, ...)
{
  if(0 != (EVENT_TRACE & trace_data.pTraceFilter->traceClassFilter) )
  {
    va_list args;

    /* Get variable arguments list */
    va_start(args, fmt);
    /* Send trace data */
    trace_sendTrace(fmt, args);
    va_end(args);
  }
}
#endif /* (TRACE_CLASS_EVENT == STD_ON) */


#if (TRACE_CLASS_FUNCTION == STD_ON)

/*!
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Output a function trace via the configured trace interface
 *
 * @param[in] function
 *      Function string to be sent
 *
 ******************************************************************************
 */

void trace_function(const char* fmt, ...)
{
  if(0 != (FUNCT_TRACE & trace_data.pTraceFilter->traceClassFilter) )
  {
    va_list args;

    /* Get variable arguments list */
    va_start(args, fmt);
    /* Send trace data */
    trace_sendTrace(fmt, args);
    va_end(args);
  }
}
#endif /* (TRACE_CLASS_FUNCTION == STD_ON) */


#if (TRACE_CLASS_INFO == STD_ON)

/*!
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Output an information message via the configured trace interface
 *
 * @param[I] fmt - Format string
 * @param[I] vargs - Information to be sent
 *
 ******************************************************************************
 */

void trace_info(const char* fmt, ...)
{
  if(0 != (INFO_TRACE & trace_data.pTraceFilter->traceClassFilter) )
  {
    va_list args;

    /* Get variable arguments list */
    va_start(args, fmt);
    /* Send trace data */
    trace_sendTrace(fmt, args);
    va_end(args);
  }
}
#endif /* (TRACE_CLASS_INFO == STD_ON) */


#if (TRACE_CLASS_FEATURE == STD_ON)

/*!
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Output a message for the user defined trace feature with a user defined
 *   trace class via the configured trace interface
 *
 * @param[I] trcFeature - Trace feature for which to send the trace
 * @param[I] trcFeatureClass - Feature class at which to send the trace
 * @param[I] fmt - Format string
 * @param[I] vargs - Arguments to be sent
 *
 ******************************************************************************
 */

void trace_feature(uint16 trcFeature, uint8 trcFeatureClass, const char* fmt, ...)
{
  sint16 featureID = trcFeature;

  if(0 == (FEATURE_TRACE & trace_data.pTraceFilter->traceClassFilter) )
  {
    /* Feature traces are disabled */
  }
  else if(featureID >= MAX_TRC_FEATURE_ID)
  {
    /* Invalid trace feature */
#if defined (DEBUG)
    TRACE_ERROR("Invalid trace feature: %d of %d\n", featureID, MAX_TRC_FEATURE_ID);
#endif /* defined (DEBUG) */
  }
  else if(0 == (trcFeatureClass & trace_data.pTraceFilter->traceFeatureFilter[featureID]) )
  {
    /* Feature class is disabled */
  }
  else
  {
    /* Feature's debug level is higher than the trace's debug level */
    va_list args;
    /* Get variable arguments list */
    va_start(args, fmt);
    /* Send trace data */
    trace_sendTrace(fmt, args);
    va_end(args);
  }
}
#endif /* (TRACE_CLASS_FEATURE == STD_ON) */

