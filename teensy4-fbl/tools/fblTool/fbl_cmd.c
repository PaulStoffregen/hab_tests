#ifndef FBL_CMD_C
#define FBL_CMD_C
#endif /* FBL_CMD_C */


#include "bsp.h"

#include "trace_pub.h"
#include "hab.h"
#include "libc.h"
#include "uart.h"
#include "pdu.h"
#include "bcp.h"
#include "dlcf.h"
#include "swinfo.h"
#include "sw_release.h"
#include "fbl_defs.h"

#include <stdio.h>
#include <string.h> /* for memset()/memcpy() */
#include <stddef.h> /* for offsetof() */


#if (TRC_FEAT_FBLCTL_ENA == STD_ON)
#define TRACE_FBLCTL_API(...)   TRACE_FEATURE(TRC_FEAT_ID_eFBLCTL, TRACE_FEATURE_CLASS3, __VA_ARGS__)
#define TRACE_FBLCTL_INFO(...)  TRACE_FEATURE(TRC_FEAT_ID_eFBLCTL, TRACE_FEATURE_CLASS2, __VA_ARGS__)
#define TRACE_FBLCTL_ERROR(...) TRACE_FEATURE(TRC_FEAT_ID_eFBLCTL, TRACE_FEATURE_CLASS0, __VA_ARGS__)
#define TRACE_FBLCTL_STATE(...) TRACE_FEATURE(TRC_FEAT_ID_eFBLCTL, TRACE_FEATURE_CLASS1, __VA_ARGS__)
#else /* (TRC_FEAT_FBLCTL_ENA != STD_ON) */
#define TRACE_FBLCTL_API(...)   /* empty */
#define TRACE_FBLCTL_INFO(...)  /* empty */
#define TRACE_FBLCTL_ERROR(...) /* empty */
#define TRACE_FBLCTL_STATE(...) /* empty */
#endif /* (TRC_FEAT_FBLCTL_ENA) */


/*
 * There shall be a cyclic service at which one can register a request and possibly also a notification callback.
 * The command lind interpreter shall request a command transmission and when finished, the results shall be
 * printed on screen.
 */
typedef uint32 T_FBLCTL_SIZE;
typedef uint32 T_FBLCTL_ADDR;

typedef enum FBLCTL_JOB_TYPE
{
  FBLCTL_JOB_TYPE_eNONE = 0,
  FBLCTL_JOB_TYPE_eBOOTSTRAP,
  FBLCTL_JOB_TYPE_eSWINFO,
  FBLCTL_JOB_TYPE_eINVALIDATE,
  FBLCTL_JOB_TYPE_eERASE,
  FBLCTL_JOB_TYPE_ePROGRAM,
  FBLCTL_JOB_TYPE_eACTIVATE,
  FBLCTL_JOB_TYPE_eRESET,
}T_FBLCTL_JOB_TYPE;


typedef enum FBLCTL_SUB_STATE
{
  FBLCTL_SUB_STATE_eIDLE = 0,
  FBLCTL_SUB_STATE_eINIT_REQ,
  FBLCTL_SUB_STATE_eSEND_REQ,
  FBLCTL_SUB_STATE_eRECV_RSP,
}T_FBLCTL_SUB_STATE;


typedef struct
{
  T_FBLCTL_ADDR      addr;
  T_FBLCTL_SIZE      size;
  T_FBLCTL_JOB_TYPE  type;
  T_FBLCTL_SUB_STATE subState;
  uint16             timeout;
  uint8              retries;
}T_FBLCTL_JOB_DATA;


typedef enum FBLCTL_STATE
{
  FBLCTL_STATE_eRESET = 0,
  FBLCTL_STATE_eIDLE,
  FBLCTL_STATE_eBUSY,
  FBLCTL_STATE_eINIT_REQ,
  FBLCTL_STATE_eSEND_REQ,
  FBLCTL_STATE_eRECV_RSP,
}T_FBLCTL_STATE;


typedef enum FBLCTL_JOB_STATE
{
  FBLCTL_JOB_STATE_eOK = 0,
  FBLCTL_JOB_STATE_ePENDING,
  FBLCTL_JOB_STATE_eERROR,
}T_FBLCTL_JOB_STATE;


typedef struct
{
  T_FBLCTL_STATE     state;
  T_FBLCTL_JOB_STATE jobState;
  T_FBLCTL_JOB_DATA  jobData;
  T_DLCF_CTX         dlcfCtx;
  void              (*cbk)(uint32);
  uint32             injectErr;
  uint8              txBuf[1100];
  uint8              rxBuf[100];
}T_FBLCTL_DATA;


static T_FBLCTL_DATA fblctl_data =
{
  .state = 0,
  .jobState = 0,
};


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static T_FBLCTL_DATA* fblctl_getCtlData(void)
{
  return &fblctl_data;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */
#if 0
static void fblctl_signalJobFail()
{
}
#endif


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */
static void fblctl_signalJobEnd(T_FBLCTL_DATA* ctlData, uint32 errCode)
{
  void (*cbk)(uint32);

  cbk = ctlData->cbk;
  if(NULL != cbk)
  {
    cbk(errCode);
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

void fblctl_dumpSwInfo(T_PDU* rspPdu)
{
  T_FBL_MSG_SWINFO_RSP* rspMsg = (T_FBL_MSG_SWINFO_RSP*)rspPdu->data;
  T_SWINFO fblSwInfo;
  T_SWINFO appSwInfo;
  T_SW_REL_INFO fblRelInfo;
  T_SW_REL_INFO appRelInfo;
//  uint32 imgAddr;
//  uint32 imgSize;
  uint16 len;

  len = offsetof(T_SWINFO, tag);
  libc_memcpy(&fblSwInfo, rspMsg->fblInfo, len);
  libc_memcpy(&appSwInfo, rspMsg->appInfo, len);
//  printf("Tag:      %08x\n", rspMsg->tag);

  printf("\n");
  printf("FBL SWInfo:\n");
  printf("ImgAddr:  %08x\n", fblSwInfo.imgAddr);
  printf("ImgSize:  %08x (%d)\n", fblSwInfo.imgSize, fblSwInfo.imgSize);
  printf("ImgCRC:   %08x\n", fblSwInfo.crc);

  len = sizeof(fblSwInfo.versionLen);
  libc_memcpy(&fblSwInfo.versionLen, rspMsg->fblVerLen, len);
  if(fblSwInfo.versionLen > 0)
  {
    uint16 layoutID;

    libc_memcpy(&fblRelInfo, rspMsg->fblVerInfo, fblSwInfo.versionLen);

    layoutID = BYTES_TO_UINT16(fblRelInfo.layoutID[0], fblRelInfo.layoutID[1]);
    if(0 != layoutID)
    {
      /* invalid layout ID */
    }
    else
    {
      /* */
      uint16 major = BYTES_TO_UINT16(fblRelInfo.major[0], fblRelInfo.major[1]);
      uint16 minor = BYTES_TO_UINT16(fblRelInfo.minor[0], fblRelInfo.minor[1]);
      uint16 patch = BYTES_TO_UINT16(fblRelInfo.patch[0], fblRelInfo.patch[1]);
      uint32 rev = BYTES_TO_UINT32(fblRelInfo.revision[0], fblRelInfo.revision[1], fblRelInfo.revision[2], fblRelInfo.revision[3]);
       
      printf("Version:  %d.%d.%d\n", major, minor, patch);
      printf("Revision: %d\n", rev);
      printf("Commit:   %s\n", fblRelInfo.shortHash);
      printf("Release:  %s %s\n", fblRelInfo.relDate, fblRelInfo.relTime);
      printf("Built:    %s %s\n", fblRelInfo.buildDate, fblRelInfo.buildTime);
    }
  }
  printf("\n");


  printf("APP SWInfo:\n");
  printf("ImgAddr:  %08x\n", appSwInfo.imgAddr);
  printf("ImgSize:  %08x (%d)\n", appSwInfo.imgSize, appSwInfo.imgSize);
  printf("ImgCRC:   %08x\n", appSwInfo.crc);

  /* Decode release info length */ 
  len = sizeof(appSwInfo.versionLen);
  libc_memcpy(&appSwInfo.versionLen, rspMsg->appVerLen, len);
  if(appSwInfo.versionLen == sizeof(T_SW_REL_INFO))
  {
    uint16 layoutID;

    libc_memcpy(&appRelInfo, rspMsg->appVerInfo, appSwInfo.versionLen);

    layoutID = BYTES_TO_UINT16(appRelInfo.layoutID[0], appRelInfo.layoutID[1]);
    if(0 != layoutID)
    {
      /* invalid layout ID */
    }
    else
    {
      /* */
      uint16 major = BYTES_TO_UINT16(appRelInfo.major[0], appRelInfo.major[1]);
      uint16 minor = BYTES_TO_UINT16(appRelInfo.minor[0], appRelInfo.minor[1]);
      uint16 patch = BYTES_TO_UINT16(appRelInfo.patch[0], appRelInfo.patch[1]);
      uint32 rev = BYTES_TO_UINT32(appRelInfo.revision[0], appRelInfo.revision[1], appRelInfo.revision[2], appRelInfo.revision[3]);
     
      printf("Version:  %d.%d.%d\n", major, minor, patch);
      printf("Revision: %d\n", rev);
      printf("Commit:   %s\n", appRelInfo.shortHash);
      printf("Release:  %s %s\n", appRelInfo.relDate, appRelInfo.relTime);
      printf("Built:    %s %s\n", appRelInfo.buildDate, appRelInfo.buildTime);
    }
  }
  else
  {
//    printf("APP SWInfo:\n");
  }
  printf("\n");
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static uint32 fblctl_procBootstrapRsp(T_FBLCTL_DATA* ctlData, T_PDU* rspPdu)
{
  T_BCP_MSG* rspMsg;
  uint32 errCode = BCP_ERR_ID_eNONE;

  TRACE_FBLCTL_API("FBLCTL: fblctl_procBootstrapRsp()\n");

  rspMsg = (T_BCP_MSG*)rspPdu->data;
  switch(rspMsg->msgType)
  {
  case FBL_MSG_ID_eACK_RSP:
    /* Positive response */
    TRACE_FBLCTL_INFO("FBLCTL: ACK received\n");
    TRACE_SEND(" OK\n\n");
    break;
         
  case FBL_MSG_ID_eNAK_RSP:
    /* Negative response */
    errCode = ((T_FBL_MSG_NAK_RSP*)(void*)rspMsg)->errCode;
    TRACE_FBLCTL_INFO("FBLCTL: NAK received (%02x)\n", errCode);
    TRACE_SEND(" FAILED [%s]\n\n", bcp_errStr[errCode]);
    break;
         
  default:
    /* Unexpected response */
    errCode = BCP_ERR_ID_eUNEXPECTED;
    TRACE_FBLCTL_INFO("FBLCTL: Unexpected MSG received\n");
    TRACE_SEND(" FAILED [%s]\n\n", bcp_errStr[BCP_ERR_ID_eUNEXPECTED]);
    break;
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

static uint32 fblctl_procSwInfoRsp(T_FBLCTL_DATA* ctlData, T_PDU* rspPdu)
{
  T_BCP_MSG* rspMsg;
  uint32 errCode = BCP_ERR_ID_eNONE;

  TRACE_FBLCTL_API("FBLCTL: fblctl_procSwInfoRsp()\n");

  rspMsg = (T_BCP_MSG*)rspPdu->data;
  switch(rspMsg->msgType)
  {
  case FBL_MSG_ID_eSWINFO_RSP:
    /* Positive response */
    TRACE_FBLCTL_INFO("FBLCTL: ACK received\n");
    TRACE_SEND(" OK\n\n");
    fblctl_dumpSwInfo(rspPdu);
    break;

  case FBL_MSG_ID_eNAK_RSP:
    /* Negative response */
    errCode = ((T_FBL_MSG_NAK_RSP*)(void*)rspMsg)->errCode;
    TRACE_FBLCTL_INFO("FBLCTL: NAK received (%02x)\n", errCode);
    TRACE_SEND(" FAILED [%s]\n\n", bcp_errStr[errCode]);
    break;

  default:
    /* Unexpected response */
    errCode = BCP_ERR_ID_eUNEXPECTED;
    TRACE_FBLCTL_INFO("FBLCTL: Unexpected MSG received\n");
    TRACE_SEND(" FAILED [%s]\n\n", bcp_errStr[BCP_ERR_ID_eUNEXPECTED]);
    break;
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

static uint32 fblctl_procInvalidateRsp(T_FBLCTL_DATA* ctlData, T_PDU* rspPdu)
{
  T_BCP_MSG* rspMsg;
  uint32 errCode = BCP_ERR_ID_eNONE;

  TRACE_FBLCTL_API("FBLCTL: fblctl_procInvalidateRsp()\n");

  rspMsg = (T_BCP_MSG*)rspPdu->data;
  switch(rspMsg->msgType)
  {
  case FBL_MSG_ID_eACK_RSP:
    /* Positive response */
    TRACE_FBLCTL_INFO("FBLCTL: ACK received\n");
    TRACE_SEND(" OK\n\n");
    break;

  case FBL_MSG_ID_eNAK_RSP:
    /* Negative response */
    errCode = ((T_FBL_MSG_NAK_RSP*)(void*)rspMsg)->errCode;
    TRACE_FBLCTL_INFO("FBLCTL: NAK received (%02x)\n", errCode);
    TRACE_SEND(" FAILED [%s]\n\n", bcp_errStr[errCode]);
    break;

  default:
    /* Unexpected response */
    errCode = BCP_ERR_ID_eUNEXPECTED;
    TRACE_FBLCTL_INFO("FBLCTL: Unexpected MSG received\n");
    TRACE_SEND(" FAILED [%s]\n\n", bcp_errStr[BCP_ERR_ID_eUNEXPECTED]);
    break;
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

static uint32 fblctl_procEraseRsp(T_FBLCTL_DATA* ctlData, T_PDU* rspPdu)
{
  T_BCP_MSG* rspMsg;
  uint32 errCode = BCP_ERR_ID_eNONE;

  TRACE_FBLCTL_API("FBLCTL: fblctl_procEraseRsp()\n");

  rspMsg = (T_BCP_MSG*)rspPdu->data;
  switch(rspMsg->msgType)
  {
  case FBL_MSG_ID_eACK_RSP:
    /* Positive response */
    TRACE_FBLCTL_INFO("FBLCTL: ACK received\n");
    TRACE_SEND(" OK\n\n");
    break;

  case FBL_MSG_ID_eNAK_RSP:
    /* Negative response */
    errCode = ((T_FBL_MSG_NAK_RSP*)(void*)rspMsg)->errCode;
    TRACE_FBLCTL_INFO("FBLCTL: NAK received (%02x)\n", errCode);
    TRACE_SEND(" FAILED [%s]\n\n", bcp_errStr[errCode]);
    break;

  default:
    /* Unexpected response */
    errCode = BCP_ERR_ID_eUNEXPECTED;
    TRACE_FBLCTL_INFO("FBLCTL: Unexpected MSG received\n");
    TRACE_SEND(" FAILED [%s]\n\n", bcp_errStr[BCP_ERR_ID_eUNEXPECTED]);
    break;
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

static uint32 fblctl_procProgramRsp(T_FBLCTL_DATA* ctlData, T_PDU* rspPdu)
{
  T_BCP_MSG* rspMsg;
  uint32 errCode = BCP_ERR_ID_eNONE;

  TRACE_FBLCTL_API("FBLCTL: fblctl_procProgramRsp()\n");

  rspMsg = (T_BCP_MSG*)rspPdu->data;
  switch(rspMsg->msgType)
  {
  case FBL_MSG_ID_eACK_RSP:
    /* Positive response */
    TRACE_FBLCTL_INFO("FBLCTL: ACK received\n");
    TRACE_SEND(" OK\n\n");
    break;

  case FBL_MSG_ID_eNAK_RSP:
    /* Negative response */
    errCode = ((T_FBL_MSG_NAK_RSP*)(void*)rspMsg)->errCode;
    TRACE_FBLCTL_INFO("FBLCTL: NAK received (%02x)\n", errCode);
    TRACE_SEND(" FAILED [%s]\n\n", bcp_errStr[errCode]);
    break;

  default:
    /* Unexpected response */
    errCode = BCP_ERR_ID_eUNEXPECTED;
    TRACE_FBLCTL_INFO("FBLCTL: Unexpected MSG received\n");
    TRACE_SEND(" FAILED [%s]\n\n", bcp_errStr[BCP_ERR_ID_eUNEXPECTED]);
    break;
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

static uint32 fblctl_procActivateRsp(T_FBLCTL_DATA* ctlData, T_PDU* rspPdu)
{
  T_BCP_MSG* rspMsg;
  uint32 errCode = BCP_ERR_ID_eNONE;

  TRACE_FBLCTL_API("FBLCTL: fblctl_procActivateRsp()\n");

  rspMsg = (T_BCP_MSG*)rspPdu->data;
  switch(rspMsg->msgType)
  {
  case FBL_MSG_ID_eACK_RSP:
    /* Positive response */
    TRACE_FBLCTL_INFO("FBLCTL: ACK received\n");
    TRACE_SEND(" OK\n\n");
    break;

  case FBL_MSG_ID_eNAK_RSP:
    /* Negative response */
    errCode = ((T_FBL_MSG_NAK_RSP*)(void*)rspMsg)->errCode;
    TRACE_FBLCTL_INFO("FBLCTL: NAK received (%02x)\n", errCode);
    TRACE_SEND(" FAILED [%s]\n\n", bcp_errStr[errCode]);
    break;

  default:
    /* Unexpected response */
    errCode = BCP_ERR_ID_eUNEXPECTED;
    TRACE_FBLCTL_INFO("FBLCTL: Unexpected MSG received\n");
    TRACE_SEND(" FAILED [%s]\n\n", bcp_errStr[BCP_ERR_ID_eUNEXPECTED]);
    break;
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

static uint32 fblctl_procRestartRsp(T_FBLCTL_DATA* ctlData, T_PDU* rspPdu)
{
  T_BCP_MSG* rspMsg;
  uint32 errCode = BCP_ERR_ID_eNONE;

  TRACE_FBLCTL_API("FBLCTL: fblctl_procResetRsp()\n");

  rspMsg = (T_BCP_MSG*)rspPdu->data;
  switch(rspMsg->msgType)
  {
  case FBL_MSG_ID_eACK_RSP:
    /* Positive response */
    TRACE_FBLCTL_INFO("FBLCTL: ACK received\n");
    TRACE_SEND(" OK\n\n");
    break;

  case FBL_MSG_ID_eNAK_RSP:
    /* Negative response */
    errCode = ((T_FBL_MSG_NAK_RSP*)(void*)rspMsg)->errCode;
    TRACE_FBLCTL_INFO("FBLCTL: NAK received (%02x)\n", errCode);
    TRACE_SEND(" FAILED [%s]\n\n", bcp_errStr[errCode]);
    break;

  default:
    /* Unexpected response */
    errCode = BCP_ERR_ID_eUNEXPECTED;
    TRACE_FBLCTL_INFO("FBLCTL: Unexpected MSG received\n");
    TRACE_SEND(" FAILED [%s]\n\n", bcp_errStr[BCP_ERR_ID_eUNEXPECTED]);
    break;
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

void fblctl_init(void)
{
  T_FBLCTL_DATA* ctlData;

  TRACE_FBLCTL_API("FBLCTL: fblctl_init()\n");
  ctlData = fblctl_getCtlData();
  if(NULL == ctlData)
  {
    /* Invalid ctlID */
  }
  else
  {
    memset(ctlData, 0, sizeof(T_FBLCTL_DATA));

    TRACE_FBLCTL_STATE("FBLCTL State: RESET -> IDLE\n");

    ctlData->state = FBLCTL_STATE_eIDLE;
    ctlData->jobState = FBLCTL_JOB_STATE_eOK;
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

void fblctl_run(void)
{
  T_FBLCTL_DATA* ctlData;

//  TRACE_FBLCTL_API("FBLCTL: fblctl_run()\n");

  /* Get controller runtime data */
  ctlData = fblctl_getCtlData();
  if(NULL == ctlData)
  {
    /* Invalid ctlID */
    TRACE_FBLCTL_ERROR("FBLCTL Error: Invalid ctlID\n");
  }
  else if(FBLCTL_STATE_eIDLE == ctlData->state)
  {
    /* Controller is idle, so do nothing and keep state. */
  }
  /* Controller has an active job running, so wait for protocol service to finish */
  else if(BCP_JOB_RESULT_ePENDING == bcp_getJobResult())
  {
    /* Protocol service is still pending */
  }
  else if(BCP_JOB_RESULT_eOK != bcp_getJobResult())
  {
    /* Protocol service failed */
    TRACE_FBLCTL_ERROR("FBLCTL Error: BCP failed\n");
    // TODO: What to do here?
  }
  else if(FBLCTL_STATE_eSEND_REQ == ctlData->state)
  {
    /* Request has been transmitted, switch to reception */
    bcp_listen();

    TRACE_FBLCTL_STATE("FBLCTL State: SEND_REQ -> RECV_RSP\n");
    ctlData->state = FBLCTL_STATE_eRECV_RSP;
  }
  else if(FBLCTL_STATE_eRECV_RSP == ctlData->state)
  {
    T_PDU rspPdu;

    if(STATUS_eOK != bcp_getRxMsg(&rspPdu))
    {
      /* Failed to get receive PDU */
      TRACE_FBLCTL_ERROR("FBLCTL Error: BCP failed to get MSG\n");
      // TODO: What to do here?
    }
    else
    {
      /* Successfully taken PDU */
      uint32 errCode;
      
      switch(ctlData->jobData.type)
      {
      case FBLCTL_JOB_TYPE_eBOOTSTRAP:
        errCode = fblctl_procBootstrapRsp(ctlData, &rspPdu);
        break;

      case FBLCTL_JOB_TYPE_eSWINFO:
        errCode = fblctl_procSwInfoRsp(ctlData, &rspPdu);
        break;

      case FBLCTL_JOB_TYPE_eINVALIDATE:
        errCode = fblctl_procInvalidateRsp(ctlData, &rspPdu);
        break;

      case FBLCTL_JOB_TYPE_eERASE:
        errCode = fblctl_procEraseRsp(ctlData, &rspPdu);
        break;

      case FBLCTL_JOB_TYPE_ePROGRAM:
        errCode = fblctl_procProgramRsp(ctlData, &rspPdu);
        break;

      case FBLCTL_JOB_TYPE_eACTIVATE:
        errCode = fblctl_procActivateRsp(ctlData, &rspPdu);
        break;

      case FBLCTL_JOB_TYPE_eRESET:
        errCode = fblctl_procRestartRsp(ctlData, &rspPdu);
        break;

      default:
        /* Unknown job type */
        break;
      }
      TRACE_FBLCTL_STATE("FBLCTL State: RECV_RSP -> IDLE\n");
      ctlData->state = FBLCTL_STATE_eIDLE;
      ctlData->jobData.type = FBLCTL_JOB_TYPE_eNONE;

      fblctl_signalJobEnd(ctlData, errCode);
    }
  }
  else
  {
    /* Invalid state */
    ctlData->state = FBLCTL_STATE_eIDLE;
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

void fblctl_registerCallback(void (*newCbk)(uint32))
{  
  T_FBLCTL_DATA* ctlData = fblctl_getCtlData();

  ctlData->cbk = newCbk;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

T_STATUS fblctl_getState(void)
{
  T_FBLCTL_DATA* ctlData;
  T_STATUS result = -1;

  ctlData = fblctl_getCtlData();
  if(NULL == ctlData)
  {
    /* Invalid ctlID */
    TRACE_FBLCTL_ERROR("FBLCTL Error: Invalid ctlID\n");
  }
  else if(FBLCTL_STATE_eIDLE != ctlData->state)
  {
    /* Not idle */
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

void fblctl_injectError(uint32 err)
{
  T_FBLCTL_DATA* ctlData = fblctl_getCtlData();

  TRACE_FBLCTL_INFO("FBLCTL: inject error (%d)\n", err);
  ctlData->injectErr = err;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 * @return Status of request
 *
 * @retval STATUS_eOK - If the request was accepted
 * @retval STATUS_eNOK - If the request was not accepted due to one of the
 *   following reaseons:
 *   1. Controller runtime data could not be fetched
 *   2. Controller is not in state IDLE
 ******************************************************************************
 */

T_STATUS fblctl_sendBootstrapReq(void)
{
  T_FBLCTL_JOB_DATA* job;
  T_FBLCTL_DATA* ctlData;
  T_STATUS result = STATUS_eNOK;
  T_PDU  reqPdu;

  TRACE_FBLCTL_API("FBLCTL: fblctl_sendBootstrapReq()\n");

  TRACE_SEND("CMD Bootstrap...\n");
  /* Get controller runtime data */
  ctlData = fblctl_getCtlData();
  if(NULL == ctlData)
  {
    /* Invalid ctlID */
    TRACE_FBLCTL_ERROR("FBLCTL Error: Invalid ctlID\n");
  }
  else if(FBLCTL_STATE_eIDLE != ctlData->state)
  {
    /* Controller is busy */
    TRACE_FBLCTL_ERROR("FBLCTL Error: Conroller busy\n");
  }
  else if(STATUS_eOK != bcp_getTxPdu(&reqPdu))
  {
    /* Failed to allocate tansmit PDU */
    TRACE_FBLCTL_ERROR("FBLCTL Error: BCP failed to alloc PDU\n");
  }
  else
  {
    uint8 entryToken[FBL_MSG__TOKEN_SIZE] = FBL_BOOTSTRAP_TOKEN;
    T_FBL_MSG_ENTRY_REQ* reqMsg = (T_FBL_MSG_ENTRY_REQ*)reqPdu.data;
    job = &ctlData->jobData;

    reqMsg->msgType = FBL_MSG_ID_eBOOTSTRAP_REQ;

    if(3 == ctlData->injectErr)
    {
      entryToken[0] = 'b';
    }
    if(2 == ctlData->injectErr)
    {
      reqMsg->msgType = FBL_MSG_ID_eINVALID;
    }
     
    memcpy((void*)&reqMsg->bootMagic, entryToken, sizeof(reqMsg->bootMagic));
    reqPdu.len = sizeof(T_FBL_MSG_ENTRY_REQ);
     
    if(1 == ctlData->injectErr)
    {
      reqPdu.len++;
    }
     
    result = bcp_sendMsg(&reqPdu);
  }
     
  if(STATUS_eOK == result)
  {
    /* Successfully requested to send message */
    TRACE_FBLCTL_STATE("FBLCTL State: IDLE -> SEND_REQ [Job=BOOTSTRAP]\n");
    ctlData->state = FBLCTL_STATE_eSEND_REQ;
    job->type = FBLCTL_JOB_TYPE_eBOOTSTRAP;
    job->timeout = 2 * reqPdu.len;
     
    /* Set number of retries */
    //tries = 10;
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

T_STATUS fblctl_sendSwInfoReq(void)
{
  T_FBLCTL_JOB_DATA* job;
  T_FBLCTL_DATA* ctlData;
  T_STATUS result = STATUS_eNOK;
  T_PDU  reqPdu;

  TRACE_FBLCTL_API("FBLCTL: fblctl_sendSwInfoReq()\n");

  TRACE_SEND("CMD SoftwareInfo...\n");
  /* Get controller runtime data */
  ctlData = fblctl_getCtlData();
  if(NULL == ctlData)
  {
    /* Invalid ctlID */
    TRACE_FBLCTL_ERROR("FBLCTL Error: Invalid ctlID\n");
  }
  else if(FBLCTL_STATE_eIDLE != ctlData->state)
  {
    /* Controller is busy */
    TRACE_FBLCTL_ERROR("FBLCTL Error: Conroller busy\n");
  }
  else if(STATUS_eOK != bcp_getTxPdu(&reqPdu))
  {
    /* Failed to allocate tansmit PDU */
    TRACE_FBLCTL_ERROR("FBLCTL Error: BCP failed to alloc PDU\n");
  }
  else
  {
    T_FBL_MSG_SWINFO_REQ* reqMsg = (T_FBL_MSG_SWINFO_REQ*)reqPdu.data;
    job = &ctlData->jobData;

    reqMsg->msgType = FBL_MSG_ID_eSWINFO_REQ;
    reqPdu.len = sizeof(T_FBL_MSG_SWINFO_REQ);

    result = bcp_sendMsg(&reqPdu);
  }

  if(STATUS_eOK == result)
  {
    /* Successfully requested to send message */
    TRACE_FBLCTL_STATE("FBLCTL State: IDLE -> SEND_REQ [Job=SWINFO]\n");
    ctlData->state = FBLCTL_STATE_eSEND_REQ;

    job->type = FBLCTL_JOB_TYPE_eSWINFO;
    job->timeout = 2 * reqPdu.len;
     
    /* Set number of retries */
    //tries = 10;
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

T_STATUS fblctl_sendInvalidateReq(uint32 dstAddr, uint32 len)
{
  T_FBLCTL_JOB_DATA* job;
  T_FBLCTL_DATA* ctlData;
  T_STATUS result = STATUS_eNOK;
  T_PDU  reqPdu;

  TRACE_FBLCTL_API("FBLCTL: fblctl_sendInvalidateReq()\n");

  TRACE_SEND("CMD Invalidate...\n");
  /* Get controller runtime data */
  ctlData = fblctl_getCtlData();
  if(NULL == ctlData)
  {
    /* Invalid ctlID */
    TRACE_FBLCTL_ERROR("FBLCTL Error: Invalid ctlID\n");
  }
  else if(FBLCTL_STATE_eIDLE != ctlData->state)
  {
    /* Controller is busy */
    TRACE_FBLCTL_ERROR("FBLCTL Error: Conroller busy\n");
  }
  else if(STATUS_eOK != bcp_getTxPdu(&reqPdu))
  {
    /* Failed to allocate tansmit PDU */
    TRACE_FBLCTL_ERROR("FBLCTL: BCP failed to alloc PDU\n");
  }
  else
  {
    T_FBL_MSG_INVALIDATE_REQ* reqMsg = (T_FBL_MSG_INVALIDATE_REQ*)reqPdu.data;
    job = &ctlData->jobData;

    reqMsg->msgType = FBL_MSG_ID_eINVALIDATE_REQ;
    reqMsg->blkAddr = dstAddr;
    reqMsg->blkSize = len;
    reqPdu.len = sizeof(T_FBL_MSG_INVALIDATE_REQ);

    result = bcp_sendMsg(&reqPdu);
  }

  if(STATUS_eOK == result)
  {
    TRACE_FBLCTL_STATE("FBLCTL State: IDLE -> SEND_REQ [Job=INVALIDATE]\n");
    ctlData->state = FBLCTL_STATE_eSEND_REQ;
    job->type = FBLCTL_JOB_TYPE_eINVALIDATE;
    job->timeout = 2 * reqPdu.len;

    /* Set number of retries */
    //tries = 10;
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

T_STATUS fblctl_sendEraseReq(uint32 dstAddr, uint32 len)
{
  T_FBLCTL_JOB_DATA* job;
  T_FBLCTL_DATA* ctlData;
  T_STATUS result = STATUS_eNOK;
  T_PDU  reqPdu;

  TRACE_FBLCTL_API("FBLCTL: fblctl_sendEraseReq()\n");

  TRACE_SEND("CMD Erase...\n");
  /* Get controller runtime data */
  ctlData = fblctl_getCtlData();
  if(NULL == ctlData)
  {
    /* Invalid ctlID */
    TRACE_FBLCTL_ERROR("FBLCTL Error: Invalid ctlID\n");
  }
  else if(FBLCTL_STATE_eIDLE != ctlData->state)
  {
    /* Controller is busy */
    TRACE_FBLCTL_ERROR("FBLCTL Error: Conroller busy\n");
  }
  else if(STATUS_eOK != bcp_getTxPdu(&reqPdu))
  {
    /* Failed to allocate tansmit PDU */
    TRACE_FBLCTL_ERROR("FBLCTL Error: BCP failed to alloc PDU\n");
  }
  else
  {
    T_FBL_MSG_ERASE_REQ* reqMsg = (T_FBL_MSG_ERASE_REQ*)reqPdu.data;
    job = &ctlData->jobData;

    reqMsg->msgType = FBL_MSG_ID_eERASE_REQ;
    reqMsg->blkAddr = dstAddr;
    reqMsg->blkSize = len;
    reqPdu.len = sizeof(T_FBL_MSG_ERASE_REQ);

    result = bcp_sendMsg(&reqPdu);
  }

  if(STATUS_eOK == result)
  {
    TRACE_FBLCTL_STATE("FBLCTL State: IDLE -> SEND_REQ [Job=ERASE]\n");

    ctlData->state = FBLCTL_STATE_eSEND_REQ;
    job->type = FBLCTL_JOB_TYPE_eERASE;
    job->timeout = 2 * reqPdu.len;

    /* Set number of retries */
    //tries = 10;
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

T_STATUS fblctl_sendProgramReq(uint32 dstAddr, uint32 (*readSrc)(uint8* msgBuf, uint32 bufSize, uint32 dstAddr))
{
  T_FBLCTL_JOB_DATA* job;
  T_FBLCTL_DATA* ctlData;
  T_STATUS result = STATUS_eNOK;
  T_PDU  reqPdu;

  TRACE_FBLCTL_API("FBLCTL: fblctl_sendProgramReq()\n");

  TRACE_SEND("CMD Program...\n");
  /* Get controller runtime data */
  ctlData = fblctl_getCtlData();
  if(NULL == ctlData)
  {
    /* Invalid ctlID */
    TRACE_FBLCTL_ERROR("FBLCTL Error: Invalid ctlID\n");
  }
  else if(FBLCTL_STATE_eIDLE != ctlData->state)
  {
    /* Controller is busy */
    TRACE_FBLCTL_ERROR("FBLCTL Error: Conroller busy\n");
  }
  else if(STATUS_eOK != bcp_getTxPdu(&reqPdu))
  {
    /* Failed to allocate tansmit PDU */
    TRACE_FBLCTL_ERROR("FBLCTL Error: BCP failed to alloc PDU\n");
  }
  else
  {
    T_FBL_MSG_PROGRAM_REQ* reqMsg = (T_FBL_MSG_PROGRAM_REQ*)reqPdu.data;
    job = &ctlData->jobData;

    reqMsg->msgType = FBL_MSG_ID_ePROGRAM_REQ;
    reqMsg->blkAddr = dstAddr;
    readSrc(reqMsg->blkData, FBL_BLK_SIZE, dstAddr);
    reqPdu.len = sizeof(T_FBL_MSG_PROGRAM_REQ);

    result = bcp_sendMsg(&reqPdu);
  }

  if(STATUS_eOK == result)
  {
    TRACE_FBLCTL_STATE("FBLCTL State: IDLE -> SEND_REQ [Job=PROGRAM]\n");

    ctlData->state = FBLCTL_STATE_eSEND_REQ;
    job->type = FBLCTL_JOB_TYPE_ePROGRAM;
    job->timeout = 2 * reqPdu.len;

    /* Set number of retries */
    //tries = 10;
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

T_STATUS fblctl_sendActivateReq(uint32 dstAddr, uint32 len)
{
  T_FBLCTL_JOB_DATA* job;
  T_FBLCTL_DATA* ctlData;
  T_STATUS result = STATUS_eNOK;
  T_PDU  reqPdu;

  TRACE_FBLCTL_API("FBLCTL: fblctl_sendActivateReq()\n");

  TRACE_SEND("CMD Activate...\n");
  /* Get controller runtime data */
  ctlData = fblctl_getCtlData();
  if(NULL == ctlData)
  {
    /* Invalid ctlID */
    TRACE_FBLCTL_ERROR("FBLCTL Error: Invalid ctlID\n");
  }
  else if(FBLCTL_STATE_eIDLE != ctlData->state)
  {
    /* Controller is busy */
    TRACE_FBLCTL_ERROR("FBLCTL Error: Conroller busy\n");
  }
  else if(STATUS_eOK != bcp_getTxPdu(&reqPdu))
  {
    /* Failed to allocate tansmit PDU */
    TRACE_FBLCTL_ERROR("FBLCTL Error: BCP failed to alloc PDU\n");
  }
  else
  {
    T_FBL_MSG_ACTIVATE_REQ* reqMsg = (T_FBL_MSG_ACTIVATE_REQ*)reqPdu.data;
    job = &ctlData->jobData;
  
    reqMsg->msgType = FBL_MSG_ID_eACTIVATE_REQ;
    reqMsg->blkAddr = dstAddr;
    reqMsg->blkSize = len;
    reqPdu.len = sizeof(T_FBL_MSG_ACTIVATE_REQ);
    job->timeout = 2 * reqPdu.len;
  
    result = bcp_sendMsg(&reqPdu);
  }

  if(STATUS_eOK == result)
  {
    TRACE_FBLCTL_STATE("FBLCTL State: IDLE -> SEND_REQ [Job=ACTIVATE]\n");

    ctlData->state = FBLCTL_STATE_eSEND_REQ;
    job->type = FBLCTL_JOB_TYPE_eACTIVATE;
    job->timeout = 2 * reqPdu.len;

    /* Set number of retries */
    //tries = 10;
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

T_STATUS fblctl_sendRestartReq(void)
{
  T_FBLCTL_JOB_DATA* job;
  T_FBLCTL_DATA* ctlData;
  T_STATUS result = STATUS_eNOK;
  T_PDU  reqPdu;

  TRACE_FBLCTL_API("FBLCTL: fblctl_sendRestartReq()\n");

  TRACE_SEND("CMD Restart...\n");
  /* Get controller runtime data */
  ctlData = fblctl_getCtlData();
  if(NULL == ctlData)
  {
    /* Invalid ctlID */
    TRACE_FBLCTL_ERROR("FBLCTL Error: Invalid ctlID\n");
  }
  else if(FBLCTL_STATE_eIDLE != ctlData->state)
  {
    /* Controller is busy */
    TRACE_FBLCTL_ERROR("FBLCTL Error: Conroller busy\n");
  }
  else if(STATUS_eOK != bcp_getTxPdu(&reqPdu))
  {
    /* Failed to allocate tansmit PDU */
    TRACE_FBLCTL_ERROR("FBLCTL Error: BCP failed to alloc PDU\n");
  }
  else
  {
    T_FBL_MSG_RESET_REQ* reqMsg = (T_FBL_MSG_RESET_REQ*)reqPdu.data;
    job = &ctlData->jobData;
  
    reqMsg->msgType = FBL_MSG_ID_eRESET_REQ;
    reqPdu.len = sizeof(T_FBL_MSG_RESET_REQ);
    job->timeout = 2 * reqPdu.len;
  
    result = bcp_sendMsg(&reqPdu);
  }

  if(STATUS_eOK == result)
  {
    TRACE_FBLCTL_STATE("FBLCTL State: IDLE -> SEND_REQ [Job=RESET]\n");

    ctlData->state = FBLCTL_STATE_eSEND_REQ;
    job->type = FBLCTL_JOB_TYPE_eRESET;
    job->timeout = 2 * reqPdu.len;

    /* Set number of retries */
    //tries = 10;
  }
  return result;
}


