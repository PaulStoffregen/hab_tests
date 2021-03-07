#ifndef DLCF_RBUF_C
#define DLCF_RBUF_C
#endif /* DLCF_RBUF_C */

#include "bsp.h"

#include "pdu.h"
#include "rbuf.h"
#include "dlcf.h"


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static boolean dlcf_rbuf_sendByte(void* param, const uint8 byte)
{
  boolean result;
  T_DLCF_DEV_DATA* devData = (T_DLCF_DEV_DATA*)param;

  if(RBUF_OK != rbuf_wrByte(devData->txFifo, byte))
  {
    /* Failed to transmit byte */
    result = FALSE;
  }
  else
  {
    /* Sucessfully transmitted byte */
    result = !FALSE;
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static boolean dlcf_rbuf_recvByte(void* param, uint8* byte)
{
  boolean result;
  T_RBUF_PIPE* devData = (T_RBUF*)param;

  if(RBUF_OK != rbuf_rdByte(rbufData->rxFifo, byte))
  {
    /* Failed to receive byte */
    result = FALSE;
  }
  else
  {
    /* Sucessfully received byte */
    result = !FALSE;
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

T_DLCF_DEV_INFO dlcf_getDevInfo(void)
{
  static uint32 devID = COM_UART;
  
  static T_DLCF_DEV_INFO devInfo =
  {
    .wrByte = &dlcf_rbuf_sendByte,
    .rdByte = &dlcf_rbuf_recvByte,
    .devData = &devID,
  };
  
  return devInfo;
}


