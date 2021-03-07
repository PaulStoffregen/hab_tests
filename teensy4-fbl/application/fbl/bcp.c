#ifndef BCP_C
#define BCP_C
#endif /* BCP_C */

#include "bsp.h"
#include "config.h"
#include <string.h>
#include "trace_pub.h"

#include "uart.h"

#include "pdu.h"
#include "dlcf.h"
#include "libc.h"
#include "fbl_defs.h"
#include "bcp.h"
#include "crc16.h"

#if (TRC_FEAT_BCP_ENA == STD_ON)
#define TRACE_BCP_API(...)   TRACE_FEATURE(TRC_FEAT_ID_eBCP, TRACE_FEATURE_CLASS3, __VA_ARGS__)
#define TRACE_BCP_INFO(...)  TRACE_FEATURE(TRC_FEAT_ID_eBCP, TRACE_FEATURE_CLASS2, __VA_ARGS__)
#define TRACE_BCP_ERROR(...) TRACE_FEATURE(TRC_FEAT_ID_eBCP, TRACE_FEATURE_CLASS0, __VA_ARGS__)
#define TRACE_BCP_STATE(...) TRACE_FEATURE(TRC_FEAT_ID_eBCP, TRACE_FEATURE_CLASS1, __VA_ARGS__)
#else /* (TRC_FEAT_BCP_ENA != STD_ON) */
#define TRACE_BCP_API(...)   /* empty */
#define TRACE_BCP_INFO(...)  /* empty */
#define TRACE_BCP_ERROR(...) /* empty */
#define TRACE_BCP_STATE(...) /* empty */
#endif /* (TRC_FEAT_BCP_ENA) */

#if !defined (COM_UART)
#define COM_UART 0
#endif /* !defined COM_UART */


#define BCP_HEADER_LEN 0
#define BCP_FOOTER_LEN sizeof(uint16)


typedef enum BCP_STATE
{
  BCP_STATE_eRESET = 0,
  BCP_STATE_eIDLE,
  BCP_STATE_eEXIT,
  BCP_STATE_eLISTEN,
  BCP_STATE_eRX_FAILED,
  BCP_STATE_eRX_SUCCESS,
  BCP_STATE_eRX_PENDING,
  BCP_STATE_eTX_PREPARE,
  BCP_STATE_eTX_PENDING,     
  BCP_STATE_eRECEIVE,
}T_BCP_STATE;

#define MSG_BUF_SZ_FBL_RX (1024 + 16)
#define MSG_BUF_SZ_FBL_TX (1024 + 16)

typedef struct
{
  uint8  rxBuffer[MSG_BUF_SZ_FBL_RX];
  uint8  txBuffer[MSG_BUF_SZ_FBL_TX];
  uint16 txLen;
  T_PDU  rxPdu;
  T_PDU  txPdu;
  T_BCP_STATE state;
  T_BCP_JOB_RESULT lastJobResult;
  T_DLCF_CTX dlcfCtx;
  T_CRC16_DATA crcCtx;
}T_BCP_DATA;

static T_BCP_DATA bcp_dataTbl[1];


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

boolean bcp_isFrameCrcValid(T_CRC16_DATA* ctx, T_PDU* pdu)
{
  boolean result = FALSE;
  uint16 expCrc;
  uint16 msgCrc = ( 0
                  | (pdu->data[pdu->len - 2] << 8)
                  | (pdu->data[pdu->len - 1] << 0)
                  );

  crc16_preset(ctx, 0);
  crc16_updateFTabFwd(ctx, pdu->data, pdu->len - sizeof(uint16));
  crc16_finalize(ctx, 0);
  expCrc = crc16_read(ctx);
  if(msgCrc == expCrc)
  {
    result = !FALSE;
  }
  else
  {
    TRACE_BCP_INFO("BCP: Invalid CRC: %08x (expected %08x)\n", msgCrc, expCrc);
  }
  return result;
}


/* Define control characters
 * and their escaped characters.
 * Note:
 *   For the purpose of testing we choose characters that
 *   will be included in the simple message.
 */

#define FBLCOM_CTL_BYTE_SOF 0x02
#define FBLCOM_CTL_BYTE_DLE 0x1B
#define FBLCOM_CTL_BYTE_EOF 0x03

/* SOF  ESC  EOF */
uint8 bcp_ctlBytes[] =
{
  FBLCOM_CTL_BYTE_SOF,
  FBLCOM_CTL_BYTE_DLE,
  FBLCOM_CTL_BYTE_EOF,
};

uint8 bcp_escBytes[] =
{
  FBLCOM_CTL_BYTE_SOF,
  FBLCOM_CTL_BYTE_DLE,
  FBLCOM_CTL_BYTE_EOF,
};


boolean dlcf_uart_recvByte(void* param, uint8* byte);
boolean dlcf_uart_sendByte(void* param, const uint8 byte);

/* Setup channel configuration */
T_DLCF_CFG bcp_dlcfCfg =
{
  .ctlBytes = bcp_ctlBytes,
  .escBytes = bcp_escBytes,
  .numCtlEscBytes = sizeof(bcp_ctlBytes),
};


static const uint32 bcp_devID = COM_UART;
static const T_DLCF_DEV_INFO bcp_devInfo =
{
  .wrByte = &dlcf_uart_sendByte,
  .rdByte = &dlcf_uart_recvByte,
  .devData = (void*)&bcp_devID,
};


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

T_BCP_STATUS bcp_getStatus(void)
{
  T_BCP_DATA* bcpData = bcp_dataTbl;
  T_BCP_STATUS result = BCP_STATUS_eUNINIT;

  /* Check whether complete frame has been received and was validated */
  switch(bcpData->state)
  {
  case BCP_STATE_eRESET:
    result = BCP_STATUS_eUNINIT;
    break;
    
  case BCP_STATE_eIDLE:
    result = BCP_STATUS_eIDLE;
    break;

  case BCP_STATE_eTX_PREPARE:
  case BCP_STATE_eTX_PENDING:
  case BCP_STATE_eRX_PENDING:
    result = BCP_STATUS_eBUSY;
    break;

  default:
    result = BCP_STATUS_eUNINIT;
    break;
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

T_BCP_JOB_RESULT bcp_getJobResult(void)
{
  T_BCP_DATA* bcpData = bcp_dataTbl;
  T_BCP_JOB_RESULT result = BCP_JOB_RESULT_ePENDING;

  /* Check whether complete frame has been received and was validated */
  if(BCP_STATE_eIDLE != bcpData->state)
  {
    /* Job pending */
  }
  else
  {
    result = bcpData->lastJobResult;
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Get a PDU for transmit.
 *
 ******************************************************************************
 */

T_STATUS bcp_getTxPdu(T_PDU* txPdu)
{
  T_BCP_DATA* bcpData = bcp_dataTbl;
  T_STATUS result = STATUS_eNOK;

  if(BCP_STATE_eIDLE != bcpData->state)
  {
    /* BCP is not idle */
  }
  else
  {
    /* Prepare transmit PDU */
    txPdu->data = &bcpData->txBuffer[BCP_HEADER_LEN];
    txPdu->size = sizeof(bcpData->txBuffer) - (BCP_HEADER_LEN + BCP_FOOTER_LEN);
    txPdu->len = 0;

    bcpData->state = BCP_STATE_eTX_PREPARE;
    TRACE_BCP_STATE("BCP State: IDLE -> TX_PREP\n");
    result = STATUS_eOK;
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Send a PDU containing a message.
 *
 ******************************************************************************
 */

T_STATUS bcp_sendMsg(T_PDU* txMsg)
{
  T_BCP_DATA* bcpData = bcp_dataTbl;
  T_STATUS result = STATUS_eNOK;
  uint16 msgCrc;
  T_PDU* txFrm;

  TRACE_BCP_API("bcp_sendMsg()\n");
  if(BCP_STATE_eTX_PREPARE != bcpData->state)
  {
    /* Invalid state for operation */
  }
  else
  {
    /* Prepare transmit frame */
    txFrm = &bcpData->txPdu;

    txFrm->data = bcpData->txBuffer;
    txFrm->size = sizeof(bcpData->txBuffer);
    txFrm->len = BCP_HEADER_LEN + txMsg->len;

    /* Possibly prepend BCP header
     *  ...
     */
    
    /* Calculate frame CRC */
    crc16_preset(&bcpData->crcCtx, 0);
    crc16_updateFTabFwd(&bcpData->crcCtx, txMsg->data, txMsg->len);
    crc16_finalize(&bcpData->crcCtx, 0);
    msgCrc = crc16_read(&bcpData->crcCtx);

    /* Append CRC */
    txFrm->data[txFrm->len++] = (msgCrc >> 8);
    txFrm->data[txFrm->len++] = (msgCrc >> 0);
    
    dlcf_sendPdu(&bcpData->dlcfCtx, txFrm);

    bcpData->state = BCP_STATE_eTX_PENDING;
    TRACE_BCP_STATE("BCP State: TX_PREP -> TX_PEND\n");

    result = STATUS_eOK;
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

void bcp_cancel(void)
{
  T_BCP_DATA* bcpData = bcp_dataTbl;

  TRACE_BCP_API("bcp_cancel()\n");

  bcpData->lastJobResult = BCP_JOB_RESULT_eCANCELLED;
  bcpData->state = BCP_STATE_eIDLE;
  TRACE_BCP_STATE("BCP State: ANY -> IDLE\n");
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void bcp_sendAckRsp(uint32 cmd)
{
  T_PDU txPdu;

  if(BCP_OK != bcp_getTxPdu(&txPdu))
  {
    /* Failed to get transmit PDU */
  }
  else
  {
    T_FBL_MSG_ACK_RSP* txMsg = (T_FBL_MSG_ACK_RSP*)txPdu.data;

    /* Fill message buffer */
    txMsg->msgType = FBL_MSG_ID_eACK_RSP;
    txMsg->reqType = cmd;
  
    txPdu.len = sizeof(T_FBL_MSG_ACK_RSP);

    /* Send message */
    (void)bcp_sendMsg(&txPdu);
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

void bcp_sendNakRsp(uint32 cmd, uint32 errorCode)
{  
  T_PDU txPdu;

  if(BCP_OK != bcp_getTxPdu(&txPdu))
  {
    /* Failed to get transmit PDU */
  }
  else
  {
    T_FBL_MSG_NAK_RSP* txMsg = (T_FBL_MSG_NAK_RSP*)txPdu.data;

    /* Fill message buffer */
    txMsg->msgType = FBL_MSG_ID_eNAK_RSP;
    txMsg->reqType = cmd;
    txMsg->errCode = errorCode;

    txPdu.len = sizeof(T_FBL_MSG_NAK_RSP);

    /* Send message */
    (void)bcp_sendMsg(&txPdu);
  }
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Gather receive PDU after having checked for frame reception
 *
 ******************************************************************************
 */

T_STATUS bcp_getRxMsg(T_PDU* rxMsg)
{
  T_BCP_DATA* bcpData = bcp_dataTbl;
  T_STATUS result;

  /* Prepare receive message PDU */
  rxMsg->data = &bcpData->rxBuffer[BCP_HEADER_LEN];
  rxMsg->size = sizeof(bcpData->rxBuffer) - (BCP_HEADER_LEN + BCP_FOOTER_LEN);
  rxMsg->len = bcpData->rxPdu.len - (BCP_HEADER_LEN + BCP_FOOTER_LEN);
  
  result = BCP_OK;

  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Dumps the contents of the given PDU
 *
 ******************************************************************************
 */

void bcp_dumpPdu(T_PDU* pdu)
{
  uint32 srcPos;
  uint32 width;

  TRACE_BCP_INFO("0000 :");
  width = 0;
  for(srcPos = 0; srcPos < pdu->len; srcPos++)
  {
     /* Print offset */
    if(width < 16)
    {
      /* No line break */
      width++;
    }
    else
    {
      /* */
      TRACE_BCP_INFO("\n%04X :", srcPos);
      width = 1;
    }
    TRACE_BCP_INFO(" %02X", pdu->data[srcPos]);
  }
  TRACE_BCP_INFO("\n");
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void bcp_init(void)
{
  T_BCP_DATA* bcpData = bcp_dataTbl;

  TRACE_BCP_API("bcp_init()\n");
  /* Setup DLCF */
  dlcf_configCtx(&bcpData->dlcfCtx, &bcp_dlcfCfg);
  dlcf_setDevInfo(&bcpData->dlcfCtx, &bcp_devInfo);

  /* Setup CRC16 */
  crc16_configCtx(&bcpData->crcCtx, crc16_tblP1021);

  bcpData->lastJobResult = BCP_JOB_RESULT_eOK;
  bcpData->state = BCP_STATE_eIDLE;
  TRACE_BCP_STATE("BCP State: RESET -> IDLE\n");
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void bcp_run(void)
{
  T_BCP_DATA* bcpData = bcp_dataTbl;
  T_DLCF_STATUS dlcfStatus;

//  TRACE_BCP_API("BCP: bcp_run()\n");
  switch(bcpData->state)
  {
  case BCP_STATE_eIDLE:
    /* Nothing to do */
    break;

  case BCP_STATE_eLISTEN:
  case BCP_STATE_eRX_PENDING:
#if 0
    if(UART_OK != uart_recvByte(COM_UART, &rxByte)
    {
      /* Nothing received */
    }
    else
    {
      status = dlcf_procRxByte(&bcpData->dlcfCtx, rxByte);
    }

    if(DLCF_FRAME_FINISHED == status)
    {
      /* ... */
    }
#else
    dlcf_run(&bcpData->dlcfCtx); // TODO: Separate from BCP
    dlcfStatus = dlcf_getRxStatus(&bcpData->dlcfCtx);
    if(DLCF_STATUS_eFRAME_PENDING == dlcfStatus)
    {
      /* Frame still pending */
//      TRACE_BCP_INFO("BCP: Frame pending\n");
    }
    else if(DLCF_STATUS_eFRAME_FINISHED == dlcfStatus)
    {
      /* Frame received */
      TRACE_BCP_INFO("BCP: Frame received (L=%d).\n", bcpData->rxPdu.len);
      bcp_dumpPdu(&bcpData->rxPdu);

      dlcf_clrRxStatus(&bcpData->dlcfCtx);

      if(FALSE != bcp_isFrameCrcValid(&bcpData->crcCtx, &bcpData->rxPdu))
      {
        bcpData->lastJobResult = BCP_JOB_RESULT_eOK;
        bcpData->state = BCP_STATE_eIDLE;
      }
      else
      {
        bcpData->lastJobResult = BCP_JOB_RESULT_eFAILED;
        bcpData->state = BCP_STATE_eIDLE;
      }
      TRACE_BCP_STATE("BCP State: LISTEN -> IDLE\n");
    }
    else
    {
      /* Error */
      TRACE_BCP_ERROR("BCP Error: Unexpected RX status.\n");
    }
#endif
    break;

  case BCP_STATE_eTX_PENDING:
    dlcf_run(&bcpData->dlcfCtx); // TODO: Separate from BCP
    dlcfStatus = dlcf_getTxStatus(&bcpData->dlcfCtx);
    if(DLCF_STATUS_eFRAME_PENDING == dlcfStatus)
    {
      /* Frame still pending */
    }
    else if(DLCF_STATUS_eFRAME_FINISHED == dlcfStatus)
    {
      /* Frame transmitted */
      TRACE_BCP_INFO("BCP: Frame transmitted (L=%d).\n", bcpData->txPdu.len);      
      bcp_dumpPdu(&bcpData->txPdu);
      
      dlcf_clrTxStatus(&bcpData->dlcfCtx);
      bcpData->lastJobResult = BCP_JOB_RESULT_eOK;
      bcpData->state = BCP_STATE_eIDLE;
      TRACE_BCP_STATE("BCP State: TX_PEND -> IDLE\n");
    }
    else
    {
      /* Error */
      TRACE_BCP_ERROR("BCP Error: Invalid TX status.\n");
    }
    break;

  default:
    break;
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

T_STATUS bcp_listen(void)
{
  T_BCP_DATA* bcpData = bcp_dataTbl;
  T_STATUS result = STATUS_eNOK;

  TRACE_BCP_API("BCP: bcp_listen()\n");

  if(BCP_STATE_eIDLE != bcpData->state)
  {
    /* Operation not allowed */
  }
  else
  {
    T_PDU* rxPdu = &bcpData->rxPdu;

    /* Setup the receive PDU */
    rxPdu->data = bcpData->rxBuffer;
    rxPdu->size = sizeof(bcpData->rxBuffer);
    rxPdu->len = 0;

    /* Start reception process by supplying a recive PDU. */
    dlcf_recvPdu(&bcpData->dlcfCtx, rxPdu);

    /* Enter listen state */
    bcpData->state = BCP_STATE_eLISTEN;
    TRACE_BCP_STATE("BCP: IDLE -> LISTEN\n");

    result = STATUS_eOK;
  }
  return result;
}

