#ifndef DLCF_DEV_C
#define DLCF_DEV_C
#endif /* DLCF_DEV_C */


#include "bsp.h"

#include "pdu.h"
#include "uart.h"
#include "dlcf.h"


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

boolean dlcf_uart_sendByte(void* param, const uint8 byte)
{
   boolean result;
   uint32* devID = (uint32*)param;
   
   if(UART_OK != uart_sendByte(*devID, byte))
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

boolean dlcf_uart_recvByte(void* param, uint8* byte)
{
  boolean result;
  uint32* devID = (uint32*)param;
   
  if(UART_OK != uart_recvByte(*devID, byte))
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


#if 0
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
    .wrByte = &dlcf_uart_sendByte,
    .rdByte = &dlcf_uart_recvByte,
    .devData = &devID,
  };
  
  return devInfo;
}
#endif

