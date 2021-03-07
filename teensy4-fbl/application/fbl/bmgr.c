#ifndef BMGR_C
#define BMGR_C
#endif /* BMGR_C */

#include "bsp.h"
#include "cpu_irq.h"
#include "hab_api.h"
#include "hab_info.h"
#include "trace_pub.h"
#include "uart.h"
#include "pdu.h"
#include "bcp.h"
#include "fbl_defs.h"
#include "libc.h"
#include "noinit.h"
#include "swinfo.h"
#include "target_cfg.h"
#include "fbl.h"
#include "bmgr.h"


#if (TRC_FEAT_BMGR_ENA == STD_ON)
#define TRACE_BMGR_API(...)   TRACE_FEATURE(TRC_FEAT_ID_eBMGR, TRACE_FEATURE_CLASS3, __VA_ARGS__)
#define TRACE_BMGR_INFO(...)  TRACE_FEATURE(TRC_FEAT_ID_eBMGR, TRACE_FEATURE_CLASS2, __VA_ARGS__)
#define TRACE_BMGR_ERROR(...) TRACE_FEATURE(TRC_FEAT_ID_eBMGR, TRACE_FEATURE_CLASS0, __VA_ARGS__)
#define TRACE_BMGR_STATE(...) TRACE_FEATURE(TRC_FEAT_ID_eBMGR, TRACE_FEATURE_CLASS1, __VA_ARGS__)
#else /* (TRC_FEAT_BMGR_ENA != STD_ON) */
#define TRACE_BMGR_API(...)   /* empty */
#define TRACE_BMGR_INFO(...)  /* empty */
#define TRACE_BMGR_ERROR(...) /* empty */
#define TRACE_BMGR_STATE(...) /* empty */
#endif /* (TRC_FEAT_BMGR_ENA) */


#define FBL_APP_ENTRY_REQ_LO 0xDEADBEAF
#define FBL_APP_ENTRY_REQ_HI 0xAFFEDEAD


#define BMGR_AUTH_TIMEOUT    10
#define BMGR_CMD_TIMEOUT     10000 /* ms */


typedef enum BMGR_STATE
{
  BMGR_STATE_eRESET = 0,
  BMGR_STATE_eINIT,
  BMGR_STATE_eWAIT_CMD,
  BMGR_STATE_eSEND_ACK,
  BMGR_STATE_eSEND_NAK,
  BMGR_STATE_eENTER_FBL,
  BMGR_STATE_eENTER_APP,
  BMGR_STATE_eIDLE,
}T_BMGR_STATE;


typedef struct
{
   T_BMGR_STATE state;
   const void* appIvtAddr;
   void (*appEntry)(void);
   uint32 timeout;
}T_BMGR_DATA;

static T_BMGR_DATA bmgr_data;


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static void bmgr_readAppEntry(T_BMGR_DATA* bmgrData)
{
  /* Read application image entry vector */
  libc_memcpy(&bmgrData->appEntry, (void*)FBL_APP_ENTRY_ADDR, sizeof(bmgrData->appEntry));
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static void bmgr_readAppIvtAddr(T_BMGR_DATA* bmgrData)
{
  bmgrData->appIvtAddr = (const void*)FBL_APP_IVT_ADDR;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static boolean bmgr_isAppImgInvalid(T_BMGR_DATA* bmgrData)
{
  boolean result = !FALSE; /* Assume invalid application image */
  T_SWINFO appSwInfo;

  /* Read application image software info */
  libc_memcpy(&appSwInfo, (void*)FBL_APP_SWINFO_ADDR, sizeof(T_SWINFO));

  if(appSwInfo.tag != SWINFO_TAG)
  {
    /* Invalid application image tag, so
     * no valid application pressent.
     */
    TRACE_BMGR_INFO("BMGR: SWInfo invalid tag\n");
  }
  else if(appSwInfo.imgAddr < FBL_APP_START_ADDR)
  {
    /* Invalid application image start address,
     * so no valid application pressent.
     */
    TRACE_BMGR_INFO("BMGR: SWInfo invalid addr\n");
  }
  else if(appSwInfo.imgAddr + appSwInfo.imgSize > FBL_APP_END_ADDR + 1)
  {
    /* Invalid application image end address,
     * so no valid application pressent.
     */
    TRACE_BMGR_INFO("BMGR: SWInfo invalid len\n");
  }
#if 0
  else if(bmgr_isAppCrcInalid(&appSwInfo, appSwInfo.startAddr, appSwInfo.endAddr))
  {
    /* Invalid application image CRC, so
     * no valid application pressent.
     */
    TRACE_BMGR_INFO("BMGR: SWInfo invalid CRC\n");
  }
#endif
  else if( !hab_isIvtValid(bmgrData->appIvtAddr))
  {
    /* Application IVT address doesn't contain a valid IVT header */
    TRACE_BMGR_INFO("BMGR: IVT invalid header\n");
  }
  else if(0 == (((uint32)(void*)bmgrData->appEntry) & 0x1))
  {
    /* Application entry is not a valid thumb instruction address */
    TRACE_BMGR_INFO("BMGR: SWInfo invalid entry\n");
  }
  else if(((uint32)(void*)bmgrData->appEntry) < appSwInfo.imgAddr)
  {
    /* Unexpected entry address, so
     * no valid application pressent.
     */
    TRACE_BMGR_INFO("BMGR: SWInfo invalid entry\n");
  }
  else if(((uint32)(void*)bmgrData->appEntry) >= (appSwInfo.imgAddr + appSwInfo.imgSize))
  {
    /* Unexpected entry address, so
     * no valid application pressent.
     */
    TRACE_BMGR_INFO("BMGR: SWInfo invalid entry\n");
  }
  else
  {
    /* So far application seams to be valid */
    result = FALSE;
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

static boolean bmgr_isFblReqActive(void)
{
   boolean result = FALSE; /* Assume no loader request */
   T_NOINIT_DATA* noInitData;
   
   /* Get noinit data */
   noInitData = noinit_getData();

   if(FBL_APP_ENTRY_REQ_LO != noInitData->entryReqLo)
   {
      /* Invalid loader request signature */
   }
   else if(FBL_APP_ENTRY_REQ_HI != noInitData->entryReqHi)
   {
      /* Invalid loader request signature */
   }
   else
   {
      /* Loader request signature valid */
      result = !FALSE;
   }

   /* Clear signature */
   noInitData->entryReqLo = 0;
   noInitData->entryReqHi = 0;
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

static boolean bmgr_checkFblEntryCondition(T_BMGR_DATA* bmgrData)
{
   boolean enterFbl = !FALSE; /* Assume FBL entry */

   /* Check whether valid / authentic application is pressent */
   if(FALSE != bmgr_isAppImgInvalid(bmgrData))
   {
      /* Application image is invalid,
       * so enter FBL
       */
   }
   else if(FALSE != bmgr_isFblReqActive())
   {
      /* Decent request active,
       * so enter FBL
       */
   }
   else
   {
      /* A valid application seams to be pressent,
       * so do not directly enter FBL.
       */
      enterFbl = FALSE;
   }
   return enterFbl;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void bmgr_exit(void)
{
  T_BMGR_DATA* bmgrData = &bmgr_data;

  bmgrData->state = BMGR_STATE_eRESET;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

uint32 bmgr_procBootstrapMsg(T_PDU* msgPdu)
{
  T_FBL_MSG_ENTRY_REQ* reqMsg = (T_FBL_MSG_ENTRY_REQ*)msgPdu->data;
  uint8  expToken[] = FBL_BOOTSTRAP_TOKEN;
  uint32 errCode = BCP_ERR_ID_eNONE;
  
  /* Check for correct size of expected message */
  if(msgPdu->len != sizeof(T_FBL_MSG_ENTRY_REQ))
  {
    /* Unexpected size */
    TRACE_BMGR_INFO("BMGR: Unexpected size %d\n", msgPdu->len);
    errCode = BCP_ERR_ID_eINVALID_SIZE;
  }
  /* Check for correct message contents */
  else if(0 != libc_memcmp(&reqMsg->bootMagic, expToken, sizeof(reqMsg->bootMagic)))
  {
    TRACE_BMGR_INFO("BMGR: Invalid token\n");
    errCode = BCP_ERR_ID_eINVALID_DATA;
  }
  else
  {
    /* Valid bootstrap command received */
    TRACE_BMGR_INFO("BMGR: Valid bootstrap\n");
  }
  return errCode;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void bmgr_opWaitCmd(T_BMGR_DATA* bmgrData)
{
  T_BCP_STATUS bcpStatus;
  T_BCP_MSG* reqMsg = NULL;
  T_PDU rxPdu =
  {
    .data = NULL,
    .size = 0,
    .len  = 0,
  };
  uint32 errCode = BCP_ERR_ID_eNONE;

  bcpStatus = bcp_getStatus();

//  TRACE_BMGR_API("bmgr_opWaitCmd()\n");
  /* Check timeout */
  if(bmgrData->timeout == 0)
  {
    /* Timeout exceeded, so deactivate BCP and prepare
     * to enter application.
     */
    bcp_cancel();
    bmgrData->state = BMGR_STATE_eENTER_APP;
    TRACE_BMGR_STATE("BMGR: WAIT_CMD -> ENTER_APP\n");
  }
  /* No timeout, so check whether BCP is idle */
  else if(BCP_STATUS_eIDLE != bcpStatus)
  {
    bmgrData->timeout--;
  }
  /* Check for job result */
  else if(BCP_JOB_RESULT_eOK != bcp_getJobResult())
  {
    TRACE_BMGR_INFO("BMGR: Invalid chksum\n");
    errCode = BCP_ERR_ID_eINVALID_CRC;
  }
  else
  {
    /* Boot Control Protocol received a complete frame,
     * so get it.
     */
    (void)bcp_getRxMsg(&rxPdu);

    reqMsg = (T_BCP_MSG*)rxPdu.data;
    switch(reqMsg->msgType)
    {
    case FBL_MSG_ID_eBOOTSTRAP_REQ:
      errCode = bmgr_procBootstrapMsg(&rxPdu);
      break;

    default:
      TRACE_BMGR_INFO("BMGR: Unexpected msgType\n");
      errCode = BCP_ERR_ID_eINVALID_TYPE;
      break;
    }
  }

  /* Check for error code */
  if(BCP_ERR_ID_eNONE != errCode)
  {
    /* Send negative response */
    bcp_sendNakRsp(FBL_MSG_ID_eBOOTSTRAP_REQ, errCode);
    bmgrData->state = BMGR_STATE_eSEND_NAK;
    TRACE_BMGR_STATE("BMGR: WAIT_CMD -> SEND_NAK\n");
  }
  /* No error, so check for message */
  else if(NULL != reqMsg)
  {
    /* Message was received and as we dont have an error code
     * send positive response
     */
    bcp_sendAckRsp(reqMsg->msgType);
    bmgrData->state = BMGR_STATE_eSEND_ACK;
    TRACE_BMGR_STATE("BMGR: WAIT_CMD -> SEND_ACK\n");
  }
  /* No message was received */
  else
  {
  }
}


void bmgr_opSendAck(T_BMGR_DATA* bmgrData)
{
  T_BCP_STATUS bcpStatus;
  bcpStatus = bcp_getStatus();

  TRACE_BMGR_API("bmgr_opSendAck()\n");
  if(bmgrData->timeout == 0)
  {
    /* Timeout exceeded */
    bmgrData->state = BMGR_STATE_eENTER_APP;
    TRACE_BMGR_STATE("BMGR: WAIT_CMD -> ENTER_APP\n");
  }
  /* Check if a frame was received */
  else if(BCP_STATUS_eIDLE != bcpStatus)
  {
    /* Check for command to be received */
    bmgrData->timeout--;
  }
  else if(BCP_JOB_RESULT_eOK != bcp_getJobResult())
  {
  }
  else
  {
    /* Boot Control Protocol transmitted complete frame. */
    bmgrData->state = BMGR_STATE_eENTER_FBL;
    TRACE_BMGR_STATE("BMGR: SEND_ACK -> ENTER_FBL\n");
  }
}


void bmgr_opSendNak(T_BMGR_DATA* bmgrData)
{
  T_BCP_STATUS bcpStatus;
  bcpStatus = bcp_getStatus();

  TRACE_BMGR_API("bmgr_opSendNak()\n");
  if(bmgrData->timeout == 0)
  {
    /* Timeout exceeded */
    bmgrData->state = BMGR_STATE_eENTER_APP;
    TRACE_BMGR_STATE("BMGR: WAIT_CMD -> ENTER_APP\n");
  }
  /* Check if a frame was received */
  else if(BCP_STATUS_eIDLE != bcpStatus)
  {
    /* Check for command to be received */
    bmgrData->timeout--;
  }
  else if(BCP_JOB_RESULT_eOK != bcp_getJobResult())
  {
  }
  else
  {
    /* Boot Control Protocol transmitted complete frame. */
    bcp_listen();
    bmgrData->state = BMGR_STATE_eWAIT_CMD;
    TRACE_BMGR_STATE("BMGR: SEND_NAK -> WAIT_CMD\n");
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

void bmgr_opEnterFbl(T_BMGR_DATA* bmgrData)
{
//  TRACE_BMGR_API("BMGR: bmgr_opEnterFbl\n");
  bmgr_exit();
  fbl_enter();
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void bmgr_opEnterApp(T_BMGR_DATA* bmgrData)
{
  int result;
  uint32 imageSize;

  TRACE_BMGR_API("BMGR: bmgr_opEnterApp()\n");
  TRACE_BMGR_API("Entry=%08x\n", bmgrData->appEntry);

  /* Get image size from IVT */
  imageSize = hab_getImageSizeFromIvt((void*)bmgrData->appIvtAddr);
  result = hab_authImage(FBL_APP_START_ADDR, imageSize, FBL_APP_IVT_OFFS);

  if(HAB_API_OK == result)
  {
    /* Give the UART/TRACE some time to get out all bytes */
    for(volatile int i = 0; i < 1000; i++);
    
    /* Wait till UART/TRACE is ready, disable UARTS and disable all interrupts */
    cpu_disableIRQs();

    /* Jump to entry */
    bmgrData->appEntry();
  }
  else
  {
    /* Image authentication failed */
  /* We didn't enter the application, so the authenetication failed */
  hab_dumpStatus();
  
  bmgrData->state = BMGR_STATE_eENTER_FBL;
  TRACE_BMGR_STATE("BMGR: WAIT_CMD -> ENTER_FBL\n");
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

void bmgr_init(void)
{
  T_BMGR_DATA* bmgrData = &bmgr_data;

   bmgrData->state = BMGR_STATE_eRESET;
   TRACE_BMGR_API("bmgr_init()\n");

   /* Read the Application entry vector and IVT address */
   bmgr_readAppEntry(bmgrData);
   bmgr_readAppIvtAddr(bmgrData);
   
   if(FALSE != bmgr_checkFblEntryCondition(bmgrData))
   {
      /* Conditions fulfilled to directly enter FBL. */
      bmgrData->state = BMGR_STATE_eENTER_FBL;
      TRACE_BMGR_STATE("BMGR: RESET -> ENTER_FBL\n");
   }
   else
   {
      /* Conditions to enter FBL not yet fulfilled,
       * wait for bootstrap command before entering
       * the application.
       */
      bmgrData->state = BMGR_STATE_eWAIT_CMD;
      bmgrData->timeout = BMGR_CMD_TIMEOUT;
      TRACE_BMGR_STATE("BMGR: RESET -> WAIT_CMD\n");

      /* Set BCP to listen state */
      bcp_listen();
   }
//   bmgrData->lastTime = bmgr_getTime();
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void bmgr_run(void)
{
   T_BMGR_DATA* bmgrData = &bmgr_data;
#if 0
   uint32 currTime = bmgr_getTime();
   uint32 diffTime = currTime - bmgrData->lastTime;
   bmgrData->lastTime = currTime;
#endif
//   TRACE_BMGR_API("BMGR: bmgr_run()\n");
   switch(bmgrData->state)
   {
   case BMGR_STATE_eWAIT_CMD:
      bmgr_opWaitCmd(bmgrData);
      break;
   
   case BMGR_STATE_eSEND_ACK:
      bmgr_opSendAck(bmgrData);
      break;

   case BMGR_STATE_eSEND_NAK:
      bmgr_opSendNak(bmgrData);
      break;

   case BMGR_STATE_eENTER_FBL:
      bmgr_opEnterFbl(bmgrData);
      break;

   case BMGR_STATE_eENTER_APP:
      bmgr_opEnterApp(bmgrData);
      break;

   default:
      break;
   }
}


#if 0
boolean bmgr_isAppCrcValid(T_SWINFO* appSwInfo, uint32 sAddr, uint32 eAddr)
{
   boolean result = FALSE;
   uint32 crc32;
   uint32 crc32Default;
   uint32 flashAddr;
   uint32 ramAddr;

   /* Set initial value */
   crc32 = CRC_INITIAL_VALUE32;

   /* From start to APP SW INFO */
   for(flashAddr = sAddr; flashAddr < FBL_APP_SWINFO_ADR; flashAddr++)
   {
      /* Trigger watchdog */
      MCU_TRIGGER_WATCHDOG;

      /* Calculate checksum from flash */
      crc32 = crc_calculateCRC32((uint8*)(void*)flashAddr, 1u, crc32);
   }

   /* First part of app SW info */
   for(ramAddr = (uint32)(void*)&appSwInfo->startAddress; ramAddr < (uint32)(void*)&appSwInfo->crc32; ramAddr++)
   {
      /* Trigger watchdog */
      MCU_TRIGGER_WATCHDOG;

      /* Calculate checksum from safe memory */
      crc32 = crc_calculateCRC32((uint8*)(void*)ramAddr, 1u, crc32);
   }

   /* Calculate checksum for CRC32 default value */
   crc32Default = SWINFO_CRC32_DEFAULT;
   crc32 = crc_calculateCRC32((uint8*)(void*)&crc32Default, sizeof(appSwInfo->crc32), crc32);

   /* Last part of app SW info */
   for(ramAddr = (uint32)(void*)&appSwInfo->applicationPresent; ramAddr < (uint32)(void*)&appSwInfo->reserve[10]; ramAddr++)
   {
      /* Trigger watchdog */
      MCU_TRIGGER_WATCHDOG;

      /* Calculate checksum from safe memory */
      crc32 = crc_calculateCRC32((uint8*)(void*)ramAddr, 1u, crc32);
   }

   /* Remaining app image */
   for(flashAddr = FBL_APP_SWINFO_ADR + sizeof(swinfo_dataApp_t); flashAddr <= eAddr; flashAddr++)
   {
      /* Trigger watchdog */
      MCU_TRIGGER_WATCHDOG;

      /* Calculate checksum from flash */
      crc32 = crc_calculateCRC32((uint8*)(void*)flashAddr, 1u, crc32);
   }

   /* Compare CRC */
   if(crc32 == appSwInfo->crc32)
   {
      result = !FALSE;
   }
   return result;
}
#endif

