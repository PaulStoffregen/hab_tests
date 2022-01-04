#ifndef UART_PRV_H
#define UART_PRV_H

#include "rbuf.h"  /* for T_RBUF */
/*! Structure for UART controller device instance descriptor */
typedef struct
{
  uint8  devID;          /*!< Device ID of the device instance */
  uint32  base;          /*!< Base address of the device instance */
  uint8  flags;
  uint8* txBuffer;       /*!< Pointer to transmit FIFO buffer */
  uint8* rxBuffer;       /*!< Pointer to receive FIFO buffer */
  uint16 txBufSize;      /*!< Size of transmit FIFO buffer */
  uint16 rxBufSize;      /*!< Size of receive FIFO buffer */
}T_UART_CTL_DESC;


/*! Structure for UART controller device instance runtime data */
typedef struct
{
  uint8  lock;
  uint8  state;
  const T_UART_CTL_DESC* props;
}T_UART_CTL_DATA;

#define UART_DEV_DATA_FLAGS_FIFO_ENA_BF   1,  1
#define UART_DEV_DATA_FLAGS_NOBLOCK_BF    2,  1

#include "uart.h"
#endif /* UART_PRV_H */

