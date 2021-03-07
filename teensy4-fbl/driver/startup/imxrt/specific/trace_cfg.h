#ifndef TRACE_CFG_H
#define TRACE_CFG_H

#include "bsp.h"

/* Select the appropriate trace mode */
#define TRACE_MODE           TRACE_MODE_FLEX
#define TRACE_BUF_SIZE       (1 * 1024) /* 1KiB */

/* Compile-time switches for trace classes */
#define TRACE_CLASS_FUNCTION STD_ON
#define TRACE_CLASS_EVENT    STD_ON
#define TRACE_CLASS_ERROR    STD_ON
#define TRACE_CLASS_INFO     STD_ON
#define TRACE_CLASS_FEATURE  STD_ON

/* Compile-time switch for support of debug interface */
#define TRACE_DEBUG_SUPPORT  STD_OFF

/* Initial trace mask */
#define TRACE_MASK_INIT     (ERROR_TRACE | EVENT_TRACE | FUNCT_TRACE | INFO_TRACE | FEATURE_TRACE)

#endif /* TRACE_CFG_H */
