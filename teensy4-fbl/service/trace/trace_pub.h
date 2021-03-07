/*!
 ******************************************************************************
 * @file trace_pub.h
 ******************************************************************************
 *
 * @brief Public header file for trace module
 *
 * @par Purpose
 *   Propagate API for trace module
 *
 ******************************************************************************
 */

#ifndef TRACE_PUB_H
#define TRACE_PUB_H

#ifdef __cplusplus
extern "C" {
#endif


#define TRACE_MODE_STDOUT 1  /** Trace is sent to stdout */
#define TRACE_MODE_FLEX   2  /** Trace is sent to flexible interface */

/* Definition of trace mask flags */
#define ERROR_TRACE    (uint8)0x01U  /** Bit flag representing error traces */
#define EVENT_TRACE    (uint8)0x02U  /** Bit flag representing event traces */
#define FUNCT_TRACE    (uint8)0x04U  /** Bit flag representing function traces */
#define INFO_TRACE     (uint8)0x08U  /** Bit flag representing info traces */
#define FEATURE_TRACE  (uint8)0x10U  /** Bit flag representing feature traces */


/* Include trace's configuration */
#include "trace_cfg.h"

#if (TRACE_CLASS_FEATURE == STD_ON)
/* Include feature-trace's configuration */
#include "trace_feature_cfg.h"
#endif /* (TRACE_CLASS_FEATURE == STD_ON) */

#include "trace_types.h"

#ifdef TRACE_MODE

/*
 ******************************************************************************
 * Common trace functions
 ******************************************************************************
 */

/* Init / deinit */
extern T_STATUS trace_init(void);
extern T_STATUS trace_deinit(void);

/* Function prototypes for main trace functions */
extern void trace_send(const char* fmt, ...);
extern void trace_string(const char *str);
extern void trace_char(const char c);
extern void trace_dump(const void* const mem, uint16 len, uint8 width, uint8 indent);

/* The macro TRACE_SEND is always enabled */
#define TRACE_SEND(...) trace_send(__VA_ARGS__)
/* The macro TRACE_STRING is always enabled */
#define TRACE_STRING(str) trace_string(str)
/* The macro TRACE_CHAR is always enabled */
#define TRACE_CHAR(c) trace_char(c)
/* The macro TRACE_DUMP is always enabled */
#define TRACE_DUMP(mem, len, width, indent) trace_dump(mem, len, width, indent)

/* Function prototypes for trace control */
extern void trace_enaTraceClass(uint8 traceClass);
extern void trace_disTraceClass(uint8 traceClass);
extern uint8 trace_getTraceClass(void);
extern void trace_setTraceClass(uint8 traceClass);


/* Function prototypes for (de-)activateable trace functions */
#if (TRACE_CLASS_ERROR == STD_ON)
extern void trace_error(const char* fmt, ...);
#endif /* (TRACE_CLASS_ERROR == STD_ON) */


#if (TRACE_CLASS_EVENT == STD_ON)
extern void trace_event(const char* fmt, ...);
#endif /* (TRACE_CLASS_EVENT == STD_ON) */


#if (TRACE_CLASS_FUNCTION == STD_ON)
extern void trace_function(const char* fmt, ...);
#endif /* (TRACE_CLASS_FUNCTION == STD_ON) */


#if (TRACE_CLASS_INFO == STD_ON)
extern void trace_info(const char* fmt, ...);
#endif /* (TRACE_CLASS_INFO == STD_ON) */


#if (TRACE_CLASS_MESSAGE == STD_ON)
extern void trace_message(const char* fmt, ...);
#endif /* (TRACE_CLASS_MESSAGE == STD_ON) */



#if (TRACE_CLASS_ERROR == STD_ON)
#define TRACE_ERROR(...) trace_error(__VA_ARGS__)
#else /* (TRACE_CLASS_ERROR != STD_ON) */
#define TRACE_ERROR(...) /* empty */
#endif /* (TRACE_CLASS_ERROR) */


#if (TRACE_CLASS_EVENT == STD_ON)
#define TRACE_EVENT(...) trace_event(__VA_ARGS__)
#else /* (TRACE_CLASS_EVENT != STD_ON) */
#define TRACE_EVENT(...) /* empty */
#endif /* (TRACE_CLASS_EVENT) */


#if (TRACE_CLASS_FUNCTION == STD_ON)
#define TRACE_FUNCTION(...) trace_function(__VA_ARGS__)
#else /* (TRACE_CLASS_EVENT != STD_ON) */
#define TRACE_FUNCTION(...) /* empty */
#endif /* (TRACE_CLASS_EVENT) */


#if (TRACE_CLASS_INFO == STD_ON)
#define TRACE_INFO(...) trace_info(__VA_ARGS__)
#else /* (TRACE_CLASS_INFO != STD_ON) */
#define TRACE_INFO(...) /* empty */
#endif /* (TRACE_CLASS_INFO) */


#if (TRACE_CLASS_MESSAGE == STD_ON)
#define TRACE_MESSAGE(...) trace_message(__VA_ARGS__)
#else /* (TRACE_CLASS_MESSAGE != STD_ON) */
#define TRACE_MESSAGE(...) /* empty */
#endif /* (TRACE_CLASS_MESSAGE) */


#if (TRACE_CLASS_FEATURE == STD_ON)
#define TRACE_FEATURE(trcFeature, trcFeatureClass, ...)  trace_feature((uint16)trcFeature, trcFeatureClass, __VA_ARGS__)

/* Definition of user trace mask flags */
#define TRACE_FEATURE_CLASS0 (uint8)0x01U /** Bit flag representing feature trace class 0 */
#define TRACE_FEATURE_CLASS1 (uint8)0x02U /** Bit flag representing feature trace class 1 */
#define TRACE_FEATURE_CLASS2 (uint8)0x04U /** Bit flag representing feature trace class 2 */
#define TRACE_FEATURE_CLASS3 (uint8)0x08U /** Bit flag representing feature trace class 3 */
#define TRACE_FEATURE_CLASS4 (uint8)0x10U /** Bit flag representing feature trace class 4 */
#define TRACE_FEATURE_CLASS5 (uint8)0x20U /** Bit flag representing feature trace class 5 */
#define TRACE_FEATURE_CLASS6 (uint8)0x40U /** Bit flag representing feature trace class 6 */
#define TRACE_FEATURE_CLASS7 (uint8)0x80U /** Bit flag representing feature trace class 7 */

extern void trace_feature(uint16 trcFeature, uint8 trcFeatureClass, const char* fmt, ...);

extern void  trace_enaFeatureTraceClass(uint16 trcFeature, uint8 trcFeatureClass);
extern void  trace_disFeatureTraceClass(uint16 trcFeature, uint8 trcFeatureClass);
extern boolean trace_isFeatureTraceClassEnabled(uint16 trcFeature, uint8 trcFeatureClass);
extern uint8 trace_getFeatureTraceFilter(uint16 trcFeature);
extern void trace_setFeatureTraceFilter(uint16 trcFeature, uint8 trcFeatureFilter);

#else /* (TRACE_CLASS_FEATURE != STD_ON) */
#define TRACE_FEATURE(trcFeature, trcFeatureClass, ...)  /* empty */
#endif /* (TRACE_CLASS_FEATURE == STD_ON) */

/*lint -restore */

#else /* TRACE_MODE */

/* If TRACE_MODE is undefined, all trace macros will be empty */

#define TRACE_SEND(...)     /* empty */
#define TRACE_STRING(...)   /* empty */
#define TRACE_CHAR(...)     /* empty */
#define TRACE_ERROR(...)    /* empty */
#define TRACE_EVENT(...)    /* empty */
#define TRACE_FUNCTION(...) /* empty */
#define TRACE_INFO(...)     /* empty */
#define TRACE_MESSAGE(...)  /* empty */
#define TRACE_FEATURE(trcFeature, trcFeatureClass, ...)  /* empty */

#endif /* TRACE_MODE */

#ifdef __cplusplus
}
#endif

#endif /* TRACE_PUB_H */

