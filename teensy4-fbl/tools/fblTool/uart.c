#ifndef UART_C
#define UART_C
#endif /* UART_C */

#include "bsp.h"
#include "libc.h"

#include <windows.h>
#include <stdio.h>

#include "uart.h"

#define UART_RX_TIMEOUT     (5)
#define UART_TX_TIMEOUT     (50)

HANDLE hSerial = INVALID_HANDLE_VALUE;

/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */
#if 0
void uart_setStdDev(uint32 devID)
{
  std_dev = devID;
}
#endif

/*
 ******************************************************************************
 * Function: uart_initDev
 ******************************************************************************
 * @par Description:
 *   ...
 *
 * @param
 *
 * @return
 *
 ******************************************************************************
 */

void uart_initDev(uint32 devID)
{
  char comPortDevPath[20] = "\\\\.\\COM";
  char comPortDevNum[4];
  sprintf(comPortDevNum, "%d", devID);
  strcat(comPortDevPath, comPortDevNum);

  hSerial = CreateFile(
    comPortDevPath,
    GENERIC_READ | GENERIC_WRITE,
    0,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    NULL
  );
  
  if(hSerial == INVALID_HANDLE_VALUE)
  {
    printf("Error: wrong COM port (%d)?\n", devID);
  }
}


/*!
 ******************************************************************************
 * @fn uart_configCtl
 ******************************************************************************
 * @par Description :
 *   This function configures the UART core.
 *
 * @param speed - Baud rate setting using UART_SPEED()
 * @param dataBits - Nuber of data bits per frame 5..8
 * @param stopBits - Number of stop bits per frame 1..2
 * @param parity - Parity (even/odd/none)
 * @param opMode - Operating mode combines RX/TX, Sync/Async and polatity
 *
 * @return status
 *
 * @retval UART_OK - succeeding operation
 * @retval UART_ERROR_FORMAT - invalid format
 *
 ******************************************************************************
 */

T_STATUS uart_configCtl(uint32 ctlID, const T_UART_CTL_CFG* ctlCfg)
{
  T_STATUS stat = UART_OK;
  DCB dcbSerialParams = {0};
  COMMTIMEOUTS timeouts = {0};

  if(0 != GetCommState(hSerial, &dcbSerialParams) )
  {
    /* Success */
    dcbSerialParams.BaudRate = ctlCfg->baudrate;
    dcbSerialParams.ByteSize = ctlCfg->dataBits;
    switch(ctlCfg->stopBits)
    {
    default:
    case 1:
      dcbSerialParams.StopBits = ONESTOPBIT;
      break;
      
    case 2:
      dcbSerialParams.StopBits = TWOSTOPBITS;
      break;
    }
    
    switch(ctlCfg->parity & UART_PARITY_MASK)
    {
    case UART_PARITY_NONE:
      dcbSerialParams.Parity = NOPARITY;
      break;
      
    case UART_PARITY_EVEN:
      dcbSerialParams.Parity = EVENPARITY;
      break;
      
    case UART_PARITY_ODD:
      dcbSerialParams.Parity = ODDPARITY;
      break;
    }
    dcbSerialParams.fBinary = !FALSE;
    dcbSerialParams.fRtsControl = RTS_CONTROL_DISABLE;
    dcbSerialParams.fDtrControl = DTR_CONTROL_DISABLE;
  }
  else
  {
    stat = UART_ERROR_NOMEM;
    printf("Couldn't read config!");
  }

  if(UART_OK != stat)
  {
    /* Previous error */
  }
  else if(0 != SetCommState(hSerial, &dcbSerialParams) )
  {
    /* Success */
  }
  else
  {
    stat = UART_ERROR_NOMEM;
    printf("Couldn't write config!");
  }

  if(UART_OK != stat)
  {
    /* Previous error */
  }
  else if(0 != GetCommTimeouts(hSerial, &timeouts) )
  {
    /* Success */
    timeouts.ReadIntervalTimeout = UART_RX_TIMEOUT;
    timeouts.ReadTotalTimeoutConstant = UART_RX_TIMEOUT;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = UART_TX_TIMEOUT;
    timeouts.WriteTotalTimeoutMultiplier = 10;
  }
  else
  {
    stat = UART_ERROR_NOMEM;
    printf("Couldn't read timeouts!");
  }
  
  if(UART_OK != stat)
  {
    /* Previous error */
  }
  else if(0 != SetCommTimeouts(hSerial, &timeouts) )
  {
    /* Success */
  }
  else
  {
    stat = UART_ERROR_NOMEM;
    printf("Couldn't write timeouts!");
  }

  return stat;
}


/*
 ******************************************************************************
 * @fn uart_recvByte
 ******************************************************************************
 * @par Description :
 *   This function checks whether it is possible to read access the uart data
 *   register and if yes, it reads the data. If not, it returns with an error
 *   code.
 *
 * @param data
 * @param len
 *
 * @return Status
 *
 * @retval UART_OK
 * @retval UART_ERROR_RX_EMPTY
 * @retval UART_ERROR_DATA_FAULTY
 *
 ******************************************************************************
 */

T_STATUS uart_recvByte(uint32 devID, uint8* rxByte)
{
  T_STATUS drvStat;
  DWORD numBytesRead;
  uint8 iBuf;

  if(0 != ReadFile(hSerial, &iBuf, 1, &numBytesRead, NULL))
  {
    /* Success */
    if(1 != numBytesRead)
    {
      drvStat = UART_ERROR_RX_EMPTY;
    }
    else
    {
      *rxByte = iBuf;
      drvStat = UART_OK;
    }
  }
  else
  {
    printf("Couldn't read com port!");
    drvStat = UART_ERROR_RX_EMPTY;
  }
  
  return drvStat;
}


/*
 ******************************************************************************
 * @fn uart_sendByte
 ******************************************************************************
 * @par Description:
 *
 * @param data
 * @patam len
 *
 * @return
 *
 * @retval
 *
 ******************************************************************************
 */

T_STATUS uart_sendByte(uint32 devID, uint8 txByte)
{
  T_STATUS drvStat;
  DWORD bytesWritten;
  
  if(0 != WriteFile(hSerial, &txByte, 1, &bytesWritten, NULL))
  {
    /* Success */
    drvStat = UART_OK;
  }
  else
  {
    drvStat = UART_ERROR_TX_BUSY;
  }
  return drvStat;
}


#if 0
int uart_putchar(int c)
{
  int result = -1; /* EOF */

  if(UART_OK != uart_sendByte(std_dev, (uint8)c))
  {
  }
  else
  {
    result = c;
  }
  return result;
}


int uart_getchar(void)
{
  uint8 c;
  int result = -1; /* EOF */

  if(UART_OK != uart_recvByte(std_dev, &c))
  {
  }
  else
  {
    result = c;
  }
  return result;
}
#endif

