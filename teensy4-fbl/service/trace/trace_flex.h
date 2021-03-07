/*!
 ******************************************************************************
 * @file trace_flex.h
 ******************************************************************************
 *
 * @brief Trace module flexible interface API
 *
 * @par Purpose
 *   Propagation of the flexible interface API for the trace module.
 *
 ******************************************************************************
 */

#ifndef TRACE_FLEX_H
#define TRACE_FLEX_H

extern T_STATUS flex_trc_init(void);
extern T_STATUS flex_trc_deinit(void);
extern void flex_trc_sendString(const char *str);

#endif /* TRACE_FLEX_H */

