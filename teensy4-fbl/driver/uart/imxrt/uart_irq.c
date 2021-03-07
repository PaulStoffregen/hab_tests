#ifndef UART_IRQ_C
#define UART_IRQ_C
#endif /* UART_IRQ_C */

#include "bsp.h"
#include "reg.h"
#include "uart_prv.h"
#include "uart_ddm.h"
#include "uart_irq.h"


#if !defined DRV_PANIC
#define DRV_PANIC(...) /* empty */
#endif /* */


/*
 ******************************************************************************
 * Function: uart_irqHandler
 ******************************************************************************
 * @par Description:
 *   This function implements the common UART interrupt handler,
 *   which is executed in order to tansmit/receive furher data from/to
 *   the ring buffer.
 *
 * @param none
 *
 * @return none
 *
 ******************************************************************************
 */

void uart_irqHandler(uint32 ctlID)
{
  T_UART_CTL_DATA* ctlData;
  T_STATUS fifoStat;
  uint32 base;
  uint32 ctrlReg;
  uint32 rxStat;
  uint32 txStat;
  uint8  rxData;
  uint8  txData;

  /* Get pointer to controller device's runtime data */
  ctlData = uart_getDevData(ctlID);
  base = uart_getDevBase(ctlID);
  if(NULL == ctlData)
  {
    DRV_PANIC();
  }
  else if(0 == base)
  {
    DRV_PANIC();
  }
  else
  {
    /* Retrieve receiver status */
    uart_getRxStat(base, &rxStat);

    /* Check for receive interrupt */
    if(0 != (rxStat & BF_MASK(LPUART_STATUS_RDRF_BF)))
    {
      uart_rxByte(base, ctlData->flags, &rxData);
    }
    else
    {
      /* No receive interrupt */
    }
    
    /* Check for receive errors */
    if(0 != (rxStat & ~(BF_MASK(LPUART_STATUS_RDRF_BF))))
    {
      /* Receiving error occured so data will be lost */
      ctlData->error = rxStat;
      ctlData->lostBytes++;
      uart_clrRxStat(base);
    }
    else if(0 == (rxStat & BF_MASK(LPUART_STATUS_RDRF_BF)))
    {
      /* Nothing received */
    }
    else if(NULL != ctlData->rxFifo.buffer)
    {
      /* Data received without errors */
      fifoStat = rbuf_wrByte(&ctlData->rxFifo, rxData);
      if(RBUF_OK != fifoStat)
      {
        /* Ring buffer is full, so data was not written and will be lost */
        /* Note: If we disable the receive interrupt here we might save
         *       some CPU cycles but we don't know how many bytes we miss.
         */
        ctlData->error |= UART_ERROR_DATA_LOST;
        ctlData->lostBytes++;
      }
      else
      {
        /* Sucessfully written to ring buffer */
      }
    }
    else
    {
      /* No receive fifo configured */
      ctlData->error |= UART_ERROR_DATA_LOST;
      ctlData->lostBytes++;
    }

    /* Retrieve transmitter status */
    uart_getTxStat(base, &txStat);
    if(0 == txStat)
    {
      /* No transmit interrupt */
    }
    else if(NULL != ctlData->txFifo.buffer)
    {
      fifoStat = rbuf_rdByte(&ctlData->txFifo, &txData);
      if(RBUF_OK != fifoStat)
      {
        /* Ring buffer is empty, so disable transmit interrupt */
        REG32_RD_BASE_OFFS(ctrlReg, base, LPUART_CTRL_OFFS);
        ctrlReg &= ~( 0
                    | BF_SET(1, LPUART_CTRL_TIE_BF)
                    );
        REG32_WR_BASE_OFFS(ctrlReg, base, LPUART_CTRL_OFFS);
      }
      else
      {
        /* Sucessfully read from ring buffer */
        uart_txByte(base, ctlData->flags, txData);
      }
    }
    else
    {
      /* No transmit fifo configured */
      REG32_RD_BASE_OFFS(ctrlReg, base, LPUART_CTRL_OFFS);
      ctrlReg &= ~( 0
                  | BF_SET(1, LPUART_CTRL_TIE_BF)
                  );
      REG32_WR_BASE_OFFS(ctrlReg, base, LPUART_CTRL_OFFS);
    }
  }
}


/*
 ******************************************************************************
 * Function: uart1_handler
 ******************************************************************************
 * @brief UART instance #1's interrupt handler entry
 *
 ******************************************************************************
 */

void uart1_handler(void)
{
  uart_irqHandler(0);
}


/*
 ******************************************************************************
 * Function: uart2_handler
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void uart2_handler(void)
{
  uart_irqHandler(1);
}


/*
 ******************************************************************************
 * Function: uart3_handler
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void uart3_handler(void)
{
  uart_irqHandler(2);
}

