#ifndef DLCF_C
#define DLCF_C
#endif /* DLCF_C */

#include "bsp.h"
#include "trace_pub.h"
#include <stdio.h>
#include "libc.h"

#include "uart.h"
#include "pdu.h"
#include "dlcf.h"


#if (TRC_FEAT_DLCF_ENA == STD_ON)
#define TRACE_DLCF_API(...)   TRACE_FEATURE(TRC_FEAT_ID_eDLCF, TRACE_FEATURE_CLASS3, __VA_ARGS__)
#define TRACE_DLCF_INFO(...)  TRACE_FEATURE(TRC_FEAT_ID_eDLCF, TRACE_FEATURE_CLASS2, __VA_ARGS__)
#define TRACE_DLCF_ERROR(...) TRACE_FEATURE(TRC_FEAT_ID_eDLCF, TRACE_FEATURE_CLASS0, __VA_ARGS__)
#define TRACE_DLCF_STATE(...) TRACE_FEATURE(TRC_FEAT_ID_eDLCF, TRACE_FEATURE_CLASS1, __VA_ARGS__)
#else /* (TRC_FEAT_DLCF_ENA != STD_ON) */
#define TRACE_DLCF_API(...)   /* empty */
#define TRACE_DLCF_INFO(...)  /* empty */
#define TRACE_DLCF_ERROR(...) /* empty */
#define TRACE_DLCF_STATE(...) /* empty */
#endif /* (TRC_FEAT_DLCF_ENA) */


/*
 * Data Link Control Framing
 */

/* The callbacks shall be used to transmit or receive single characters to
 * or from real or virtual devices.
 * The underlaying device normally needs a device ID or context pointer to
 * be given to the driver function. As the driver API may vary depending on
 * the device type. So a generic callback with a static prototype might need
 * an adaption unit for every device type and maybe for every distinct device.
 *
 * So instead of using callbacks a better way might be to define a DLCF
 * function which calls an adapter function as it is done in trace_flex and
 * debug_flex.
 *
 * The advantage is, that the adapter functions are project specific part of
 * DLCF instead of the underlaying device.
 *
 * There is currently no device or channel management, so the adapter
 * functions can be static. If a channel management is demanded, the adapter
 * functions need to be setup at compile time in the channel's configuration
 * or dynamically at runtime by related functions.
 *
 * uart_sendByte(uint32 devID, uint8 byte);
 * uart_recvByte(uint32 devID, uint8* byte);
 *
 * Return UART specific result
 *
 *
 * rbuf_wrByte(T_RBUF_DESC* bufInfo, uint8 byte);
 * rbuf_rdByte(T_RBUF_DESC* bufInfo, uint8* byte);
 *
 * Return RBUF specific result 
 *
 *
 * boolean dlcf_flex_sendByte(T_DLCF_CTX* ctx, const uint8 byte)
 * {
 *   boolean result;
 *   uint32 devID = ctx->devID;
 *
 *   if(UART_OK != uart_sendByte(devID, byte))
 *   {
 *     result = FALSE;
 *   }
 *   else
 *   {
 *     result = !FALSE;
 *   }
 *   return result;
 * }
 *
 *
 * boolean dlcf_flex_recvByte(T_DLCF_CTX* ctx, uint8* byte)
 * {
 *   boolean result;
 *   uint32 devID = ctx->devID;
 *
 *   if(UART_OK != uart_recvByte(devID, byte))
 *   {
 *     result = FALSE;
 *   }
 *   else
 *   {
 *     result = !FALSE;
 *   }
 *   return result;
 * }
 *
 *
 */

/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static boolean dlcf_sendByte(T_DLCF_CTX* ctx, uint8 byte)
{
  const T_DLCF_DEV_INFO* devInfo = ctx->devInfo;
  return devInfo->wrByte(devInfo->devData, byte);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief 
 *
 ******************************************************************************
 */

static boolean dlcf_recvByte(T_DLCF_CTX* ctx, uint8* byte)
{
  const T_DLCF_DEV_INFO* devInfo = ctx->devInfo;
  return devInfo->rdByte(devInfo->devData, byte);
}


/*
 ******************************************************************************
 * Function: dlcf_configCtx
 ******************************************************************************
 * @brief Configure the given DLCF context
 *
 * @param [out] ctx - DLCF context
 * @param [in] cfg - The configuration for the DLCF channel
 *
 ******************************************************************************
 */
 
void dlcf_configCtx(T_DLCF_CTX* ctx, T_DLCF_CFG* cfg)
{
  TRACE_DLCF_API("dlcf_configCtx()\n");
  ctx->cfg = cfg;
  ctx->txCbk = NULL;
  ctx->rxCbk = NULL;
  ctx->txState = DLCF_TX_STATE_eCONFIG;
  ctx->rxState = DLCF_RX_STATE_eIDLE;
  ctx->devInfo = NULL;
  TRACE_DLCF_STATE("DLCF RX: RESET -> IDLE\n");
  TRACE_DLCF_STATE("DLCF TX: RESET -> CONFIG\n");  
}


/*
 ******************************************************************************
 * Function: dlcf_setupDevInfo
 ******************************************************************************
 * @brief Setup underlaying device info
 *
 ******************************************************************************
 */

T_STATUS dlcf_setDevInfo(T_DLCF_CTX* ctx, const T_DLCF_DEV_INFO* devInfo)
{
  T_STATUS result;
  
  TRACE_DLCF_API("dlcf_setDevInfo()\n");
  switch(ctx->txState)
  {
  case DLCF_TX_STATE_eCONFIG:
  case DLCF_TX_STATE_eIDLE:
  case DLCF_TX_STATE_eFINISHED:
    /* Device setup is not allowed when active */
    ctx->txState = DLCF_TX_STATE_eIDLE;
    ctx->devInfo = devInfo;
    result = DLCF_OK;
    TRACE_DLCF_STATE("DLCF TX: CONFIG -> IDLE\n");
    break;

  default:
    result = DLCF_ERROR_INVALID;
    break;
  }
  return result;
}


/*
 ******************************************************************************
 * Function: dlcf_sendPdu
 ******************************************************************************
 * @brief Submit a PDU to be sent
 *
 * @param [out] ctx - DLCF context
 * @param [in] txPdu - PDU to be sent
 *
 ******************************************************************************
 */
 
T_STATUS dlcf_sendPdu(T_DLCF_CTX* ctx, T_PDU* txPdu)
{
  T_STATUS result = DLCF_FRAME_PENDING;

  switch(ctx->txState)
  {
  case DLCF_TX_STATE_eIDLE:
  case DLCF_TX_STATE_eFINISHED:
    if(txPdu == NULL)
    {
      /* Invalid PDU */
      result = DLCF_ERROR_INVALID;
    }
    else if(txPdu->data == NULL)
    {
      /* Invalid data */
      result = DLCF_ERROR_INVALID;
    }
    else if(txPdu->len == 0)
    {
      /* Invalid length */
      result = DLCF_ERROR_INVALID;
    }
    else if(NULL == ctx->devInfo)
    {
      /* Invalid write callback */
      result = DLCF_ERROR_INVALID;
    }
    else if(NULL == ctx->devInfo->wrByte)
    {
      /* Invalid write callback */
      result = DLCF_ERROR_INVALID;
    }
    else
    {
      /* Lock context */
      ctx->txState = DLCF_TX_STATE_eSOF;
      ctx->txData = txPdu->data;
      ctx->txLen = txPdu->len;
      ctx->txPos = 0u;
      // TODO: Here we might execute the first transmission
      result = DLCF_OK;
    }
    break;

  default:
    break;
  }
  return result;
}


/*
 ******************************************************************************
 * Function: dlcf_recvPdu
 ******************************************************************************
 * @brief Supply a PDU to receive to
 *
 * @param [out] ctx - DLCF context
 * @param [in] rxPdu - PDU to be received
 *
 ******************************************************************************
 */
 
T_STATUS dlcf_recvPdu(T_DLCF_CTX* ctx, T_PDU* rxPdu)
{
  T_STATUS result = DLCF_FRAME_PENDING;

  TRACE_DLCF_API("dlcf_recvPdu()\n");
  switch(ctx->rxState)
  {
  case DLCF_RX_STATE_eIDLE:
  case DLCF_RX_STATE_eFINISHED:
    if(rxPdu == NULL)
    {
      /* Invalid PDU */
      result = DLCF_ERROR_INVALID;
    }
    else if(rxPdu->data == NULL)
    {
      /* Invalid data */
      result = DLCF_ERROR_INVALID;
    }
    else if(rxPdu->size == 0)
    {
      /* Invalid size */
      result = DLCF_ERROR_INVALID;
    }
    else if(NULL == ctx->devInfo)
    {
      /* Invalid write callback */
      result = DLCF_ERROR_INVALID;
    }
    else if(NULL == ctx->devInfo->rdByte)
    {
      /* Invalid read callback */
      result = DLCF_ERROR_INVALID;
    }
    else
    {
      /* Lock context */
      ctx->rxState = DLCF_RX_STATE_eSOF; /* Wait for reception of SOF */
      ctx->rxPdu = rxPdu;
      ctx->rxPos = 0u;
      // TODO: Here we might execute the first reception
      result = DLCF_OK;
      TRACE_DLCF_STATE("DLCF RX: IDLE -> SOF\n");
    }
    break;

  default:
    break;
  }
  return result;
}


/*
 ******************************************************************************
 * Function: dlcf_getRxStatus
 ******************************************************************************
 * @brief Retrieve receive status
 *
 * @param [in] ctx - DLCF context
 *
 ******************************************************************************
 */
 
T_DLCF_STATUS dlcf_getRxStatus(T_DLCF_CTX* ctx)
{
  T_DLCF_STATUS result = DLCF_STATUS_eFRAME_PENDING;
  
  switch(ctx->rxState)
  {
  case DLCF_RX_STATE_eFINISHED:
    result = DLCF_STATUS_eFRAME_FINISHED;
    break;

  case DLCF_RX_STATE_eIDLE:
    result = DLCF_STATUS_eOK;
    break;

  default:
    /* Reception not finished,
     * keep result.
     */
    break;
  }
  return result;
}


/*
 ******************************************************************************
 * Function: dlcf_clrRxStatus
 ******************************************************************************
 * @brief Clear receive status
 *
 * @param [in] ctx - DLCF context
 *
 ******************************************************************************
 */

void dlcf_clrRxStatus(T_DLCF_CTX* ctx)
{
  switch(ctx->rxState)
  {
  case DLCF_RX_STATE_eFINISHED:
    ctx->rxState = DLCF_RX_STATE_eIDLE;
    break;

  default:
    /* Reception not finished or still idle,
     * keep state.
     */
    break;
  }
}


/*
 ******************************************************************************
 * Function: dlcf_getTxStatus
 ******************************************************************************
 * @brief Retrieve transmit status
 *
 * @param [out] ctx - DLCF context
 *
 ******************************************************************************
 */
 
T_DLCF_STATUS dlcf_getTxStatus(T_DLCF_CTX* ctx)
{
  T_DLCF_STATUS result = DLCF_STATUS_eFRAME_PENDING;
  
  switch(ctx->txState)
  {
  case DLCF_TX_STATE_eFINISHED:
  case DLCF_TX_STATE_eIDLE:
    result = DLCF_STATUS_eFRAME_FINISHED;
    break;

  case DLCF_TX_STATE_eSOF:
  case DLCF_TX_STATE_eDATA:
  case DLCF_TX_STATE_eESC:
  case DLCF_TX_STATE_eEOF:
    /* Transmission still in progress,
     * keep result.
     */
    break;

  default:
    break;
  }
  return result;
}


/*
 ******************************************************************************
 * Function: dlcf_clrTxStatus
 ******************************************************************************
 * @brief Clear transmit status
 *
 * @param [in] ctx - DLCF context
 *
 ******************************************************************************
 */

void dlcf_clrTxStatus(T_DLCF_CTX* ctx)
{
  switch(ctx->txState)
  {
  case DLCF_TX_STATE_eFINISHED:
    ctx->txState = DLCF_TX_STATE_eIDLE;
    break;

  default:
    /* Transmission not finished or still idle,
     * keep state.
     */
    break;
  }
}


/*
 * In the sender a given message with a given length (defined by a
 * character string) needs to be scanned for all defined control
 * characters which may be, more or less, depending on the used framing.
 * For every control character there exists an associated escape
 * character which generally may be definded in one of two ways:
 * 1. As an mathematical or logical operation applied to the
 *    control character (e.g. ESC = CTL XOR 0x20)
 * 2. As an arbitrary character (e.g. CTL = 0x7E -> ESC = 0x5E)
 *
 * In the present moule the latter aproach will be used as it is more
 * GENERIC and the escape characters associated with the control
 * characters will be defined statically, so that no mathematical
 * operation needs to be executed.
 * For every occurence of a control character within the message
 * the frame will contain a DLE character followed by the associated
 * escape character.
 *
 * HDLC-like framing defines a sync sequence (called flag sequence) of a
 * single byte: SOF = 0x7E
 *
 * Further it defines an escape character (called control escape octet)
 * with the value: DLE = 0x7D
 *
 * So both are so called control characters, which need to be escaped if
 * contained in the message.
 *
 * 0x7E -> DLE / ESC(SOF) [0x7D 0x5E]
 * 0x7D -> DLE / ESC(DLE) [0x7D 0x5D]
 *
 * STX/ETX framing defines control characters for start and end of frame
 * as single bytes:
 * STX = 0x02
 * ETX = 0x03
 *
 * Further it defines a an escape character with the following value:
 * DLE = 0x10
 *
 * Those 3 characters are called control characters, which need to be
 * escaped is contained in the message.
 *
 * 0x02 (STX) -> DLE / STX [0x10 0x02] 
 * 0x03 (ETX) -> DLE / ETX [0x10 0x03] 
 * 0x10 (DLE) -> DLE / DLE [0x10 0x10] 
 *
 * ASCII control characters:
 * 0x00 : NUL .. Null
 * 0x01 : SOH .. Start of Heading:
 *               Marks the begin of machine readable address or routing
 *               information and is closed by STX
 * 0x02 : STX .. Start of Text:
 *               Marks the begin of the message to be transfered
 * 0x03 : ETX .. End of Text:
 *               Marks the end of the message to be transfered
 * 0x04 : EOT .. End of Transmission:
 *               Marks the end of the whole transmission
 * 0x06 : ACK .. Acknowledge
 * 0x0A : LF  .. Line Feed
 * 0x0D : CR  .. Carridge Return
 * 0x10 : DLE .. Data Link Escape
 * 0x15 : NAK .. Negative Acknowledge
 *
 * Character Stuffing:
 * - Reserved control characters mark begin and end of a message
 *
 * Transparency:
 * - Control characters within the message are made non control
 *   characters by prepending them with another control character DLE.
 * - The DLE, when occuring within the message is also prepended by DLE.
 *
 * Ich kann prinzipielle so lang über eine Nachricht traversieren, bis
 * ich ein Kontroll-Zeichen erkenne. Dann muss der bisher traversierte
 * Teil der Nachricht weitergereicht werden, gefolgt von DLE und CTL-ESC.
 * Das bedeutet jedoch ein aufwendiges management der buffer.
 * Im einfachsten Fall wird jedoch jedes Zeichen einzeln weitergereicht.
 *
 * In Senderichtung kann der nächst niedrigere Layer z.B. eine Queue
 * implementieren oder direkt die Zeichen entgegennehmen und
 * senden / verarbeiten.
 *
 * In Empfangsrichtung kann die receive()-Funktion:
 * - entweder dirkt zyklisch den nächst niedrigeren Layer nach neuen
 *   Daten pollen und jedes empfangene Byte direkt verarbeiten
 * - oder aber es wird ein Callback registriert, der wiederholt vom
 *   niedrigeren Layer aufgerufen wird, wenn dieser Daten empfangen hat.
 * 
 */

/* Das senden eines Frames erfolgt in 3 Schritten:
 * 1. Senden des Start Of Frame (SOF) + Header
 * 2. Senden der Payload
 * 3. Senden des Footers + End of Frame (EOF)
 *
 * Eine zu Übertragende Nachricht besteht mindestens aus den zu
 * übertragenden Daten, die im besten Fall als einzelne PDU Struktur
 * vorliegen. Es ist jedoch auch denkbar, dass eine Nachricht aus
 * mehreren PDUs besteht.
 * Beinhaltet das Protokoll neben SOF/EOF weitere Protocol Control
 * Infromation (PCI) wie Adress-Info, Frame-Type, Sequence Number o.Ä.,
 * so müssen diese entweder Teil des Nachrichten-Objekts sein oder
 * als weitere Parameter oder durch separate Funktionsaufrufe übergeben
 * werden.
 * Je nach Protokoll kann eine CRC-Berechnung Teil der Übertragung sein.
 * Diese muss in dem Falle ebenfalls während der Übertragung erfolgen.
 *
 * Bei der Aufteilung in die oben genannten 3 Schritte via separater
 * Funktionsaufrufe, kann die Zusammenstellung der PCI und die
 * Berechnung der CRC außerhalb der generischen Framing-Funktionen
 * erfolgen. Diese können dann ebenfalls als PDU übergeben werden.
 * 
 * Sollen diese Aufgeben von den generischen Framing-Funktionen
 * wahrgenommen werden, so muss eine geeignete Beschreibungs-Form
 * gefunden werden, um vorhandensein, Ort u. Größe der jeweiligen
 * Komponenten zu spezifizieren.
 * Offs + Len
 *
 * Die generische Funktion setAddress muss dann wissen, wie viele
 * Bits und welche aus der übergebenen Adresse entnommen und an
 * welcher Stelle im Frame diese eingetragen werden müssen.
 *
 * In den meisten Fällen wird das Framing für Zeichen-basierte
 * Kommunikation eingesetzt werden. Insofern ist es unpraktisch,
 * die Daten beim Senden zunächst in ein weiteres Paket zu kopieren,
 * bevor sie versandt werden. Dadurch würde weiterer Speicher in
 * mindestens der gleichen Größe wie die eigentliche Nachricht
 * benötigt. Außerdem wäre ein weiterer Kopiervorgang erfoderlich.
 *
 * Es bietet sich also an, die Zeichen-basierte Sende-Funktion
 * des darunterliegenden Treibers bzw. der darunterliegenden
 * Protokollschicht aufzurufen.
 *
 * Beim empfangen gilt das gleiche.
 */
 
/* The sendFrame function is called to encode a message into a
 * network frame.
 *
 * The recvFrame function is called to decode a network frame
 * into a message.
 */
 
#if 0
uint16 dlcf_sendFrame(T_DLCF_CTX* ctx, T_PDU* txPdu)
{
  boolean sent;
  uint16 msgPos;
  uint8  ccID;
  ctx->txMsg = txPdu->data;
  ctx->txLen = txPdu->len;

  /* Iterate over the message to be sent and look for any control bytes
   * contained within the message.
   */
  for(msgPos = 0; msgPos < ctx->txLen; msgPos++)
  {
    sent = FALSE;
    
    /* Compare every byte in the message with all defined control characters. */
    for(int k = 0; k < ctx->cfg.numCtlEscBytes; k++)
    {
      if(ctx->txMsg[msgPos] == ctx->cfg.ctlBytes[k])
      {
        /* Detected control character, so send DLE followed by
         * the detected control character's escape character
         */
        ll_sendByte(ctx->cfg.ctlBytes[0]);
        ll_sendByte(ctx->cfg.escBytes[k];
        
        /* Remember that we already sent something for the current byte. */
        sent = !FALSE;
        /* Leave the inner loop. */
        break;
      }
      else
      {
        /* Try next defined control byte */
      }
    }

    /* Check if we already sent something for the current byte. */
    if(!sent)
    {
      /* Nothing yet sent, so the message's current character is not
       * an control character
       */
      ll_sendByte(ctx->txMsg[msgPos]);
    }
  }
}
#endif


#if 0
uint16 dlcf_recvFrame(T_DLCF_CTX* ctx, const uint8* msg, uint16 len)
{
  uint8 byte;
  boolean isDle = FALSE;
  ctx->rxMsg = msg;
  ctx->rxLen = len;
  
  for(int i = 0; i < ctx->rxLen; i++)
  {
    /* Get message's current byte */
    byte = ctx->rxMsg[i];

    /* Check whether the message's recent character was a DLE */
    if(isDle)
    {
      /* Previous character was the DLE, so compare current byte with
       * all the defined escape bytes.
       */
      for(int k = 0; k < ctx->cfg.numCtlEscBytes; k++)
      {
        if(byte == ctx->cfg.escBytes[k])
        {
          /* Current character is a escape character, so replace it
           * with its associated control character.
           */
          byte = ctx->cfg.ctlBytes[k];
          isDle = FALSE;
          break;
        }
      }

      if(!isDle)
      {
        /* We left the loop with escape character replaced by its
         * associated control character.
         */
        ll_recvByte(byte);
      }
      else
      {
        /* Unknown escape character detected */
      }
    }
    else if(byte == ctx->cfg.ctlBytes[0])
    {
      /* Detected data link escape character. */
      isDle = !FALSE;
    }
    else
    {
      ll_recvByte(byte);
    }
  }
}
#endif

#if !3
uint16 dlcf_continueTxFrame(T_DLCF_CTX* ctx, T_PDU* dstPdu, T_PDU* srcPdu, uint16 srcOffs)
{
  boolean ctrlDetected;
  uint8  byte;
  uint16 srcPos = srcOffs;
  uint16 srcLen = srcPdu->len - srcPos;
  uint16 dstPos = dstPdu->len;
  uint16 dstSpace = dstPdu->size - dstPos;
  uint8  ctrlID;
  uint16 bytesToWrite;

  /* Iterate over the message to be sent and look for any control bytes
   * contained within the message.
   */
  while(srcLen > 0)
  {
    byte = srcPdu->data[srcPos];
    
    /* Compare every byte in the message with all defined control characters. */
    bytesToWrite = 1;
    ctrlDetected = FALSE;
    for(ctrlID = 0; ctrlID < ctx->cfg->numCtlEscBytes; ctrlID++)
    {
      if(byte != ctx->cfg->ctlBytes[ctrlID])
      {
        /* Try next defined control byte */
      }
      else
      {
        /* Detected control character, so DLE followed by
         * the detected control character's escape character
         * needs to be written.
         */
        bytesToWrite++;

        /* Leave the inner loop. */
        break;
      }
    }

    /* Check if the destination PDU has enough space left to write
     * as much as we need to write.
     */
    if(bytesToWrite > dstSpace)
    {
      /* Not enough space */
      break;
    }
    else if(bytesToWrite > 1)
    {
      /* Nothing yet sent, so the message's current character is not
       * an control character
       */
    }
    else
    {
      /* Detected control character, so send DLE followed by
       * the detected control character's escape character
       */
      dstPdu->data[dstPos++] = ctx->cfg->ctlBytes[0];
      byte = ctx->cfg->ctlBytes[ctrlID];
    }
    dstPdu->data[dstPos++] = byte;
    dstSpace -= bytesToWrite;
    srcPos++;
    srcLen--;
  }
  dstPdu->len = dstPos;

  return srcPos;
}
#endif

#if !4
uint16 dlcf_recvFrame(T_DLCF_CTX* ctx, T_PDU* dstPdu, T_PDU* srcPdu, uint16 srcOffs)
{
  uint8 byte;
  boolean dleDetected = FALSE;
  uint16 srcPos = srcOffs;
  uint16 srcLen = srcPdu->len - srcPos;
  uint16 dstPos = dstPdu->len;
  uint16 dstSpace = dstPdu->size - dstPos;
  uint8  ctrlID;
  uint16 bytesToWrite;
  
  while(srcLen > 0)
  {
    if(dstSpace < 1)
    {
      break;
    }

    /* Get message's current byte */
    byte = srcPdu->data[srcPos++];
    srcLen--;

    /* Check whether the message's recent character was a DLE */
    if(dleDetected)
    {
      /* Previous character was the DLE, so compare current byte with
       * all the defined escape bytes.
       */
      for(ctrlID = 0; ctrlID < ctx->cfg.numCtlEscBytes; ctrlID++)
      {
        if(byte == ctx->cfg.escBytes[ctrlID])
        {
          /* Current character is a escape character, so replace it
           * with its associated control character.
           */
          byte = ctx->cfg.ctlBytes[ctrlID];

          /* Remove DLE flag */
          dleDetected = FALSE;
          break;
        }
      }

      if(!dleDetected)
      {
        /* We left the loop with escape character replaced by its
         * associated control character.
         */
        bytesToWrite = 1;
      }
      else
      {
        /* Unknown escape character detected */
      }
    }
    else if(byte == ctx->cfg.ctlBytes[0])
    {
      /* The previous character was not the DLE but now we detected a DLE. */
      dleDetected = !FALSE;
    }
    else
    {
      /* Neither the previous nor the current character is a DLE */
      bytesToWrite = 1;
    }

    if(bytesToWrite > 0)
    {
      dstPdu->data[dstPos++] = byte;
      dstSpace--;
    }
  }
  dstPdu->len = dstPos;

  return srcPos;
}
#endif


T_STATUS dlcf_procTxByte(T_DLCF_CTX* ctx, uint8* txByte)
{
  T_STATUS result = DLCF_OK;

  /* Check if any data has to be sent */
  switch(ctx->txState)
  {
  case DLCF_TX_STATE_eSOF:
    /* Send start of frame */
    *txByte = ctx->cfg->ctlBytes[DLCF_CTL_BYTE_ID_eSOF];
    ctx->txState = DLCF_TX_STATE_eDATA;
    TRACE_DLCF_INFO("DLCF ENC: SOF=%02x\n", *txByte);
    break;

  case DLCF_TX_STATE_eDATA:
    /* Send data */
    do
    {
      if(ctx->txPos < ctx->txLen)
      {
        /* There are further bytes to be transmitted. */
        uint8 ctrlID;

        *txByte = ctx->txData[ctx->txPos++];
        for(ctrlID = 0; ctrlID < ctx->cfg->numCtlEscBytes; ctrlID++)
        {
          if(*txByte != ctx->cfg->ctlBytes[ctrlID])
          {
            /* Try next defined control byte */
          }
          else
          {
            /* Detected control character, so DLE followed by
             * the detected control character's escape character
             * needs to be written.
             */
            *txByte = ctx->cfg->ctlBytes[DLCF_CTL_BYTE_ID_eESC];
            /* Remember the control character's ID */
            ctx->ctlID = ctrlID;
            ctx->txState = DLCF_TX_STATE_eESC;
            /* Leave the for-loop. */
            break;
          }
        }

#if 0
        if(ctx->txState != DLCF_TX_STATE_eESC)
        {
          printf("TX: CHR=%02x\n", *txByte);
        }
        else
        {
          printf("TX: ESC=%02x\n", *txByte);
        }
#endif
      }
      else 
      {
        /* No more bytes to be transmitted. */
        ctx->txState = DLCF_TX_STATE_eEOF;

        /* If registered execute callback function */
        if(NULL != ctx->txCbk)
        {
          /* The callback might continue the transmission */
          ctx->txCbk();
        }

        /* Check whether transfer continues */
        if(ctx->txState != DLCF_TX_STATE_eEOF)
        {
          /* Transfer continues */
          continue;
        }
        else
        {
          /* Transfer definately finished, so send end of frame */
          *txByte = ctx->cfg->ctlBytes[DLCF_CTL_BYTE_ID_eEOF];
          TRACE_DLCF_INFO("DLCF ENC: EOF=%02x\n", *txByte);
          ctx->txState = DLCF_TX_STATE_eFINISHED;
        }
      }
    }while(0);
    break;

  case DLCF_TX_STATE_eESC:
    *txByte = ctx->cfg->escBytes[ctx->ctlID];
    TRACE_DLCF_INFO("DLCF ENC: XCHR (%02x -> %02x)\n", ctx->cfg->ctlBytes[ctx->ctlID], *txByte);
    ctx->txState = DLCF_TX_STATE_eDATA;
    break;

  default:
    result = DLCF_FRAME_PENDING;
    break;
  }
  return result;
}




/* gets a character or a STX - DLE encapsulated block from the RX queue
 * ATTENTION: non blocking function
 *
 * if the first received character is NOT a STX it returns with a length of 1
 * if the first received character is a STX this function leaves the idle state
 * and waits for a ETX in subsequent calls.
 * the leading DLE prefixes are removed and the net data is stored to the
 * buffer regarding the buffer length.
 * the function returns the type of received data (single char or block)
 *
 * CRC16 is calculated during reception, the calling instance has to take care
 * of this.
 *
 * ATTENTION: the control structure shall be initialized in advance
 */

T_STATUS dlcf_procRxByte(T_DLCF_CTX* ctx, uint8 rxByte)
{
  /* Assume receiver pending */
  T_STATUS result = DLCF_FRAME_PENDING;

  switch(ctx->rxState)
  {
  case DLCF_RX_STATE_eSOF:
    /* Receiver is waiting for SOF */
    if(ctx->cfg->ctlBytes[DLCF_CTL_BYTE_ID_eSOF] == rxByte)
    {
      TRACE_DLCF_INFO("DLCF DEC: SOF=%02x\n", rxByte);
      /* Start of frame detetcted */
      ctx->rxPos = 0u;
      /* Switch to data receive state */
      ctx->rxState = DLCF_RX_STATE_eDATA;
      
      TRACE_DLCF_STATE("DLCF RxState: SOF -> DATA\n");
    }
    else
    {
      /* Ignore any other character */
      TRACE_DLCF_INFO("DLCF DEC: IGN=%02x\n", rxByte);
      result = DLCF_BYTE_RECEIVED;
    }
    break;

  case DLCF_RX_STATE_eDATA:
    /* Check for frame restart */
    if(ctx->cfg->ctlBytes[DLCF_CTL_BYTE_ID_eSOF] == rxByte)
    {
      /* Restart of frame detetcted */
      TRACE_DLCF_INFO("DLCF DEC: RSOF=%02x\n", rxByte);
      ctx->rxPos = 0u;
      TRACE_DLCF_INFO("DLCF RxState: RSOF -> DATA\n");
    }
    /* Check for DLE character */
    else if(ctx->cfg->ctlBytes[DLCF_CTL_BYTE_ID_eESC] == rxByte)
    {
      /* DLE detected, so switch to ESC state */
      TRACE_DLCF_INFO("DLCF DEC: DLE=%02x\n", rxByte);
      ctx->rxState = DLCF_RX_STATE_eESC;
      TRACE_DLCF_STATE("DLCF RxState: DATA -> ESC\n");
    }
    /* Check for end of frame */
    else if(ctx->cfg->ctlBytes[DLCF_CTL_BYTE_ID_eEOF] == rxByte)
    {
      /* End of frame received */
      TRACE_DLCF_INFO("DLCF DEC: EOF=%02x\n", rxByte);
      ctx->rxPdu->len = ctx->rxPos;
      ctx->rxState = DLCF_RX_STATE_eFINISHED;
      
      /* Leave function with frame received */
      result = DLCF_FRAME_FINISHED;
      
      TRACE_DLCF_STATE("DLCF RxState: DATA -> FIN\n");
    }
    /* Any non-control-caracter received
     * Check for enough remaining space
     */
    else if(ctx->rxPos < ctx->rxPdu->size)
    {
      /* Enough space left */
      TRACE_DLCF_INFO("DLCF DEC: CHR=%02x\n", rxByte);
      ctx->rxPdu->data[ctx->rxPos] = rxByte;
      ctx->rxPos++;
    }
    else
    {
      /* Buffer overflow - switch to error state */
      TRACE_DLCF_INFO("DLCF ERR: OVL=%02x\n", rxByte);
      ctx->rxState = DLCF_RX_STATE_eERROR;
      
      TRACE_DLCF_STATE("DLCF RxState: DATA -> ERR\n");
    }
    break;

  case DLCF_RX_STATE_eESC:
    /* Check for space in buffer - otherwise ignore data */
    if(ctx->rxPos < ctx->rxPdu->size)
    {
      uint8 ctrlID;
       
      /* Enough space left,
       * iterate over escaped control bytes and compare with received character.
       */
      for(ctrlID = 0; ctrlID < ctx->cfg->numCtlEscBytes; ctrlID++)
      {
        if(rxByte != ctx->cfg->escBytes[ctrlID])
        {
          /* Try next defined control byte */
        }
        else
        {
          /* Detected escaped control character, so the unescaped
           * control character needs to be written.
           */
          rxByte = ctx->cfg->ctlBytes[ctrlID];
          TRACE_DLCF_INFO("DLCF DEC: XCHR (%02x -> %02x)\n", ctx->cfg->escBytes[ctrlID], rxByte);
          ctx->rxState = DLCF_RX_STATE_eDATA;

          TRACE_DLCF_STATE("DLCF RxState: ESC -> DATA\n");
          /* Leave the loop. */
          break;
        }
      }
      // TODO: What if the received charater is not a escaped control character?
      ctx->rxPdu->data[ctx->rxPos] = rxByte;
      ctx->rxPos++;
    }
    else
    {
      /* Buffer overflow - switch to error state */
      TRACE_DLCF_INFO("DLCF ERR: OVL=%02x\n", rxByte);
      ctx->rxState = DLCF_RX_STATE_eERROR;
      
      TRACE_DLCF_STATE("DLCF RxState: ESC -> ERR\n");
    }
    break;

  case DLCF_RX_STATE_eERROR:
    /* Wait for end of frame */
    if(ctx->cfg->ctlBytes[DLCF_CTL_BYTE_ID_eEOF] == rxByte)
    {
      TRACE_DLCF_INFO("DLCF DEC: EOF=%02x\n", rxByte);
      ctx->rxPos = 0u;
      ctx->rxState = DLCF_RX_STATE_eIDLE;
      result = DLCF_FRAME_FINISHED; // TODO: ???
    }
    else
    {
      TRACE_DLCF_INFO("DLCF DEC: ERR=%02x\n", rxByte);
    }
    break;

  default:
    /* Not in frame reception */
    ctx->rxState = DLCF_RX_STATE_eIDLE;
    result = DLCF_BYTE_RECEIVED;
    break;
  }
  return result;
}


#if !5
/* puts a STX - ETX encapsulated block to the TX queue
 * ATTENTION: blocking function
 *
 * the first character is STX, the last character is ETX.
 * if these control characters contained in the block, they will be
 * prefixed by DLE, DLE is prefixed by DLE also.
 *
 * CRC16 is calculated during transmit and added at the end.
 */

void uarttp_sendBlockBlocking(uint32 devID, T_PDU* txPdu)
{
   uint16 crc16;
   uint16 idx;
   uint8  tmpU8;

   /* Check if any data has to be sent */
   if(txPdu->len > 0u)
   {
      /* Send start of block */
      sendCbk(startSeq);
//      uarttp_sendByteBlocking(channel, UARTTP_STX);
//      crc16 = CRC_INITIAL_VALUE16;
      crc16_preset(crcCtx, CRC_INITIAL_VALUE16);

      /* Send block */
      for(idx = 0; idx < txPdu->len; idx++)
      {
         tmpU8 = txPdu->data[idx];
         uarttp_SendBytePrefix(channel, tmpU8);
         crc16 = crc_calculateCRC16(&tmpU8, 1, crc16);
      }

      
      /* send CRC16 */
      uarttp_SendBytePrefix(channel, (uint8)(crc16 >> 8));
      uarttp_SendBytePrefix(channel, (uint8)(crc16 & 0xFFu));

      /* send end of block */
      uarttp_sendByteBlocking(channel, UARTTP_ETX);
   }
}
#endif

#if !6
static bool dlcf_isCrcValid(T_PDU* rxPdu)
{
  bool result;
  uint8 expCrc[CRC_SIZE];

  crc16_calc(crcCtx, rxPdu->data, rxPdu->len - CRC_SIZE);
  crc16_write_le(crcCtx, expCrc);
  if(0 != memcmp(rxPdu->data[rxPdu->len - CRC_SIZE], expCrc, CRC_SIZE))
  {
    result = FALSE;
  }
  else
  {
    result = !FALSE;
  }
  return result;
}
#endif


#if !7
void dlcf_sendFrame(T_DLCF_CTX* ctx, T_PDU* txPdu)
{
  uint8 txByte;
  ctx->txPdu = txPdu;

  while(DLCF_FRAME_PENDING == dlcf_sendByte(ctx, &txByte))
  {
    //uart_sendByte(devID, txByte);
  }
}
#endif


/*
 ******************************************************************************
 * Function: dlcf_run
 ******************************************************************************
 * @brief DLCF's cyclic function
 *
 * @param [out] ctx - DLCF context
 *
 ******************************************************************************
 */
 
void dlcf_run(T_DLCF_CTX* ctx)
{
  T_DLCF_STATUS rxStat = DLCF_STATUS_eFRAME_PENDING;
  uint8 byte;

  /* Execute transmission path:
   * Try to process next byte of the tansmit PDU.
   */
  if(DLCF_OK != dlcf_procTxByte(ctx, &byte))
  {
    /* Nothing to transmit */
  }
  else
  {
    /* We got a byte to be transmitted */
    TRACE_DLCF_INFO("DLCF TX: %02x ('%1c')\n", byte, byte);
    /* Send it to the next lower layer */
    dlcf_sendByte(ctx, byte);
    // TODO: What to do if it fails?
  }

  if(DLCF_RX_STATE_eIDLE == ctx->rxState)
  {
    /* No RX PDU set.
     * If we do nothing here, we cannot deliver non-frame bytes either.
     * If we try to receive something, we will latest get stuck, if
     * SOF is received. If we really expect framing, we should never see
     * the SOF unintended.
     */
  }
  /* Execute reception path */
  else if(FALSE != dlcf_recvByte(ctx, &byte))
  {
    /* Byte received */
    TRACE_DLCF_INFO("DLCF RX: %02x ('%1c')\n", byte, byte);
    /* Process the received byte into the receive PDU */
    rxStat = dlcf_procRxByte(ctx, byte);
  }
  else
  {
    /* Nothing received */
  }

  /* Examine the receiver status */
  switch(rxStat)
  {
  case DLCF_STATUS_eFRAME_PENDING:
    /* Frame not yet finished */
    break;
    
  case DLCF_STATUS_eBYTE_RECEIVED:
    /* Single character received */
    break;

  case DLCF_STATUS_eFRAME_FINISHED:
    /* Complete frame received */
    break;

  default:
    break;
  }
}


#if !8
//void dlcf_recvFrame(T_DLCF_CTX* ctx, T_PDU* rxPdu)
void dlcf_runRx(void)
{
  T_DLCF_CTX* ctx;
  T_PDU* rxPdu;
  uint8 rxByte;
  uint8 stat = DLCF_FRAME_PENDING;
  ctx->rxPdu = rxPdu;

  if(UART_OK == uart_recvByte(devID, &rxByte))
  {
    stat = dlcf_procRxByte(ctx, rxByte);
  }

  if(DLCF_FRAME_PENDING == stat)
  {
    /* Frame not yet finished */
  }
  else if(DLCF_RECV_CHAR == stat)
  {
    /* Single character received */
  }
  else
  {
    /* Complete frame received */
  }
}
#endif

/*
 * dlcf_requestTransmit()
 *   dlcf__initTransmit()
 *   
 * dlcf_startTransmit()
 *   dlcf__execTransmit()
 *
 * dlcf_continueTransmit()
 *   dlcf__initTransmit()
 *   dlcf__execTransmit()
 *
 * dlcf_finishTransmit()
 *   ...
 *
 * dlcf_transmitData()
 *   dlcf_initTransmit()
 *   dlcf_execTransmit()
 *
 * dlcf_continueTransmit()
 *   dlcf__initTransmit()
 *   dlcf__execTrandmit()
 *
 * dlcf_finishTransmit()
 *   ...
 */
#if 0
void msgrt_cyclic(void)
{
  uint8 res;
  uint8 c;

  if(UART_OK != uart_recvByte(devID, &byte))
  {
  }
  else
  {
    status = dlcf_recvByte(dlcfCtx, byte);
    if(DLCF_PENDING == status)
    {
      /* Frame reception pending */
    }
    else if(DLCF_RECV_CHAR == status)
    {
      /* Single character received */
    }
    else if(DLCF_RECV_FRAME == status)
    {
      /* Frame received */
      if(dlcf
    }
    /* Check UART receive queue for completed message */
    res = uarttp_receiveBlock(UARTTP_CH_UART_DBG, &c);
    if (res == UARTTP_RCV_TYPE_BLOCK)
    {
      /* Received data was a TP message, so check for valid checksum */
      if (uarttp_getCrc16(UARTTP_CH_UART_DBG) == 0u)
      {
         /* FCS was valid so route the message to the upd_swc */
         upd_recvMsg(rxBuffer[MSGRT_CH_UART_DBG].bytes, uarttp_getIndex(UARTTP_CH_UART_DBG));
      }

      /* Message has been processed or was invalid, so re-initialize buffer control structure */
      uarttp_recvConfirm(UARTTP_CH_UART_DBG);
    }
    else if (res == UARTTP_RCV_TYPE_CHAR)
    {
      /* Received data was a single character */
      debug_recvByteCbk(c);
    }
    else
    {
      /* Nothing was received */
    }
  }
}
#endif

