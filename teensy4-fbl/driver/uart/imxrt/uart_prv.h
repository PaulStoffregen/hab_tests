#ifndef UART_PRV_H
#define UART_PRV_H

#include "iomux.h" /* for T_IOMUX_DESC */
#include "ccm.h"   /* for T_CCM_CLK_CFG */
#include "irqc.h"  /* For T_IRQC_IRQ_CFG */
#include "rbuf.h"  /* for T_RBUF */


/*! Structure for UART controller device instance descriptor */
typedef struct
{
  uint32  base;           /*!< Base address of the device instance */
  T_IOMUX_DESC* pinCfg;   /*!< Pin configuration for device instance */
  T_CCM_CLK_CFG* clkCfg;  /*!< Clock configuration for device instance */
  T_IRQC_IRQ_CFG* irqCfg; /*!< Interrupt configuration for device instance */
  T_IRQ_HANDLER handler;  /*!< Interrupt handler for device instance */
  uint8  devID;           /*!< Device ID of the device instance */
  uint8  flags;
  uint8* txBuffer;        /*!< Pointer to transmit FIFO buffer */
  uint8* rxBuffer;        /*!< Pointer to receive FIFO buffer */
  uint16 txBufSize;       /*!< Size of transmit FIFO buffer */
  uint16 rxBufSize;       /*!< Size of receive FIFO buffer */
}T_UART_CTL_DESC;


/*! Structure for UART controller device instance runtime data */
typedef struct
{
  uint8  lock;
  uint8  state;
  uint8  error;
  uint8  flags;
  uint16 lostBytes;
  T_RBUF txFifo; /*!< Controller of TX FIFO */
  T_RBUF rxFifo; /*!< Controller of RX FIFO */
  uint8  next;
  const T_UART_CTL_DESC* props;
}T_UART_CTL_DATA;

#define UART_DEV_DATA_FLAGS_CRLF_ENA_BF   0,  1
#define UART_DEV_DATA_FLAGS_FIFO_ENA_BF   1,  1
#define UART_DEV_DATA_FLAGS_NOBLOCK_BF    2,  1

#if 0
/*! Structure for UART driver runtime data */
typedef struct
{
  T_UART_CTL_DATA* devData;
  uint8  numDevs;
  uint8  free;
}T_UART_DRV_DATA;
#endif


void uart_getRxStat(uint32 base, uint32* rxStat);
void uart_clrRxStat(uint32 base);
void uart_getTxStat(uint32 base, uint32* txStat);
void uart_clrTxStat(uint32 base);
void uart_txByte(uint32 base, uint32 flags, uint8 txByte);
void uart_rxByte(uint32 base, uint32 flags, uint8* rxByte);

#include "uart.h"
#endif /* UART_PRV_H */

