/*!
 ******************************************************************************
 * @file trace_prv.h
 ******************************************************************************
 *
 * @brief Private header file for trace module
 *
 * @par Purpose
 *   Private defines and interfaces for trace module
 *
 ******************************************************************************
 */

#ifndef TRACE_PRV_H
#define TRACE_PRV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "trace_pub.h"
#include <stdarg.h> /* For va_arg */


typedef struct
{
  T_TRACE_FILTER* pTraceFilter;
  T_TRACE_FILTER  traceFilter;
}T_TRACE_DATA;


#if (TRACE_MODE == TRACE_MODE_STDOUT)

/**
 * \par Description:
 *      Send trace to standard output
 *
 * \param[in] fmt
 *      Format string
 * \param[in] args
 *      Argument list
 *
 */
extern void trace_sendTraceStdOut(const char *fmt, va_list args);

/**
 * \par Description:
 *      Initialize standard output trace channel.
 *
 * \return
 *    Standard Error code
 *
 */
extern T_STATUS trace_initStdOut(void);

/**
 * \par Description:
 *      Deinitialize standard output trace channel.
 *
 * \return
 *    Standard Error code
 *
 */
extern T_STATUS trace_deinitStdOut(void);

#elif (TRACE_MODE == TRACE_MODE_FILE)

/**
 * \par Description:
 *      Set trace file name
 *
 * \param[in] fileName
 *      Trace file name
 *
 */
/*lint -e{970} char outside of a typedef */
extern void trace_setFileName(const char *fileName);

/**
 * \par Description:
 *      Set trace file handle
 *
 * \param[in] fileHandle
 *      Trace file handle
 *
 */
extern void trace_setFileHandle(void *fileHandle);

/**
 * \par Description:
 *      Send trace to file
 *
 * \param[in] fmt
 *      Format string
 * \param[in] args
 *      Argument list
 *
 */
/*lint -e{970} char outside of a typedef */
extern void trace_sendTraceFile(const char *fmt, va_list args);

/**
 * \par Description:
 *      Initialize file trace channel, i.e. create trace file.
 *
 * \return
 *    Standard Error code
 *
 */
extern T_STATUS trace_initFile(void);

/**
 * \par Description:
 *      Deinitialize file trace channel, i.e. close trace file.
 *
 * \return
 *    Standard Error code
 *
 */
extern T_STATUS trace_deinitFile(void);

#elif (TRACE_MODE == TRACE_MODE_UART)

/**
 * \par Description:
 *      Send trace to serial interface
 *
 * \param[in] fmt
 *      Format string
 * \param[in] args
 *      Argument list
 *
 */
/*lint -e{970} char outside of a typedef */
extern void trace_sendTraceUart(const char *fmt, va_list args);

/**
 * \par Description:
 *      Initialize serial interface trace channel.
 *
 * \return
 *    Standard Error code
 *
 */
extern T_STATUS trace_initUart(void);

/**
 * \par Description:
 *      Deinitialize serial interface trace channel.
 *
 * \return
 *    Standard Error code
 *
 */
extern T_STATUS trace_deinitUart(void);

#elif (TRACE_MODE == TRACE_MODE_IPCON)

/**
 * \par Description:
 *      Set transmit buffer and size.
 *
 * \param[in] buffer
 *      Transmit buffer
 * \param[in] size
 *      Transmit buffer size
 */
extern void trace_setTxBuffer(uint8_t *buffer, uint16 size);

/**
 * \par Description:
 *      Set local port.
 *
 * \param[in] port
 *      Local port
 */
extern void trace_setLocalPort(uint16 port);

/**
 * \par Description:
 *      Set remote port.
 *
 * \param[in] port
 *      Remote port
 */
extern void trace_setRemotePort(uint16 port);

/**
 * \par Description:
 *      Set remote IP address.
 *
 * \param[in] ipAddr
 *      Remote IP address
 */
/*lint -e{970} char outside of a typedef */
extern void trace_setRemoteAddr(const char *ipAddr);

/**
 * \par Description:
 *      Set transmission timeout.
 *
 * \param[in] timeout
 *      Transmission timeout
 */
extern void trace_setTxTimeout(uint16 timeout);

/**
 * \par Description:
 *      Send trace via IP connection
 *
 * \param[in] fmt
 *      Format string
 * \param[in] args
 *      Argument list
 *
 */
/*lint -e{970} char outside of a typedef */
extern void trace_sendTraceIpCon(const char *fmt, va_list args);

/**
 * \par Description:
 *      Initialize IP trace channel, i.e. open socket connection.
 *
 * \return
 *    Standard Error code
 *
 */
extern T_STATUS trace_initIpCon(void);

/**
 * \par Description:
 *      Deinitialize IP trace channel, i.e. close socket connection.
 *
 * \return
 *    Standard Error code
 *
 */
extern T_STATUS trace_deinitIpCon(void);

#elif (TRACE_MODE == TRACE_MODE_FLEX)

/**
 * \par Description:
 *      Send trace to flexible interface
 *
 * \param[in] fmt
 *      Format string
 * \param[in] args
 *      Argument list
 *
 */
/*lint -e{970} char outside of a typedef */

extern void trace_sendTraceFlex(const char *fmt, va_list args);

/**
 * \par Description:
 *      Initialize flexible interface trace channel.
 *
 * \return
 *    Standard Error code
 *
 */

extern T_STATUS trace_initFlex(void);

/**
 * \par Description:
 *      Deinitialize flexible interface trace channel.
 *
 * \return
 *    Standard Error code
 *
 */

extern T_STATUS trace_deinitFlex(void);

#endif /* (TRACE_MODE) */

/*lint -restore */

#ifdef __cplusplus
}
#endif

#endif /* TRACE_PRV_H */

