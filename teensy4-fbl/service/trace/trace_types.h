/*!
 ******************************************************************************
 * @file trace_types.h
 ******************************************************************************
 *
 * @brief Type header file for trace module
 *
 * @par Purpose
 *   Propagate types for trace module
 *
 ******************************************************************************
 */

#ifndef TRACE_TYPES_H
#define TRACE_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef struct
{
  uint8 traceClassFilter;                         /**< Trace filter for ERROR, EVENT, ... */
#if (TRACE_CLASS_FEATURE == STD_ON)
  uint8 traceFeatureFilter[MAX_TRC_FEATURE_ID];   /**< Feature trace filter */
#endif
}T_TRACE_FILTER;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TRACE_TYPES_H */

