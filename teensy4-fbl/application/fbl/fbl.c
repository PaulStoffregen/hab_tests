#ifndef FBL_C
#define FBL_C
#endif /* FBL_C */

#include "bsp.h"

#include "trace_pub.h"
#include "rom_api.h"
#include "ext_flash.h"
#include "libc.h"
#include "pdu.h"
#include "bcp.h"
#include "fbl_defs.h"
#include "sw_release.h"
#include "swinfo.h"
#include "target_cfg.h"
#include "fbl.h"

#include <string.h> /* for offsetof() */


#if (TRC_FEAT_FBL_ENA == STD_ON)
#define TRACE_FBL_API(...)   TRACE_FEATURE(TRC_FEAT_ID_eFBL, TRACE_FEATURE_CLASS3, __VA_ARGS__)
#define TRACE_FBL_INFO(...)  TRACE_FEATURE(TRC_FEAT_ID_eFBL, TRACE_FEATURE_CLASS2, __VA_ARGS__)
#define TRACE_FBL_ERROR(...) TRACE_FEATURE(TRC_FEAT_ID_eFBL, TRACE_FEATURE_CLASS0, __VA_ARGS__)
#define TRACE_FBL_STATE(...) TRACE_FEATURE(TRC_FEAT_ID_eFBL, TRACE_FEATURE_CLASS1, __VA_ARGS__)
#else /* (TRC_FEAT_FBL_ENA != STD_ON) */
#define TRACE_FBL_API(...)   /* empty */
#define TRACE_FBL_INFO(...)  /* empty */
#define TRACE_FBL_ERROR(...) /* empty */
#define TRACE_FBL_STATE(...) /* empty */
#endif /* (TRC_FEAT_FBL_ENA) */


typedef enum FBL_STATE
{
  FBL_STATE_eRESET = 0,
  FBL_STATE_eINIT,
  FBL_STATE_eIDLE,
  FBL_STATE_eEXIT,
  FBL_STATE_eREBOOT,
}T_FBL_STATE;


typedef enum FBL_COM_STATE
{
  FBL_COM_STATE_eRESET = 0,
  FBL_COM_STATE_eRECV_REQ,
  FBL_COM_STATE_eSEND_RSP,
}T_FBL_COM_STATE;


typedef struct
{
  uint32 imgAddr;
  uint32 imgSize;
  uint32 entryVect;
  uint32 entryVectInv;
  uint16 timeCnt;
  T_FBL_COM_STATE comState;
  T_FBL_STATE state;
}T_FBL_DATA;

static T_FBL_DATA fbl_dataTbl[1];


void fbl_sendSwInfoRsp(void)
{
  T_FBL_MSG_SWINFO_RSP* rspMsg = NULL;
  const T_SWINFO* fblSwInfo = swinfo_getOwnSwInfo();
  const T_SW_REL_INFO* fblRelInfo = NULL;
  T_SWINFO appSwInfo;
  T_SW_REL_INFO appRelInfo;
  T_PDU  rspPdu;
  uint16 len;

  /* Read application software info */
  libc_memcpy(&appSwInfo, (void*)FBL_APP_SWINFO_ADDR, sizeof(T_SWINFO));

  if(appSwInfo.versionLen != sizeof(T_SW_REL_INFO))
  {
    /* Invalid release info */
  }
  else if(appSwInfo.versionAddr < appSwInfo.imgAddr)
  {
    /* Invalid release info */
  }
  else
  {
    libc_memcpy(&appRelInfo, (void*)appSwInfo.versionAddr, appSwInfo.versionLen);
  }
  
  if(0 == fblSwInfo->versionLen)
  {
    /* No release info for FBL */
  }
  else
  {
    fblRelInfo = (T_SW_REL_INFO*)(void*)fblSwInfo->versionAddr;
  }
  
  if(STATUS_eOK != bcp_getTxPdu(&rspPdu))
  {
    TRACE_FBL_ERROR("FBL Error\n");
  }
  else
  {
    rspMsg = (T_FBL_MSG_SWINFO_RSP*)rspPdu.data;

    /* Set message length */
    rspMsg->msgType = FBL_MSG_ID_eSWINFO_RSP;

    /* Copy swInfo tag */
    len = sizeof(fblSwInfo->tag);
    libc_memcpy((void*)&rspMsg->tag, (void*)&fblSwInfo->tag, len);

    /* Copy addr, size and CRC */
    len = offsetof(T_SWINFO, tag);
    libc_memcpy(rspMsg->fblInfo, fblSwInfo, len);
    libc_memcpy(rspMsg->appInfo, &appSwInfo, len);

    /* Check whether swInfo's release info fits into the buffer */
    len = fblSwInfo->versionLen;
    if(len < sizeof(rspMsg->fblVerInfo))
    {
      /* Copy release info */
      libc_memcpy(rspMsg->fblVerInfo, fblRelInfo, len);
    }
    /* Copy release info length */
    len = sizeof(fblSwInfo->versionLen);
    libc_memcpy(rspMsg->fblVerLen, &fblSwInfo->versionLen, len);


    /* Check whether swInfo's release info fits into the buffer */
    len = appSwInfo.versionLen;
    if(len < sizeof(rspMsg->appVerInfo))
    {
      /* Copy release info */
      libc_memcpy(rspMsg->appVerInfo, &appRelInfo, len);
    }
    /* Copy release info length */
    len = sizeof(appSwInfo.versionLen);
    libc_memcpy(rspMsg->appVerLen, &appSwInfo.versionLen, len);


    rspPdu.len = sizeof(T_FBL_MSG_SWINFO_RSP);
  }

  if(NULL == rspMsg)
  {
    /* */
  }
  else if(STATUS_eOK != bcp_sendMsg(&rspPdu))
  {
    TRACE_FBL_ERROR("FBL Error\n");
  }
  else
  {
    /* Successfully sent response */
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

uint32 fbl_procBootstrapMsg(T_FBL_DATA* fblData, T_PDU* msgPdu)
{
  T_FBL_MSG_ENTRY_REQ* reqMsg = (T_FBL_MSG_ENTRY_REQ*)msgPdu->data;
  uint8  expToken[] = FBL_BOOTSTRAP_TOKEN;
  uint32 errCode = BCP_ERR_ID_eNONE;

  (void)fblData;

  /* Check for correct size of expected message */
  if(msgPdu->len != sizeof(T_FBL_MSG_ENTRY_REQ))
  {
    /* Unexpected size */
    TRACE_FBL_INFO("FBL: Unexpected size %d\n", msgPdu->len);
    errCode = BCP_ERR_ID_eINVALID_SIZE;
  }
  /* Check for correct message contents */
  else if(0 != libc_memcmp(&reqMsg->bootMagic, expToken, sizeof(reqMsg->bootMagic)))
  {
    TRACE_FBL_INFO("FBL: Invalid token\n");
    errCode = BCP_ERR_ID_eINVALID_DATA;
  }
  else
  {
    /* Valid bootstrap command received */
    TRACE_FBL_INFO("FBL: Valid bootstrap\n");
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

uint32 fbl_procSwInfoMsg(T_FBL_DATA* fblData, T_PDU* reqPdu)
{
  uint32 errCode = BCP_ERR_ID_eNONE;

  (void)fblData;

  /* Check for correct size of expected message */
  if(reqPdu->len != sizeof(T_FBL_MSG_SWINFO_REQ))
  {
    /* Unexpected size */
    TRACE_FBL_INFO("FBL: Unexpected size %d\n", reqPdu->len);
    errCode = BCP_ERR_ID_eINVALID_SIZE;
  }
  else
  {
    /* Valid SW info command received */
    TRACE_FBL_INFO("FBL: Valid swInfo request\n");
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

uint32 fbl_procInvalidateMsg(T_FBL_DATA* fblData, T_PDU* reqPdu)
{
  T_FBL_MSG_INVALIDATE_REQ* reqMsg = (T_FBL_MSG_INVALIDATE_REQ*)reqPdu->data;
  uint32 errCode = BCP_ERR_ID_eNONE;

  // TODO: Handle update sequence state checking
  
  /* Check for correct size of expected message */
  if(reqPdu->len != sizeof(T_FBL_MSG_INVALIDATE_REQ))
  {
    /* Unexpected size */
    TRACE_FBL_INFO("FBL: Unexpected size %d\n", reqPdu->len);
    errCode = BCP_ERR_ID_eINVALID_SIZE;
  }
  else if(reqMsg->blkAddr < FBL_APP_START_ADDR)
  {
    /* Invalid application start address */
    errCode = BCP_ERR_ID_eINVALID_DATA;
  }
  else if(0 != (reqMsg->blkAddr & (FBL_ALIGN_SIZE - 1)))
  {
    /* Improper alignment of start address */
    errCode = BCP_ERR_ID_eINVALID_DATA;
  }
  else if(reqMsg->blkSize < FBL_ALIGN_SIZE)
  {
    /* Invalid size */
    errCode = BCP_ERR_ID_eINVALID_DATA;
  }
  else if(0 != (reqMsg->blkSize & (FBL_ALIGN_SIZE - 1)))
  {
    /* Improper alignment of size */
    errCode = BCP_ERR_ID_eINVALID_DATA;
  }
  else if((reqMsg->blkAddr + reqMsg->blkSize) >= FBL_APP_END_ADDR)
  {
    /* Invalid end address */
    errCode = BCP_ERR_ID_eINVALID_DATA;
  }
  else
  {
    uint32 logAddr = FBL_APP_ENTRY_ADDR - FBL_FLASH_BASE_ADDR;
    uint8  invalidSignValue[sizeof(fblData->entryVect)] = { 0 };

    TRACE_FBL_INFO("FBL: Valid invalidate request\n");
    (void)extflash_write(logAddr, invalidSignValue, FLASH_WRITE_PAGE_SIZE);
    fblData->imgAddr = reqMsg->blkAddr;
    fblData->imgSize = reqMsg->blkSize;
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

uint32 fbl_procEraseMsg(T_FBL_DATA* fblData, T_PDU* reqPdu)
{
  T_FBL_MSG_ERASE_REQ* reqMsg = (T_FBL_MSG_ERASE_REQ*)reqPdu->data;
  uint32 errCode = BCP_ERR_ID_eNONE;

  // TODO: Handle update sequence state checking
  
  /* Check for correct size of expected message */
  if(reqPdu->len != sizeof(T_FBL_MSG_ERASE_REQ))
  {
    /* Unexpected size */
    TRACE_FBL_INFO("FBL: Unexpected size %d\n", reqPdu->len);
    errCode = BCP_ERR_ID_eINVALID_SIZE;
  }
  else if(reqMsg->blkAddr != fblData->imgAddr)
  {
    /* Invalid start address */
    errCode = BCP_ERR_ID_eINVALID_DATA;
  }
  else if(reqMsg->blkSize != fblData->imgSize)
  {
    /* Invalid size */
    errCode = BCP_ERR_ID_eINVALID_DATA;
  }
  else
  {
    uint32 logAddr = FBL_APP_ENTRY_ADDR - 0x60000000;
    uint32 regionSize = 256 * 1024;

    TRACE_FBL_INFO("FBL: Valid erase request\n");
    (void)extflash_erase(logAddr, regionSize);

    // TODO: Blank check
    //extflash_blankCheck(logAddr, regionSize);
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

uint32 fbl_procProgramMsg(T_FBL_DATA* fblData, T_PDU* reqPdu)
{
  T_FBL_MSG_PROGRAM_REQ* reqMsg = (T_FBL_MSG_PROGRAM_REQ*)reqPdu->data;
  uint32 errCode = BCP_ERR_ID_eNONE;

  // TODO: Handle update sequence state checking
  
  /* Check for correct size of expected message */
  if(reqPdu->len != sizeof(T_FBL_MSG_PROGRAM_REQ))
  {
    /* Unexpected size */
    TRACE_FBL_INFO("FBL: Unexpected size %d\n", reqPdu->len);
    errCode = BCP_ERR_ID_eINVALID_SIZE;
  }
  else if(reqMsg->blkAddr < fblData->imgAddr)
  {
    /* Invalid block address */
    errCode = BCP_ERR_ID_eINVALID_DATA;
  }
  else if(0 != (reqMsg->blkAddr & (FBL_ALIGN_SIZE - 1)))
  {
    /* Improper alignment of block address */
    errCode = BCP_ERR_ID_eINVALID_DATA;
  }
  else if((reqMsg->blkAddr + FBL_ALIGN_SIZE) > (fblData->imgAddr + fblData->imgSize))
  {
    /* Invalid block address */
    errCode = BCP_ERR_ID_eINVALID_DATA;
  }
  else
  {
    uint32 logAddr = reqMsg->blkAddr - FBL_FLASH_BASE_ADDR;

    TRACE_FBL_INFO("FBL: Valid program request\n");
    if(reqMsg->blkAddr > FBL_APP_ENTRY_ADDR)
    {
      /* Block start is beyond application entry */
    }
    else if((reqMsg->blkAddr + FBL_BLK_SIZE) < FBL_APP_ENTRY_ADDR)
    {
      /* Block end is less then application entry */
    }
    else
    {
      /* The current block contains the entry vector, which needs to be
       * copied to a safe location and replaced by the flash blank value.
       * 60010404 - 60010000 = 00000404;
       * 0x4
       */
      uint32 entryOffs = FBL_APP_ENTRY_ADDR & (FBL_BLK_SIZE - 1);
      uint32 blankValue = FLASH_BLANK_VALUE;

      libc_memcpy(&fblData->entryVect, &reqMsg->blkData[entryOffs], sizeof(fblData->entryVect));
      fblData->entryVectInv = (uint32)~(fblData->entryVect);

      libc_memcpy(&reqMsg->blkData[entryOffs], &blankValue, sizeof(fblData->entryVect));
    }
    (void)extflash_write(logAddr, reqMsg->blkData, FBL_ALIGN_SIZE);
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

uint32 fbl_procActivateMsg(T_FBL_DATA* fblData, T_PDU* reqPdu)
{
  T_FBL_MSG_ACTIVATE_REQ* reqMsg = (T_FBL_MSG_ACTIVATE_REQ*)reqPdu->data;
  uint32 errCode = BCP_ERR_ID_eNONE;

  // TODO: Handle update sequence state checking
  
  /* Check for correct size of expected message */
  if(reqPdu->len != sizeof(T_FBL_MSG_ACTIVATE_REQ))
  {
    /* Unexpected size */
    TRACE_FBL_INFO("FBL: Unexpected size %d\n", reqPdu->len);
    errCode = BCP_ERR_ID_eINVALID_SIZE;
  }
  else if(reqMsg->blkAddr != fblData->imgAddr)
  {
    /* Invalid start address */
    errCode = BCP_ERR_ID_eINVALID_DATA;
  }
  else if(reqMsg->blkSize != fblData->imgSize)
  {
    /* Invalid size */
    errCode = BCP_ERR_ID_eINVALID_DATA;
  }
  // TODO: Check for correct image CRC
  else if(fblData->entryVect != ~(fblData->entryVectInv))
  {
    errCode = BCP_ERR_ID_eINCONSISTENT;
  }
  else
  {
    uint32 logAddr = FBL_APP_ENTRY_ADDR - 0x60000000;

    TRACE_FBL_INFO("FBL: Valid activate request\n");
    (void)extflash_write(logAddr, (uint8*)(void*)&fblData->entryVect, sizeof(fblData->entryVect));
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

uint32 fbl_procRestartMsg(T_FBL_DATA* fblData, T_PDU* reqPdu)
{
  uint32 errCode = BCP_ERR_ID_eNONE;

  // TODO: Handle update sequence state checking
  
  /* Check for correct size of expected message */
  if(reqPdu->len != sizeof(T_FBL_MSG_RESET_REQ))
  {
    /* Unexpected size */
    TRACE_FBL_INFO("FBL: Unexpected size %d\n", reqPdu->len);
    errCode = BCP_ERR_ID_eINVALID_SIZE;
  }
  else
  {
    TRACE_FBL_INFO("FBL: Valid restart request\n");
    fblData->state = FBL_STATE_eEXIT;
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

static void fbl_opRecvReq(T_FBL_DATA* fblData)
{
  T_BCP_STATUS bcpStatus;
  T_BCP_MSG* reqMsg = NULL;
  T_PDU rxPdu =
  {
    .data = NULL,
    .size = 0,
    .len  = 0,
  };
  uint32 msgType = FBL_MSG_ID_eINVALID;
  uint32 errCode = BCP_ERR_ID_eNONE;

  /* Get protocol layer status */
  bcpStatus = bcp_getStatus();
   
  /* Check whether BCP is idle */
  if(BCP_STATUS_eIDLE != bcpStatus)
  {
    /* BCP is idle */
  }
  /* Check for job result */
  else if(BCP_JOB_RESULT_eOK != bcp_getJobResult())
  {
    TRACE_FBL_INFO("FBL: Reception failed\n");
    errCode = BCP_ERR_ID_eINVALID_CRC;
  }
  else
  {
    /* Boot Control Protocol received a complete frame,
     * so get it.
     */
    (void)bcp_getRxMsg(&rxPdu);

    reqMsg = (T_BCP_MSG*)rxPdu.data;
    msgType = reqMsg->msgType;
    switch(msgType)
    {
    case FBL_MSG_ID_eBOOTSTRAP_REQ:
      errCode = fbl_procBootstrapMsg(fblData, &rxPdu);
      if(BCP_ERR_ID_eNONE == errCode)
      {
        bcp_sendAckRsp(msgType);
      }
      break;

    case FBL_MSG_ID_eSWINFO_REQ:
      errCode = fbl_procSwInfoMsg(fblData, &rxPdu);
      if(BCP_ERR_ID_eNONE == errCode)
      {
        fbl_sendSwInfoRsp();
      }
      break;

    case FBL_MSG_ID_eINVALIDATE_REQ:
      errCode = fbl_procInvalidateMsg(fblData, &rxPdu);
      if(BCP_ERR_ID_eNONE == errCode)
      {
        bcp_sendAckRsp(msgType);
      }
      break;

    case FBL_MSG_ID_eERASE_REQ:
      errCode = fbl_procEraseMsg(fblData, &rxPdu);
      if(BCP_ERR_ID_eNONE == errCode)
      {
        bcp_sendAckRsp(msgType);
      }
      break;

    case FBL_MSG_ID_ePROGRAM_REQ:
      errCode = fbl_procProgramMsg(fblData, &rxPdu);
      if(BCP_ERR_ID_eNONE == errCode)
      {
        bcp_sendAckRsp(msgType);
      }
      break;

    case FBL_MSG_ID_eACTIVATE_REQ:
      errCode = fbl_procActivateMsg(fblData, &rxPdu);
      if(BCP_ERR_ID_eNONE == errCode)
      {
        bcp_sendAckRsp(msgType);
      }
      break;

    case FBL_MSG_ID_eRESET_REQ:
      errCode = fbl_procRestartMsg(fblData, &rxPdu);
      if(BCP_ERR_ID_eNONE == errCode)
      {
        bcp_sendAckRsp(msgType);
      }
      break;

    default:
      TRACE_FBL_INFO("FBL: Unexpected msgType\n");
      errCode = BCP_ERR_ID_eINVALID_TYPE;
      break;
    }
  }

  /* Check for error code */
  if(BCP_ERR_ID_eNONE != errCode)
  {
    /* Send negative response */
    bcp_sendNakRsp(msgType, errCode);
    fblData->comState = FBL_COM_STATE_eSEND_RSP;
    TRACE_FBL_STATE("FBL COM: RECV_REQ -> SEND_RSP\n");
  }
  else if(NULL != reqMsg)
  {
    /* No error */
    fblData->comState = FBL_COM_STATE_eSEND_RSP;
    TRACE_FBL_STATE("FBL COM: RECV_REQ -> SEND_RSP\n");
  }
  else
  {
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

static void fbl_opSendRsp(T_FBL_DATA* fblData)
{
  T_BCP_STATUS bcpStatus;

  /* Get protocol layer status */
  bcpStatus = bcp_getStatus();

  /* Check whether BCP is idle */
  if(BCP_STATUS_eIDLE != bcpStatus)
  {
    /* BCP is idle */
  }
  /* Check for job result */
  else if(BCP_JOB_RESULT_eOK != bcp_getJobResult())
  {
    TRACE_FBL_INFO("FBL: Transmission failed\n");
  }
  /* Boot Control Protocol transmitted complete frame. */
  else if(fblData->state != FBL_STATE_eEXIT)
  {
    /* The recent request was not a restart request */
    fblData->comState = FBL_COM_STATE_eRECV_REQ;
    TRACE_FBL_STATE("FBL COM: SEND_RSP -> RECV_REQ\n");

    bcp_listen();
  }
  else
  {
    /* The recent request was a restart request */
    fblData->comState = FBL_COM_STATE_eRESET;
    TRACE_FBL_STATE("FBL COM: SEND_RSP -> RESET\n");
    fblData->timeCnt = 10;
    fblData->state = FBL_STATE_eREBOOT;
    TRACE_FBL_STATE("FBL State: ANY -> REBOOT\n");
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

static void fbl_opReboot(T_FBL_DATA* fblData)
{
  T_ROM_API* romApi;
  uint32 arg;

  if(fblData->timeCnt > 0)
  {
    fblData->timeCnt--;
  }
  else if(BCP_STATUS_eIDLE != bcp_getStatus())
  {
    /* BCP still busy */
  }
  else
  {
    arg = BOOT_API_REBOOT;
    romApi = romApi_getAddr();
    romApi->bootApi(&arg);
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

void fbl_init(void)
{
  T_FBL_DATA* fblData = fbl_dataTbl;

  /* Initialize FBL data */
  libc_memset(fblData, 0, sizeof(T_FBL_DATA));

  /* Initialize state and flags */
  fblData->state = FBL_STATE_eINIT;
  TRACE_FBL_STATE("FBL: RESET -> INIT\n");
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void fbl_run(void)
{
  T_FBL_DATA* fblData = fbl_dataTbl;

  switch(fblData->comState)
  {
  case FBL_COM_STATE_eRECV_REQ:
    fbl_opRecvReq(fblData);
    break;

  case FBL_COM_STATE_eSEND_RSP:
    fbl_opSendRsp(fblData);
    break;
    
  default:
    break;
  }

  switch(fblData->state)
  {
  case FBL_STATE_eREBOOT:
    fbl_opReboot(fblData);
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

void fbl_enter(void)
{
  T_FBL_DATA* fblData = fbl_dataTbl;

  fblData->state = FBL_STATE_eIDLE;
  TRACE_FBL_STATE("FBL: INIT -> IDLE\n");

  fblData->comState = FBL_COM_STATE_eRECV_REQ;
  TRACE_FBL_STATE("FBL COM: RESET -> RECV_REQ\n");

  bcp_listen();
}

