#ifndef UPD_SEQ_C
#define UPD_SEQ_C
#endif /* UPD_SEQ_C */

#include "bsp.h"

#include "trace_pub.h"
#include "swinfo.h"
#include "target_cfg.h"
#include "fbl_defs.h"
#include "fbl_cmd.h"
#include "upd_seq.h"
#include "crc32.h"

#include <stdio.h>
#include <string.h> /* for memset() */
#include <stddef.h> /* for offsetof() */

#if (TRC_FEAT_UPDSEQ_ENA == STD_ON)
#define TRACE_UPDSEQ_API(...)   TRACE_FEATURE(TRC_FEAT_ID_eUPDSEQ, TRACE_FEATURE_CLASS3, __VA_ARGS__)
#define TRACE_UPDSEQ_INFO(...)  TRACE_FEATURE(TRC_FEAT_ID_eUPDSEQ, TRACE_FEATURE_CLASS2, __VA_ARGS__)
#define TRACE_UPDSEQ_ERROR(...) TRACE_FEATURE(TRC_FEAT_ID_eUPDSEQ, TRACE_FEATURE_CLASS0, __VA_ARGS__)
#define TRACE_UPDSEQ_STATE(...) TRACE_FEATURE(TRC_FEAT_ID_eUPDSEQ, TRACE_FEATURE_CLASS1, __VA_ARGS__)
#else /* (TRC_FEAT_UPDSEQ_ENA != STD_ON) */
#define TRACE_UPDSEQ_API(...)   /* empty */
#define TRACE_UPDSEQ_INFO(...)  /* empty */
#define TRACE_UPDSEQ_ERROR(...) /* empty */
#define TRACE_UPDSEQ_STATE(...) /* empty */
#endif /* (TRC_FEAT_UPDSEQ_ENA) */


typedef enum UPDSEQ_STATE
{
  UPDSEQ_STATE_eRESET = 0,
  UPDSEQ_STATE_eERROR,
  UPDSEQ_STATE_eINIT,
  UPDSEQ_STATE_eSTARTED,
  UPDSEQ_STATE_eINVALIDATE_REQ,
  UPDSEQ_STATE_eINVALIDATE_CNF,
  UPDSEQ_STATE_eERASE_REQ,
  UPDSEQ_STATE_eERASE_CNF,
  UPDSEQ_STATE_ePROGRAM_REQ,
  UPDSEQ_STATE_eWRBLK_REQ,
  UPDSEQ_STATE_eWRBLK_CNF,
  UPDSEQ_STATE_ePROGRAM_CNF,
  UPDSEQ_STATE_eACTIVATE_REQ,
  UPDSEQ_STATE_eACTIVATE_CNF,
  UPDSEQ_STATE_eRESTART_REQ,
  UPDSEQ_STATE_eRESTART_CNF,
}T_UPDSEQ_STATE;


typedef struct
{
  uint32 srcAddr;
  uint32 dstAddr;
  uint32 length;
}T_UPD_DESC;


typedef struct
{
  const char*    fileName;
  FILE*          srcFile;
  T_UPD_DESC     updDesc;
  T_UPDSEQ_STATE state;
  uint16         blkNum;
  void          (*cbk)(uint32);
  T_CRC32_DATA   crcCtx;
}T_UPDSEQ_DATA;


static T_UPDSEQ_DATA updseq_dataTbl[1];

/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static T_UPDSEQ_DATA* updseq_getCtlData(void)
{
  return updseq_dataTbl;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void updseq_signalJobEnd(T_UPDSEQ_DATA* ctlData, uint32 errCode)
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

static void updseq_finishCmd(uint32 errCode)
{
  T_UPDSEQ_DATA* ctlData = updseq_getCtlData();

  if(BCP_ERR_ID_eNONE != errCode)
  {
    ctlData->state = UPDSEQ_STATE_eERROR;
  }
  
  switch(ctlData->state)
  {
  case UPDSEQ_STATE_eINVALIDATE_REQ:
    ctlData->state = UPDSEQ_STATE_eINVALIDATE_CNF;
    break;

  case UPDSEQ_STATE_eERASE_REQ:
    ctlData->state = UPDSEQ_STATE_eERASE_CNF;
    break;

  case UPDSEQ_STATE_eWRBLK_REQ:
    ctlData->state = UPDSEQ_STATE_eWRBLK_CNF;
    break;

  case UPDSEQ_STATE_eACTIVATE_REQ:
    ctlData->state = UPDSEQ_STATE_eACTIVATE_CNF;
    break;

  case UPDSEQ_STATE_eRESTART_REQ:
    ctlData->state = UPDSEQ_STATE_eRESTART_CNF;
    break;
  
  default:
    /* We do not expect to get called in any other state */
    ctlData->state = UPDSEQ_STATE_eERROR;
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

static uint32 updseq_readSrc(uint8* msgBuf, uint32 bufSize, uint32 dstAddr)
{
  T_UPDSEQ_DATA* ctlData = updseq_getCtlData();
  uint32 bytesRead;

  bytesRead = fread((void*)msgBuf, 1, bufSize, ctlData->srcFile);
  return bytesRead;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

uint32 updseq_calcImageCrc(T_UPDSEQ_DATA* ctlData)
{
  uint32 imgCrc;
  uint8  buffer[sizeof(uint32)];
  uint32 chunkAddr;
  uint16 bytesRead;

  /* Preset CRC with initial value */
  crc32_preset(&ctlData->crcCtx, 0xFFFFFFFF);

  for(chunkAddr = 0; chunkAddr < ctlData->updDesc.length; chunkAddr += sizeof(buffer))
  {
    /* Read chunk of data from image to be downloaded */
    bytesRead = fread((void*)buffer, 1, sizeof(buffer), ctlData->srcFile);

    if(chunkAddr == FBL_APP_SWINFO_OFFS + offsetof(T_SWINFO, crc))
    {
      /* Current chunk contains the CRC value and must be replaced
       * by the default value in order to get the correct final FCS
       */
      uint32 defaultCrcValue = 0;
      memcpy(buffer, &defaultCrcValue, sizeof(buffer));
    }
    /* Process chunk for CRC calculation */
    crc32_updateSTabRev(&ctlData->crcCtx, buffer, bytesRead);
  }
  /* Finalize CRC calculation by applying XOR value */
  crc32_finalize(&ctlData->crcCtx, 0xFFFFFFFF);

  /* Get calculated CRC and compare with images stored CRC */
  imgCrc = crc32_read(&ctlData->crcCtx);
  return imgCrc;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void updseq_init(const char* fileName)
{
  T_UPDSEQ_DATA* ctlData = updseq_getCtlData();
  
  memset(ctlData, 0, sizeof(T_UPDSEQ_DATA));
  
  crc32_configCtx(&ctlData->crcCtx, crc32_tblP04C11DB7R);

  ctlData->fileName = fileName;
  ctlData->state = UPDSEQ_STATE_eINIT;  
  TRACE_UPDSEQ_STATE("UPDSEQ State: RESET -> IDLE\n");
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

T_STATUS updseq_start(void)
{
  T_UPDSEQ_DATA* ctlData = updseq_getCtlData();
  T_SWINFO swInfo;
  T_STATUS result = STATUS_eNOK;
  uint32   imgCrc = 0x12345678;

  if(NULL != ctlData->srcFile)
  {
    /* Close potentially open source file */
    fclose(ctlData->srcFile);
  }

  /* Open the source file */
  ctlData->srcFile = fopen(ctlData->fileName, "rb");

  if(NULL == ctlData->srcFile)
  {
    /* Failed to open */
  }
  /* Position file pointer at SW info */
  else if(0 != fseek(ctlData->srcFile, FBL_APP_SWINFO_OFFS, SEEK_SET))
  {
    /* Failed */
  }
  /* Read SW info */
  else if(1 != fread((void*)&swInfo, sizeof(T_SWINFO), 1, ctlData->srcFile))
  {
    /* Failed */
  }
  /* Rewind file to offset 0 */
  else if(0 != fseek(ctlData->srcFile, 0, SEEK_SET))
  {
    /* Failed */
  }
  else
  {
    /* Successfully read SW info */
    uint32 numBlocks;
    ctlData->updDesc.dstAddr = swInfo.imgAddr;
    ctlData->updDesc.length = swInfo.imgSize;
    ctlData->updDesc.srcAddr = 0;

    numBlocks = (swInfo.imgSize + FBL_BLK_SIZE - 1) / FBL_BLK_SIZE;

    fprintf(stderr, "Image info:\n");
    fprintf(stderr, "  addr=%08X\n"
                    "  size=%08X (%d Bytes)\n"
                    "  nblk=%d\n"
                    "  crc= %08X\n",
                    swInfo.imgAddr, swInfo.imgSize,  swInfo.imgSize, numBlocks, swInfo.crc);

    imgCrc = updseq_calcImageCrc(ctlData);
    if(swInfo.crc != imgCrc)
    {
      fprintf(stderr, "Invalid CRC: %08x (expected %08x)\n", swInfo.crc, imgCrc);
    }
    else if(0 != fseek(ctlData->srcFile, 0, SEEK_SET))
    {
      /* Failed */
    }
    else
    {
      result = STATUS_eOK;
    }
  }

  if(STATUS_eOK != result)
  {
    fclose(ctlData->srcFile);
  }
  else
  {
    fblctl_registerCallback(&updseq_finishCmd);
    ctlData->state = UPDSEQ_STATE_eSTARTED;
    TRACE_UPDSEQ_STATE("UPDSEQ State: IDLE -> STARTED\n");
  }
  return STATUS_eOK;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void updseq_run(void)
{
  T_UPDSEQ_DATA* ctlData = updseq_getCtlData();

  switch(ctlData->state)
  {
  case UPDSEQ_STATE_eRESET:
    break;
    
  case UPDSEQ_STATE_eINIT:
  case UPDSEQ_STATE_eINVALIDATE_REQ:
  case UPDSEQ_STATE_eERASE_REQ:
  case UPDSEQ_STATE_eWRBLK_REQ:
  case UPDSEQ_STATE_eACTIVATE_REQ:
  case UPDSEQ_STATE_eRESTART_REQ:
    break;

  case UPDSEQ_STATE_eSTARTED:
    TRACE_SEND("SEQ Invalidate     (%08x .. %08x)...\n", ctlData->updDesc.dstAddr, ctlData->updDesc.dstAddr + ctlData->updDesc.length - 1);
    TRACE_UPDSEQ_INFO("UPDSEQ: Invalidate from %08x to %08x...\n", ctlData->updDesc.dstAddr, ctlData->updDesc.dstAddr + ctlData->updDesc.length - 1);
    TRACE_UPDSEQ_STATE("UPDSEQ State: STARTED -> INVALIDATE_REQ\n");
    ctlData->blkNum = 0;
    ctlData->state = UPDSEQ_STATE_eINVALIDATE_REQ;
    fblctl_sendInvalidateReq(ctlData->updDesc.dstAddr, ctlData->updDesc.length);
    break;

  case UPDSEQ_STATE_eINVALIDATE_CNF:
    TRACE_SEND("SEQ Erase          (%08x .. %08x)...\n", ctlData->updDesc.dstAddr, ctlData->updDesc.dstAddr + ctlData->updDesc.length - 1);
    TRACE_UPDSEQ_INFO("UPDSEQ: Erase from %08x to %08x...\n", ctlData->updDesc.dstAddr, ctlData->updDesc.dstAddr + ctlData->updDesc.length - 1);
    TRACE_UPDSEQ_STATE("UPDSEQ State: INVALIDATE_CNF -> ERASE_REQ\n");
    ctlData->state = UPDSEQ_STATE_eERASE_REQ;
    fblctl_sendEraseReq(ctlData->updDesc.dstAddr, ctlData->updDesc.length);
    break;

  case UPDSEQ_STATE_eERASE_CNF:
    TRACE_SEND("SEQ Program Img    (%08x .. %08x)...\n", ctlData->updDesc.dstAddr, ctlData->updDesc.dstAddr + ctlData->updDesc.length - 1);
    TRACE_UPDSEQ_INFO("UPDSEQ: Program Image from %08x to %08x...\n", ctlData->updDesc.dstAddr, ctlData->updDesc.dstAddr + ctlData->updDesc.length - 1);
    TRACE_UPDSEQ_STATE("UPDSEQ State: ERASE_CNF -> PROGRAM_REQ\n");
    ctlData->state = UPDSEQ_STATE_ePROGRAM_REQ;
    break;

  case UPDSEQ_STATE_ePROGRAM_REQ:
  case UPDSEQ_STATE_eWRBLK_CNF:
    if((ctlData->blkNum * FBL_BLK_SIZE) < ctlData->updDesc.length)
    {
      uint32 dstAddr = ctlData->updDesc.dstAddr + ctlData->blkNum * FBL_BLK_SIZE;
      uint32 endAddr = dstAddr + FBL_BLK_SIZE - 1;

      TRACE_SEND("SEQ Program Blk#%2d (%08x .. %08x)...\n", ctlData->blkNum, dstAddr, endAddr);
      TRACE_UPDSEQ_INFO("UPDSEQ: Program blk#%d from %08x to %08x...\n", ctlData->blkNum, dstAddr, endAddr);
      TRACE_UPDSEQ_STATE("UPDSEQ State: WRBLK_CNF -> WRBLK_REQ\n");
      ctlData->state = UPDSEQ_STATE_eWRBLK_REQ;
      fblctl_sendProgramReq(dstAddr, &updseq_readSrc);
      ctlData->blkNum++;
    }
    else
    {
      TRACE_UPDSEQ_STATE("UPDSEQ State: WRBLK_CNF -> PROGRAM_CNF\n");
      ctlData->state = UPDSEQ_STATE_ePROGRAM_CNF;
    }
    break;

  case UPDSEQ_STATE_ePROGRAM_CNF:
    TRACE_SEND("SEQ Activate Img   (%08x .. %08x)...\n", ctlData->updDesc.dstAddr, ctlData->updDesc.dstAddr + ctlData->updDesc.length - 1);
    TRACE_UPDSEQ_INFO("UPDSEQ: Activate from %08x to %08x...\n", ctlData->updDesc.dstAddr, ctlData->updDesc.dstAddr + ctlData->updDesc.length - 1);
    TRACE_UPDSEQ_STATE("UPDSEQ State: PROGRAM_CNF -> ACTIVATE_REQ\n");
    ctlData->state = UPDSEQ_STATE_eACTIVATE_REQ;
    fblctl_sendActivateReq(ctlData->updDesc.dstAddr, ctlData->updDesc.length);
    break;

  case UPDSEQ_STATE_eACTIVATE_CNF:
  default:
#if 0
    TRACE_SEND("SEQ Restart...\n");
    TRACE_UPDSEQ_INFO("UPDSEQ: Restart...\n");
    TRACE_UPDSEQ_STATE("UPDSEQ State: ACTIVATE_CNF -> RESTART_REQ\n");
    ctlData->state = UPDSEQ_STATE_eRESTART_REQ;
    fblctl_sendRestartReq();
#else
    TRACE_SEND("SEQ Done...\n");
    TRACE_UPDSEQ_INFO("UPDSEQ: Done...\n");
    TRACE_UPDSEQ_STATE("UPDSEQ State: ACTIVATE_CNF -> INIT\n");
    ctlData->state = UPDSEQ_STATE_eINIT;
    updseq_signalJobEnd(ctlData, BCP_ERR_ID_eNONE);
#endif
    break;

  case UPDSEQ_STATE_eRESTART_CNF:
    TRACE_SEND("SEQ Done...\n");
    TRACE_UPDSEQ_STATE("UPDSEQ State: RESTART_CNF -> INIT\n");
    ctlData->state = UPDSEQ_STATE_eINIT;
    updseq_signalJobEnd(ctlData, BCP_ERR_ID_eNONE);
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

void updseq_registerCallback(void (*newCbk)(uint32))
{
  T_UPDSEQ_DATA* ctlData = updseq_getCtlData();

  ctlData->cbk = newCbk;
}


