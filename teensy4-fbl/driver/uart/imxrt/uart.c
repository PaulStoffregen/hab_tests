#ifndef UART_C
#define UART_C
#endif /* UART_C */

#include "bsp.h"
#include "reg.h"
#include "irqc.h"
#include "libc.h"
#include "config.h"
#include "uart_prv.h"
#include "uart_ddm.h"
#include "uart_irq.h"

#include <stdio.h>  // for 
#include <string.h> // for memset()


enum UART_CTL_STATE
{
   UART_CTL_STATE_eRESET = 0,
   UART_CTL_STATE_eINIT,
   UART_CTL_STATE_eIDLE,
   UART_CTL_STATE_eBUSY,
};


#if defined (STD_UART)
uint32 std_dev = STD_UART;
#elif (BSP_BOARD_TYPE == BSP_BRD_MIMXRT1060_EVK)
uint32 std_dev = 0;
#elif (BSP_BOARD_TYPE == BSP_BRD_TENSY40)
uint32 std_dev = 2;
#else /* unknown board */
uint32 std_dev = 0;
#endif /* (BSP_BOARD_TYPE) */


/*!
 ******************************************************************************
 * Function: uart_setSpeed
 ******************************************************************************
 * @par Description:
 *   This function changes UART registers to set up the speed of the clock
 *   generator.
 *
 * @param baudrate - ...
 *
 * @return status
 *
 * @retval UART_OK - succeeding operation
 *
 *                 refFreq
 * BaudRate = -----------------
 *             (OSR + 1) x SBR
 *
 ******************************************************************************
 */
// TODO: Check correctness
void uart_setSpeed(uint32 base, uint32 clockVal)
{
//  uint32 clk = 80000000;
  uint32 sbr = 8;
  uint32 osr = 25;

  /*  */
  REG32_WRBF_BASE_OFFS(osr, base, LPUART_BAUD_RATE_OFFS, LPUART_BAUD_OVER_SAMPLE_RATIO_BF);

  /*  */  
  REG32_WRBF_BASE_OFFS(sbr, base, LPUART_BAUD_RATE_OFFS, LPUART_BAUD_RATE_MODULO_DIV_BF);
}


/*!
 ******************************************************************************
 * Function: uart_setFormat
 ******************************************************************************
 * @par Description:
 *   This function sets the data format and transfer mode.
 *
 * @param data_bits - ...
 * @param stop_bits - ...
 * @param parity - ...
 *
 * @return none
 *
 ******************************************************************************
 */

void uart_setFormat(uint32 base, uint32 format)
{
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static void uart_setClockConfig(T_CCM_CLK_CFG* clkCfg)
{
  /* Enable clock gating */
  if(NULL != clkCfg)
  {
    ccm_setupMultipleClkProps(clkCfg);
  }
  else
  {
    /* Configuration error */
  }
}


/*
 ******************************************************************************
 * Function: uart_setPortConfig
 ******************************************************************************
 * @par Description:
 *   This function is used to setup the port-pins used by the UART transceiver
 *   depending on the operating mode.
 *
 * @param
 *
 * @return
 *
 ******************************************************************************
 */

static void uart_setPortConfig(T_IOMUX_DESC* portCfg)
{
  /* Setup IO-MUX */
  if(NULL != portCfg)
  {
    iomux_setupMultiplePads(portCfg);
  }
  else
  {
    /* Configuration error */
  }
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void uart_setStdDev(uint32 devID)
{
  std_dev = devID;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static void uart_setIrqConfig(const T_IRQC_IRQ_CFG* irqCfg)
{
  if(NULL != irqCfg)
  {
    irqc_setupMultipleIrqs(irqCfg);
  }
  else
  {
    /* Configuration error */
  }
}


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
  const T_UART_CTL_DESC* ctlDesc;
  T_UART_CTL_DATA* ctlData;
  T_STATUS result = UART_ERROR_INVALID;
  
  ctlDesc = uart_getDevProps(devID);
  if(NULL != ctlDesc)
  {
    /* Props available */
    result = UART_OK;
  }
  else
  {
    /* No props available */
  }

  if(UART_OK != result)
  {
    /* Previous error */
  }
  else
  {
    /* Allocate device data block */
    result = uart_allocDevData(devID);
  }
  
  if(UART_OK != result)
  {
    /* Allocation failed or previous error */
  }
  else
  {
    /* Get device data block */
    ctlData = uart_getDevData(devID);
  }

  if(UART_OK != result)
  {
    /* Previous error */
  }
  else if(NULL == ctlData)
  {
    /* Could not get runtime data */
  }
  else
  {
    /* Allocation successfull */

    /* Clear out runtime data */
    libc_memset(ctlData, 0, sizeof(T_UART_CTL_DATA));

    /* Set clock config */
    uart_setClockConfig(ctlDesc->clkCfg);

    /* Set peripheral pin configuration */
    uart_setPortConfig(ctlDesc->pinCfg);

    /* Possibly set IRQ config */
    uart_setIrqConfig(ctlDesc->irqCfg);
    irqc_registerHandler(ctlDesc->irqCfg[0].irqNum, ctlDesc->handler);

    /* Possibly set DMA config */

    /* Remember associated props */
    ctlData->props = ctlDesc;

    /* Setup transmitter software FIFO */
    if(NULL == ctlDesc->txBuffer)
    {
      /* No ring buffer used */
    }
    else if(ctlDesc->txBufSize == 0)
    {
      /* No ring buffer used */
    }
    else
    {
      /* Transmitter uses ring buffer */
      T_RBUF* fifo = &ctlData->txFifo;
      rbuf_init(fifo, ctlDesc->txBuffer, ctlDesc->txBufSize, NULL, NULL);
    }

    /* Setup receiver software FIFO */
    if(NULL == ctlDesc->rxBuffer)
    {
      /* No ring buffer used */
    }
    else if(ctlDesc->rxBufSize == 0)
    {
      /* No ring buffer used */
    }
    else
    {
      /* Receiver uses ring buffer */
      T_RBUF* fifo = &ctlData->rxFifo;
      rbuf_init(fifo, ctlDesc->rxBuffer, ctlDesc->rxBufSize, NULL, NULL);
    }
  
    /* Set controller state */
    ctlData->state = UART_CTL_STATE_eINIT;
  }
}


/*
 ******************************************************************************
 * Function: uart_getRxStat
 ******************************************************************************
 * @par Description:
 *   This function returns the receivers hardware status.
 *
 * @param
 *
 * @return
 *
 ******************************************************************************
 */

void uart_getRxStat(uint32 base, uint32* rxStat)
{
  uint32 reg;

  REG32_RD_BASE_OFFS(reg, base, LPUART_STATUS_OFFS);
  reg &= ( 0
         | BF_MASK(LPUART_STATUS_RDRF_BF)   /* Receive data register full */
         | BF_MASK(LPUART_STATUS_FE_BF)     /* Framing error */
         | BF_MASK(LPUART_STATUS_RX_OVR_BF) /* Receiver overrun */
         | BF_MASK(LPUART_STATUS_PE_BF)     /* Parity error */
         | BF_MASK(LPUART_STATUS_NE_BF)     /* Noise error */
         );
  *rxStat = reg;
}


/*
 ******************************************************************************
 * Function: uart_clrRxStat
 ******************************************************************************
 * @par Description:
 *   This function clears the receivers hardware status.
 *
 * @param
 *
 * @return
 *
 ******************************************************************************
 */

void uart_clrRxStat(uint32 base)
{
  uint32 reg;

  REG32_RD_BASE_OFFS(reg, base, LPUART_STATUS_OFFS);
#if 0
  reg &= ~( 0
          | BF_MASK(LPUART_STATUS_LIN_BRK_BF) /* Receive data register full */
          | BF_MASK(LPUART_STATUS_RX_EDGE_BF) /* Framing error */
          | BF_MASK(LPUART_STATUS_IDLE_BF)    /* Receiver overrun */
          | BF_MASK(LPUART_STATUS_MA1_BF)     /* Parity error */
          | BF_MASK(LPUART_STATUS_MA1_BF)     /* Noise error */
          );
#endif
  REG32_WR_BASE_OFFS(reg, base, LPUART_STATUS_OFFS);
}


/*
 ******************************************************************************
 * Function: uart_getTxStat
 ******************************************************************************
 * @par Description:
 *   This function returns the transmitters hardware status.
 *
 * @param
 *
 * @return
 *
 ******************************************************************************
 */

void uart_getTxStat(uint32 base, uint32* txStat)
{
  uint32 reg;
  
  REG32_RD_BASE_OFFS(reg, base, LPUART_STATUS_OFFS);
  reg &= ( 0
         | BF_MASK(LPUART_STATUS_TDRE_BF)
         );
  *txStat = reg;
}


/*
 ******************************************************************************
 * Function: uart_txByte
 ******************************************************************************
 * Description :
 *   This function directly reads the uart data register disregarding whether
 *   this might collide with previaous data.
 * Parameters  :
 *
 * Return      :
 *
 ******************************************************************************
 */

void uart_txByte(uint32 base, uint32 flags, uint8 txByte)
{
  uint32 reg;

  /* Wait for transmitter to become ready */
  do
  {    
    REG32_RD_BASE_OFFS(reg, base, LPUART_STATUS_OFFS);
  }while(0 == (reg & BF_MASK(LPUART_STATUS_TDRE_BF)));
  REG32_WR_BASE_OFFS(txByte, base, LPUART_DATA_OFFS);

  if(txByte != '\n')
  {
    /* Char isn't LF, so no special handling necessary */
  }
  else if(0 == BF_GET(flags, UART_DEV_DATA_FLAGS_CRLF_ENA_BF) ) /* && (txByte == '\n') */
  {
    /* Char was LF but mode is LF only */
  }
  else
  {
    /* Char was LF and mode is CR/LF */
    txByte = '\r';
    /* Wait for transmitter to become ready */
    do
    {    
      REG32_RD_BASE_OFFS(reg, base, LPUART_STATUS_OFFS);
    }while(0 == (reg & BF_MASK(LPUART_STATUS_TDRE_BF)));
    REG32_WR_BASE_OFFS(txByte, base, LPUART_DATA_OFFS);
  }
}


/*
 ******************************************************************************
 * Function    : uart_rxByte
 ******************************************************************************
 * Description :
 *   This function directly reads the uart data register disregarding whether
 *   it might not contain any data.
 *   this might collide with previaous data.
 *
 * Parameters  :
 *
 * Return      :
 *
 ******************************************************************************
 */

void uart_rxByte(uint32 base, uint32 flags, uint8* rxByte)
{
  uint32 reg;
  
  /* Wait for receiver to become ready */
  do
  {
    REG32_RD_BASE_OFFS(reg, base, LPUART_STATUS_OFFS);
  }while(0 == (reg & BF_MASK(LPUART_STATUS_RDRF_BF)));
  REG32_RD_BASE_OFFS(reg, base, LPUART_DATA_OFFS);

  *rxByte = (uint8)reg;

  if(*rxByte != '\r')
  {
    /* Char isn't CR */
  }
  else if(0 == BF_GET(flags, UART_DEV_DATA_FLAGS_CRLF_ENA_BF) )
  {
    /* Char was CR but mode is LF only */
  }
  else
  {
    /* Char was CR and mode is CR/LF */
    /* Wait for receiver to become ready */
    do
    {
      REG32_RD_BASE_OFFS(reg, base, LPUART_STATUS_OFFS);
    }while(0 == (reg & BF_MASK(LPUART_STATUS_RDRF_BF)));
    REG32_RD_BASE_OFFS(reg, base, LPUART_DATA_OFFS);
    *rxByte = (uint8)reg;
  }
}


/*
 ******************************************************************************
 * Function    : uart_setRxFifo
 ******************************************************************************
 * Description :
 *   This function initializes the receivers ring buffer.
 *
 * Parameters  :
 *
 * Return      :
 *
 ******************************************************************************
 */

T_STATUS uart_setRxFifo(uint32 ctlID, uint8* buffer, uint16 size)
{
  T_UART_CTL_DATA* ctlData;
  T_STATUS stat = UART_OK;

  /* Get pointer to controller device's runtime data */
  ctlData = uart_getDevData(ctlID);
  if(NULL == ctlData)
  {
    stat = UART_ERROR_INVALID;
  }
  else if(buffer == NULL)
  {
    stat = UART_ERROR_INVALID;
  }
  else if(size < 1)
  {
    stat = UART_ERROR_INVALID;
  }
  else
  {
    /* Supplied buffer is valid */
    rbuf_init(&ctlData->rxFifo, buffer, size, 0, 0);
  }
  return stat;
}


/*
 ******************************************************************************
 * Function    : uart_clrRxFifo
 ******************************************************************************
 * Description :
 *   This function removes the receiver's FIFO.
 *
 * Parameters  :
 *
 * Return      :
 *
 ******************************************************************************
 */

T_STATUS uart_clrRxFifo(uint32 ctlID)
{
  T_UART_CTL_DATA* ctlData;
  T_STATUS stat = UART_OK;

  /* Get pointer to controller device's runtime data */
  ctlData = uart_getDevData(ctlID);
  if(NULL == ctlData)
  {
    stat = UART_ERROR_INVALID;
  }
  else
  {
    rbuf_init(&ctlData->rxFifo, NULL, 0, 0, 0);
  }
  return stat;
}


/*
 ******************************************************************************
 * Function    : uart_setTxFifo
 ******************************************************************************
 * Description :
 *   This function initializes the transmitters ring buffer.
 *
 * Parameters  :
 *
 * Return      :
 *
 ******************************************************************************
 */

T_STATUS uart_setTxFifo(uint32 ctlID, uint8* buffer, uint16 size)
{
  T_STATUS stat = UART_OK;
  T_UART_CTL_DATA* ctlData;

  /* Get pointer to controller device's runtime data */
  ctlData = uart_getDevData(ctlID);
  if(NULL == ctlData)
  {
    stat = UART_ERROR_INVALID;
  }
  else if(buffer == NULL)
  {
    stat = UART_ERROR_INVALID;
  }
  else if(size < 1)
  {
    stat = UART_ERROR_INVALID;
  }
  else
  {
    /* Supplied buffer is valid */
    rbuf_init(&ctlData->txFifo, buffer, size, 0, 0);
  }
  return stat;
}


/*
 ******************************************************************************
 * Function: uart_clrTxFifo
 ******************************************************************************
 * @par Description:
 *   This function removes the transmitter's FIFO.
 *
 * @param
 *
 * @return
 *
 ******************************************************************************
 */

T_STATUS uart_clrTxFifo(uint32 ctlID)
{
  T_STATUS result = UART_ERROR_INVALID;
  T_UART_CTL_DATA* ctlData;

  /* Get pointer to controller device's runtime data */
  ctlData = uart_getDevData(ctlID);
  if(NULL == ctlData)
  {
    /* Invalid device ID */
  }
  else
  {
    rbuf_init(&ctlData->txFifo, NULL, 0, 0, 0);
    result = UART_OK;
  }
  return result;
}


/*
 ******************************************************************************
 * Function: uart_disTxFifo
 ******************************************************************************
 * @par Description:
 *   This function disables the transmitter's software FIFO.
 *
 * @param
 *
 * @return
 *
 ******************************************************************************
 */

T_STATUS uart_disTxFifo(uint32 ctlID)
{
  T_STATUS result = UART_ERROR_INVALID;
  T_UART_CTL_DATA* ctlData;

  ctlData = uart_getDevData(ctlID);
  if(NULL == ctlData)
  {
    /* Invalid device ID */
  }
  else
  {
    ctlData->flags &= ~(BF_SET(1, UART_DEV_DATA_FLAGS_FIFO_ENA_BF));
  }
  return result;
}


/*
 ******************************************************************************
 * Function: uart_enaTxFifo
 ******************************************************************************
 * @par Description:
 *   This function (re-)enables the transmitter's software FIFO.
 *
 * @param
 *
 * @return
 *
 ******************************************************************************
 */

T_STATUS uart_enaTxFifo(uint32 ctlID)
{
  T_STATUS result = UART_ERROR_INVALID;
  T_UART_CTL_DATA* ctlData;

  /* Get pointer to controller device's runtime data */
  ctlData = uart_getDevData(ctlID);
  if(NULL == ctlData)
  {
    /* Invalid device ID */
  }
  else
  {
    ctlData->flags |= (BF_SET(1, UART_DEV_DATA_FLAGS_FIFO_ENA_BF));
    result = UART_OK;
  }
  return result;
}


/*
 ******************************************************************************
 * Function: uart_enaTxBlocking
 ******************************************************************************
 * @par Description:
 *   This function enables the transmitter's blocking mode.
 *
 * @param
 *
 * @return
 *
 ******************************************************************************
 */

T_STATUS uart_enaTxBlocking(uint32 ctlID)
{
  T_STATUS result = UART_ERROR_INVALID;
  T_UART_CTL_DATA* ctlData;

  /* Get pointer to controller device's runtime data */
  ctlData = uart_getDevData(ctlID);
  if(NULL == ctlData)
  {
    /* Invalid device ID */
  }
  else
  {
    ctlData->flags &= ~(BF_SET(1, UART_DEV_DATA_FLAGS_NOBLOCK_BF));
    result = UART_OK;
  }
  return result;
}


/*
 ******************************************************************************
 * Function: uart_disTxBlocking
 ******************************************************************************
 * @par Description:
 *   This function disables the transmitter's blocking mode.
 *
 * @param
 *
 * @return
 *
 ******************************************************************************
 */

T_STATUS uart_disTxBlocking(uint32 ctlID)
{
  T_STATUS result = UART_ERROR_INVALID;
  T_UART_CTL_DATA* ctlData;

  /* Get pointer to controller device's runtime data */
  ctlData = uart_getDevData(ctlID);
  if(NULL == ctlData)
  {
    /* Invalid device ID */
  }
  else
  {
    ctlData->flags |= (BF_SET(1, UART_DEV_DATA_FLAGS_NOBLOCK_BF));
    result = UART_OK;
  }
  return result;
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
  T_UART_CTL_DATA* ctlData;
  T_STATUS stat;
  uint32   base;

  /* Get pointer to controller device's runtime data */
  ctlData = uart_getDevData(ctlID);
  base = uart_getDevBase(ctlID);
  if(NULL == ctlData)
  {
    stat = UART_ERROR_INVALID;
  }
  else if(0 == base)
  {
    stat = UART_ERROR_INVALID;
  }
  else if(ctlCfg->dataBits < 7 || ctlCfg->dataBits > 10)
  {
    stat = UART_ERROR_FORMAT;
  }
  else if(ctlCfg->stopBits < 1 || ctlCfg->stopBits > 2)
  {
    stat = UART_ERROR_FORMAT;
  }
  else if(ctlCfg->opMode & ~UART_OPMODE_MASK)
  {
    stat = UART_ERROR_FORMAT;
  }
  else if( (ctlCfg->parity != UART_PARITY_NONE) &&
           (ctlCfg->parity != UART_PARITY_EVEN) &&
           (ctlCfg->parity != UART_PARITY_ODD)   )
  {
    stat = UART_ERROR_FORMAT;
  }
  else
  {
    uint32 coreCtrl = 0;

    /* Reset UART */
    REG32_WRBF_BASE_OFFS(1, base, LPUART_GLOBAL_OFFS, LPUART_GLOBAL_RST_BF);
    /* Release reset */
    REG32_WRBF_BASE_OFFS(0, base, LPUART_GLOBAL_OFFS, LPUART_GLOBAL_RST_BF);

    /* Copy flags */
    ctlData->flags = ctlCfg->flags;
    
    /* Set UART baud rate generator speed */
    uart_setSpeed(base, ctlCfg->speed);

    /* Set frame/data format */
//    uart_setFormat(base, UART_FORMAT(ctlCfg->dataBits, ctlCfg->stopBits, ctlCfg->parity, ctlCfg->opMode & UART_MODE_POL_MASK));

    /* Check whether RX fifo is used */
    if(NULL != ctlData->rxFifo.buffer)
    {
      /* Enable receiver interrupt */
      coreCtrl |= BF_SET(1, LPUART_CTRL_RIE_BF);
    }

    /* Check whether RX mode is requested */
    if(ctlCfg->opMode & UART_OPMODE_RX)
    {
      /* Set receiver enable flag */
      coreCtrl |= BF_SET(1, LPUART_CTRL_RE_BF);
    }

    /* Check whether TX mode is requested */
    if(ctlCfg->opMode & UART_OPMODE_TX)
    {
      /* Set transmitter enable flag */
      coreCtrl |= BF_SET(1, LPUART_CTRL_TE_BF);
    }

    /* Set driver state */
    ctlData->state = UART_CTL_STATE_eIDLE;

    REG32_WR_BASE_OFFS(coreCtrl, base, LPUART_CTRL_OFFS);

    stat = UART_OK;
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
  T_UART_CTL_DATA* ctlData;
  T_STATUS drvStat = UART_OK;
  T_STATUS fifoStat;
  uint32 rxStat;
  uint32 error;
  uint32 base;

  /* Get pointer to controller device's runtime data */
  ctlData = uart_getDevData(devID);
  base = uart_getDevBase(devID);
  if(NULL == ctlData)
  {
    drvStat = UART_ERROR_INVALID;
  }
  else if(0 == base)
  {
    drvStat = UART_ERROR_INVALID;
  }
  else if(NULL != ctlData->rxFifo.buffer)
  {
    /* Receiver uses software FIFO */
    fifoStat = rbuf_rdByte(&ctlData->rxFifo, rxByte);

    /* RX interrupt will never be disabled as long as
     * the UART RX is used in interrupt mode.
     * This is with respect to count lost bytes.
     */
    if(fifoStat != RBUF_OK)
    {
      /* Receiver software FIFO is empty */
      drvStat = UART_ERROR_RX_EMPTY;
    }
  }
  else
  {
    /* Receiver doesn't use software FIFO,
     * so get receiver status
     */
    uart_getRxStat(base, &rxStat);

    /* Mask error flags */
    error = rxStat & ~(BF_MASK(LPUART_STATUS_RDRF_BF));
    if(0 != error)
    {
      /* Receive error occured */
      ctlData->error = error;
      drvStat = UART_ERROR_DATA_FAULTY;
    }
    else if(0 != (rxStat & BF_MASK(LPUART_STATUS_RDRF_BF)))
    {
      /* Receive complete flag is set */
      uart_rxByte(base, ctlData->flags, rxByte);
    }
    else
    {
      /* Receive buffer is empty */
      drvStat = UART_ERROR_RX_EMPTY;
    }
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
  T_UART_CTL_DATA* ctlData;
  T_STATUS drvStat = UART_ERROR_INVALID;
  boolean useSwFifo;
  uint32 txStat;
  uint32 base;

  /* Get pointer to controller device's runtime data */
  ctlData = uart_getDevData(devID);
  base = uart_getDevBase(devID);
  if(NULL == ctlData)
  {
    /* Invalid device ID */
  }
  else if(0 == base)
  {
    /* Invalid device ID */
  }
  else
  {
    /* Check whether usage of software FIFO is enabled */
    useSwFifo = (0 != BF_GET(ctlData->flags, UART_DEV_DATA_FLAGS_FIFO_ENA_BF));
    useSwFifo = (useSwFifo && (NULL != ctlData->txFifo.buffer));
    useSwFifo = (useSwFifo && (0 != ctlData->txFifo.size));
    drvStat = UART_OK;
  }

  if(UART_ERROR_INVALID == drvStat)
  {
    /* Invalid device ID */
  }
  else
  {
    do
    {
      if(!useSwFifo)
      {
        /* Transmitter doesn't use software FIFO */
        uart_getTxStat(base, &txStat);
        if(0 != txStat)
        {
          /* Data register is empty, so write new data
           * in direct mode.
           */
          uart_txByte(base, ctlData->flags, txByte);
          drvStat = UART_OK;
        }
        else
        {
          /* Data register is not empty */
          drvStat = UART_ERROR_TX_BUSY;
        }
      }
      else if(RBUF_OK != rbuf_wrByte(&ctlData->txFifo, txByte))
      {
        /* Software FIFO is full */
        drvStat = UART_ERROR_TX_BUSY;
      }
      else
      {
        uint32 coreCtrl;

        /* Successfully written to software FIFO */
        drvStat = UART_OK;

        /* Enable transmit interrupt */    
        REG32_RD_BASE_OFFS(coreCtrl, base, LPUART_CTRL_OFFS);
        coreCtrl |= ( 0
                 | BF_SET(1, LPUART_CTRL_TIE_BF)
                 );
        REG32_WR_BASE_OFFS(coreCtrl, base, LPUART_CTRL_OFFS);
      }

      /* Check whether blocking mode is enabled */
      if(0 != BF_GET(ctlData->flags, UART_DEV_DATA_FLAGS_NOBLOCK_BF))
      {
        break;
      }
    }while(UART_OK != drvStat);
  }
  return drvStat;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

sint16 uart_getRxFifoFill(uint32 ctlID)
{
  T_UART_CTL_DATA* ctlData;
  sint16 result = -1;

  /* Get pointer to controller device's runtime data */
  ctlData = uart_getDevData(ctlID);
  if(NULL == ctlData)
  {
    /* Invalid device ID */
  }
  else if(ctlData->rxFifo.buffer != NULL)
  {
    result = rbuf_getFill(&ctlData->rxFifo);
  }
  else
  {
    result = 0;
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

sint16 uart_getTxFifoFree(uint32 ctlID)
{
  T_UART_CTL_DATA* ctlData;
  sint16 result = -1;

  /* Get pointer to controller device's runtime data */
  ctlData = uart_getDevData(ctlID);
  if(NULL == ctlData)
  {
    /* Invalid device ID */
  }
  else if(ctlData->txFifo.buffer != NULL)
  {
    result = rbuf_getFree(&ctlData->txFifo);
  }
  else
  {
    result = 0;
  }
  return result;
}


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

int uart_puts(const char* str)
{
  while(*str)
  {
    (void)uart_putchar(*str++);
  }
  return 0;
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
