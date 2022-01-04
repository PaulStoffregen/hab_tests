#ifndef HAB_INFO_C
#define HAB_INFO_C
#endif /* HAB_INFO_C */

#include "bsp.h"
#include "reg.h"
#include "trace_pub.h"
#include "libc.h"
#include "hab.h"
#include "hab_prv.h"
#include "hab_api.h"
#include "hab_info.h"


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Get textual name for given tag
 *
 ******************************************************************************
 */

static char* hab_getTagStr(uint8 tag)
{
  char* tagStr;  
  switch(tag)
  {
  case HAB_TAG_IVT:
    tagStr = "IVT";
    break;
  case HAB_TAG_DCD:
    tagStr = "DCD";
    break;
  case HAB_TAG_CSF:
    tagStr = "CSF";
    break;
  case HAB_TAG_CRT:
    tagStr = "CRT";
    break;
  case HAB_TAG_SIG:
    tagStr = "SIG";
    break;
  case HAB_TAG_EVT:
    tagStr = "EVT";
    break;
  case HAB_TAG_RVT:
    tagStr = "RVT";
    break;
  case HAB_TAG_WRP:
    tagStr = "WRP";
    break;
  case HAB_TAG_MAC:
    tagStr = "MAC";
    break;
  default:
    tagStr = "Unknown!";
    break;
  }
  return tagStr;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Get textual name for given status tag
 *
 ******************************************************************************
 */

static const char* hab_getStatusStr(uint8 statusTag)
{
  char* statusStr;  
  switch(statusTag)
  {
  case HAB_STATUS_ANY:
    statusStr = "HAB_ANY";
    break;
  case HAB_STATUS_FAIL:
    statusStr = "HAB_FAILED";
    break;
  case HAB_STATUS_WARN:
    statusStr = "HAB_WARNING";
    break;
  case HAB_STATUS_SUCCESS:
    statusStr = "HAB_SUCCESS";
    break;
  default:
    statusStr = "Unknown!";
    break;
  }
  return statusStr;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Get textual name for given reason tag
 *
 ******************************************************************************
 */

static const char* hab_getReasonStr(uint8 reasonTag)
{
  char* reasonStr;
  switch(reasonTag)
  {
  case HAB_RSN_INVAL_ADDRESS:
    reasonStr = "INVALID_ADDRESS";
    break;
  case HAB_RSN_ENG_FAIL:
    reasonStr = "ENGINE_FAIL";
    break;
  case HAB_RSN_INVAL_ASSERTION:
    reasonStr = "INVALID_ASSERTION";
    break;
  case HAB_RSN_INVAL_CALL:
    reasonStr = "INVALID_CALL";
    break;
  case HAB_RSN_INVAL_CERTIFICATE:
    reasonStr = "INVALID_CERT";
    break;
  case HAB_RSN_INVAL_COMMAND:
    reasonStr = "INVALID_COMMAND";
    break;
  case HAB_RSN_INVAL_CSF:
    reasonStr = "INVALID_CSF";
    break;
  case HAB_RSN_INVAL_DCD:
    reasonStr = "INVALID_DCD";
    break;
  case HAB_RSN_INVAL_INDEX:
    reasonStr = "INVALID_INDEX";
    break;
  case HAB_RSN_INVAL_IVT:
    reasonStr = "INVALID_IVT";
    break;
  case HAB_RSN_INVAL_KEY:
    reasonStr = "INVALID_KEY";
    break;
  case HAB_RSN_INVAL_RETURN:
    reasonStr = "INVALID_RETURN";
    break;
  case HAB_RSN_INVAL_SIGNATURE:
    reasonStr = "INVALID_SIGNATURE";
    break;
  case HAB_RSN_INVAL_SIZE:
    reasonStr = "INVALID_SIZE";
    break;
  case HAB_RSN_MEM_FAIL:
    reasonStr = "MEMORY_FAIL";
    break;
  case HAB_RSN_OVR_COUNT:
    reasonStr = "OVERRUN_COUNT";
    break;
  case HAB_RSN_OVR_STORAGE:
    reasonStr = "OVERRUN_STORAGE";
    break;
  case HAB_RSN_UNSUP_ALGORITHM:
    reasonStr = "UNSUPPORTED_ALGORYTHM";
    break;
  case HAB_RSN_UNSUP_COMMAND:
    reasonStr = "UNSUPPORTED_COMMAND";
    break;
  case HAB_RSN_UNSUP_ENGINE:
    reasonStr = "UNSUPPORTED_ENGINE";
    break;
  case HAB_RSN_UNSUP_ITEM:
    reasonStr = "UNSUPPORTED_ITEM";
    break;
  case HAB_RSN_UNSUP_KEY:
    reasonStr = "UNSUPPORTED_KEY";
    break;
  case HAB_RSN_UNSUP_PROTOCOL:
    reasonStr = "UNSUPPORTED_PROTOCOL";
    break;
  case HAB_RSN_UNSUP_STATE:
    reasonStr = "UNSUPPORTED_STATE";
    break;
  default:
    reasonStr = "Unknown!";
    break;
  }
  return reasonStr;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Get textual name for given command tag
 *
 ******************************************************************************
 */

static const char* hab_getCmdStr(uint8 cmdTag)
{
  char* cmdStr;
  switch(cmdTag)
  {
  case HAB_CMD_SET_ITEM:
    cmdStr = "SET_ITEM";
    break;
  case HAB_CMD_INS_KEY:
    cmdStr = "INS_KEY";
    break;
  case HAB_CMD_AUTH_DATA:
    cmdStr = "AUTH_DATA";
    break;
  case HAB_CMD_WRT_DATA:
    cmdStr = "WRT_DATA";
    break;
  case HAB_CMD_CHK_DATA:
    cmdStr = "CHK_DATA";
    break;
  case HAB_CMD_NOP:
    cmdStr = "NOP";
    break;
  case HAB_CMD_INIT:
    cmdStr = "INIT";
    break;
  case HAB_CMD_UNLOCK:
    cmdStr = "UNLOCK";
    break;
  default:
    cmdStr = "Unknown!";
    break;
  }
  return cmdStr;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Get textual name for given context tag
 *
 ******************************************************************************
 */

static const char* hab_getContextStr(uint8 ctxTag)
{
  char* ctxStr;
  switch(ctxTag)
  {
  case HAB_CTX_ASSERT:
    ctxStr = "ASSERT";      
    break;
  case HAB_CTX_AUTH:
    ctxStr = "AUTH";
    break;
  case HAB_CTX_AUTH_DATA:
    ctxStr = "AUTH_DATA";
    break;      
  case HAB_CTX_CMD:
    ctxStr = "COMMAND";
    break;
  case HAB_CTX_CSF:
    ctxStr = "CSF";
    break;
  case HAB_CTX_DCD:
    ctxStr = "DCD";
    break;
  case HAB_CTX_ENTRY:
    ctxStr = "ENTRY";
    break;
  case HAB_CTX_EXIT:
    ctxStr = "EXIT";
    break;
  case HAB_CTX_TARGET:
    ctxStr = "TARGET";
    break;
  default:
    ctxStr = "Unknown!";
    break;
  }
  return ctxStr;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Get textual name for given engine tag
 *
 ******************************************************************************
 */

static const char* hab_getEngineStr(uint8 engineTag)
{
  char* engineStr;
  switch(engineTag)
  {
  case HAB_ENG_ANY:
    engineStr = "ANY";
    break;
  case HAB_ENG_SCC:
    engineStr = "SCC";
    break;
  case HAB_ENG_RTIC:
    engineStr = "RTIC";
    break;
  case HAB_ENG_SAHARA:
    engineStr = "SAHARA";
    break;
  case HAB_ENG_CSU:
    engineStr = "CSU";
    break;
  case HAB_ENG_SRTC:
    engineStr = "SRTC";
    break;
  case HAB_ENG_DCP:
    engineStr = "DCP";
    break;
  case HAB_ENG_CAAM:
    engineStr = "CAAM";
    break;
  case HAB_ENG_SNVS:
    engineStr = "SNVS";
    break;
  case HAB_ENG_OCOTP:
    engineStr = "OTOCP";
    break;
  case HAB_ENG_DTCP:
    engineStr = "DTCP";
    break;
  case HAB_ENG_ROM:
    engineStr = "ROM";
    break;
  case HAB_ENG_HDCP:
    engineStr = "HDCP";
    break;
  case HAB_ENG_SW:
    engineStr = "SW";
    break;
  default:
    engineStr = "Unknown!";
    break;
  }
  return engineStr;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Get textual name for given protocol tag
 *
 ******************************************************************************
 */

static const char* hab_getProtocolStr(uint8 protocolTag)
{
  char* protocolStr;
  switch(protocolTag)
  {
  case HAB_PCL_SRK:
    protocolStr = "SRK";
    break;
  case HAB_PCL_X509:
    protocolStr = "X509";
    break;
  case HAB_PCL_CMS:
    protocolStr = "CMS";
    break;
  case HAB_PCL_BLOB:
    protocolStr = "BLOB";
    break;
  case HAB_PCL_AEAD:
    protocolStr = "AEAD";
    break;
  default:
    protocolStr = "Unknown!";
    break;
  }
  return protocolStr;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Get textual name for given algorithm tag
 *
 ******************************************************************************
 */

char* hab_getAlgorithmStr(uint8 algorithmTag)
{
  char* algorithmStr;
  switch(algorithmTag)
  {
  case HAB_ALG_ANY:
    algorithmStr = "ANY";
    break;
  case HAB_ALG_HASH:
    algorithmStr = "HASH";
    break;
  case HAB_ALG_SIG:
    algorithmStr = "SIG";
    break;
  case HAB_ALG_FF:
    algorithmStr = "FF";
    break;
  case HAB_ALG_EC:
    algorithmStr = "EC";
    break;
  case HAB_ALG_CIPHER:
    algorithmStr = "CIPHER";
    break;
  case HAB_ALG_MODE:
    algorithmStr = "MODE";
    break;
  case HAB_ALG_WRAP:
    algorithmStr = "WRAP";
    break;
  case HAB_ALG_SHA1:
    algorithmStr = "SHA1";
    break;
  case HAB_ALG_SHA256:
    algorithmStr = "SHA256";
    break;
  case HAB_ALG_SHA512:
    algorithmStr = "SHA512";
    break;
  case HAB_ALG_PKCS1:
    algorithmStr = "PKCS1";
    break;
  case HAB_ALG_AES:
    algorithmStr = "AES";
    break;
  case HAB_ALG_CCM_MODE:
    algorithmStr = "CCM_MODE";
    break;
  case HAB_ALG_BLOB:
    algorithmStr = "BLOB";
    break;
  default:
    algorithmStr = "Unknown!";
    break;
  }
  return algorithmStr;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Print the content of the command description for
 *   InstallKey command to the console
 *
 ******************************************************************************
 */

void hab_dumpInsKeyCmd(const uint8* cmdData, uint32 bytes)
{
  const T_HAB_CMD_INS_KEY* cmd = (void*)cmdData;
  char flags[] = "                ";
  uint32 i;

  for(i = 0; i < 8; i++)
  {
    if(cmd->hdr.par & (0x01 << i))
    {
      flags[2 * i] = '1';
    }
    else      
    {
      flags[2 * i] = '0';
    }
  }
  TRACE_INFO("\n    Flags:   %s", flags);
  TRACE_INFO("\n             H C M F C D C A");
  TRACE_INFO("\n             S I I I F A S B");
  TRACE_INFO("\n             H D D D G T F S");

  TRACE_INFO("\n    Proto:   0x%02X -> %s", cmd->pcl, hab_getProtocolStr(cmd->pcl));
  TRACE_INFO("\n    Algo:    0x%02X -> %s", cmd->alg, hab_getAlgorithmStr(cmd->alg));
  TRACE_INFO("\n    Src:     0x%02X", cmd->src);
  TRACE_INFO("\n    Trg:     0x%02X", cmd->trg);
  TRACE_INFO("\n    keyAddr: 0x%04X", be32ToCpu(cmd->keyAddr));
  #if 0
  if(HAB_CMD_INS_KEY== cmd->hdr.par)
  {
    TRACE_INFO("\n    hashVal:");
    for(i = 0; i < numBytes - sizeof(T_HAB_CMD_INS_KEY); i++)
    {
      TRACE_INFO("\n     0x%02X 0x%02X 0x%02X 0x%02X",
        (uint8_t)(cmd->crtHash[i] >> 24),
        (uint8_t)(cmd->crtHash[i] >> 16),
        (uint8_t)(cmd->crtHash[i] >>  8),
        (uint8_t)(cmd->crtHash[i] >>  0));
    }
  }
  #endif
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Print the content of the command description for
     AuthenticateData command to the console
 *
 ******************************************************************************
 */

void hab_dumpAuthDataCmd(const uint8* cmdData, uint8 bytes)
{
  const T_HAB_CMD_AUTH_DATA* cmd = (void*)cmdData;
  uint32 numBlocks = (bytes - sizeof(T_HAB_CMD_AUTH_DATA)) / sizeof(T_HAB_BLK_DEF);
  uint32 i;

  TRACE_INFO("\n    Key:     0x%02X", cmd->key);
  TRACE_INFO("\n    Proto:   0x%02X -> %s", cmd->pcl, hab_getProtocolStr(cmd->pcl));
  TRACE_INFO("\n    Engine:  0x%02X -> %s", cmd->eng, hab_getEngineStr(cmd->eng));
  TRACE_INFO("\n    Config:  0x%02X", cmd->cfg);
  TRACE_INFO("\n    crtAddr: 0x%08X", be32ToCpu(cmd->crtAddr));

  for(i = 0; i < numBlocks; i++)
  {
    TRACE_INFO("\n    blkAddr: 0x%08X", be32ToCpu(cmd->blk[i].blkAddr));
    TRACE_INFO("\n    blkSize: 0x%08X (%d Bytes)", be32ToCpu(cmd->blk[i].blkSize), be32ToCpu(cmd->blk[i].blkSize));
  }
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Print the content of command description to the console
 *
 ******************************************************************************
 */

void hab_dumpCmdContent(const uint8* cmdData)
{
  const T_HAB_HDR* cmdHdr = (const void*)cmdData;
  uint8  tag = cmdHdr->tag;
  uint8  par = cmdHdr->par;
  uint16 len = HAB_GET_STRUCT_SIZE(*cmdHdr);

  switch(tag)
  {
  case HAB_CMD_WRT_DATA:
    TRACE_INFO("\n  Tag: 0x%02X -> %s", tag, hab_getCmdStr(tag));
    TRACE_INFO("\n  Len: 0x%04X -> %d Bytes", len, len);
    TRACE_INFO("\n  Par: 0x%02X", par);
    break;
  case HAB_CMD_CHK_DATA:
    TRACE_INFO("\n  Tag: 0x%02X -> %s", tag, hab_getCmdStr(tag));
    TRACE_INFO("\n  Len: 0x%04X -> %d Bytes", len, len);
    TRACE_INFO("\n  Par: 0x%02X", par);
    break;
  case HAB_CMD_NOP:
    TRACE_INFO("\n  Tag: 0x%02X -> %s", tag, hab_getCmdStr(tag));
    TRACE_INFO("\n  Len: 0x%04X -> %d Bytes", len, len);
    TRACE_INFO("\n  Par: 0x%02X", par);
    break;
  case HAB_CMD_SET_ITEM:
    TRACE_INFO("\n  Tag: 0x%02X -> %s", tag, hab_getCmdStr(tag));
    TRACE_INFO("\n  Len: 0x%04X -> %d Bytes", len, len);
    TRACE_INFO("\n  Par: 0x%02X", par);
    break;
  case HAB_CMD_INIT:
    TRACE_INFO("\n  Tag: 0x%02X -> %s", tag, hab_getCmdStr(tag));
    TRACE_INFO("\n  Len: 0x%04X -> %d Bytes", len, len);
    TRACE_INFO("\n  Par: 0x%02X", par);
    break;
  case HAB_CMD_UNLOCK:
    TRACE_INFO("\n  Tag: 0x%02X -> %s", tag, hab_getCmdStr(tag));
    TRACE_INFO("\n  Len: 0x%04X -> %d Bytes", len, len);
    TRACE_INFO("\n  Par: 0x%02X", par);
    break;
  case HAB_CMD_INS_KEY:
    TRACE_INFO("\n  Tag: 0x%02X -> %s", tag, hab_getCmdStr(tag));
    TRACE_INFO("\n  Len: 0x%04X -> %d Bytes", len, len);
    TRACE_INFO("\n  Par: 0x%02X -> flags", par);

    hab_dumpInsKeyCmd(cmdData, len);
    break;
  case HAB_CMD_AUTH_DATA:
    TRACE_INFO("\n  Tag: 0x%02X -> %s", tag, hab_getCmdStr(tag));
    TRACE_INFO("\n  Len: 0x%04X -> %d Bytes", len, len);
    TRACE_INFO("\n  Par: 0x%02X", par);

    hab_dumpAuthDataCmd(cmdData, len);
    break;
  default:
    break;
  }
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Print the content of the image vector table to the console
 *
 ******************************************************************************
 */

void hab_dumpIvtContent(const uint8* ivtData)
{
  const T_HAB_IVT* ivt = (const void*)ivtData;
  uint8  tag = ivt->hdr.tag;
  uint8  par = ivt->hdr.par;
  uint16 len = HAB_GET_STRUCT_SIZE(ivt->hdr);
  
  TRACE_INFO("\n  Tag: 0x%02X -> %s", tag, hab_getTagStr(tag));
  TRACE_INFO("\n  Len: 0x%04X -> %d Bytes", len, len);
  TRACE_INFO("\n  Par: 0x%02X -> HABVer: %d.%d", par, (par >> 4), (par & 0xF));

  TRACE_INFO("\n    Entry:     0x%08X", (uint32)ivt->entry);
  TRACE_INFO("\n    DCD:       0x%08X", (uint32)ivt->dcd);
  TRACE_INFO("\n    Boot Data: 0x%08X", (uint32)ivt->boot_data);
  if(ivt->boot_data != NULL)
  {
    const T_HAB_BOOT_DATA* hab_boot = ivt->boot_data;
    
    TRACE_INFO("\n      ImgStart:  0x%08X", (uint32)hab_boot->start);
    TRACE_INFO("\n      ImgSize:   0x%08X (%d Bytes)", hab_boot->size, hab_boot->size);
    TRACE_INFO("\n      Plugin:    0x%08X", hab_boot->plugin);
  }
  TRACE_INFO("\n    Self:      0x%08X", (uint32)ivt->self);
  TRACE_INFO("\n    CSF:       0x%08X\n", (uint32)ivt->csf);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void hab_dumpCsfContent(const void* csfAddr)
{
  const T_HAB_CSF* csf = csfAddr;
//  const T_HAB_HDR* csfHdr = &csf->hdr;
  const uint8* cmdData = csfAddr;
  uint16 len = sizeof(T_HAB_HDR);
  uint16 bytesRemaining = HAB_GET_STRUCT_SIZE(csf->hdr);
  uint32 i;

  cmdData += len;
  bytesRemaining -= len;
  for(i = 0; bytesRemaining > 0; i++)
  {
    const T_HAB_HDR* cmdHdr;
    TRACE_INFO("\n---- CSF Command #%d ----", i);
    cmdHdr = (void*)cmdData;
    len = HAB_GET_STRUCT_SIZE(*cmdHdr);
  
    hab_dumpCmdContent(cmdData);
    TRACE_INFO("\n");
    
    cmdData += len;
    bytesRemaining -= len;
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

uint32 hab_dumpLogEvtContent(uint8* evtData)
{
  T_HAB_EVT* logEvt = (void*)evtData;
  uint8  tag = logEvt->hdr.tag;
  uint8  par = logEvt->hdr.par;
  uint16 len = HAB_GET_STRUCT_SIZE(logEvt->hdr);

  if(tag == HAB_TAG_EVT)
  {
    TRACE_INFO("\n  Tag: 0x%02X -> %s", tag, hab_getTagStr(tag));
    TRACE_INFO("\n  Len: 0x%04X -> %d Bytes", len, len);
    TRACE_INFO("\n  Par: 0x%02X -> HAB Ver: %d.%d", par, (par >> 4), (par & 0xF));

    TRACE_INFO("\n    Evt Status:  0x%02X -> %s", logEvt->sts, hab_getStatusStr(logEvt->sts));
    TRACE_INFO("\n    Evt Reason:  0x%02X -> %s", logEvt->rsn, hab_getReasonStr(logEvt->rsn));
    TRACE_INFO("\n    Evt Context: 0x%02X -> %s", logEvt->ctx, hab_getContextStr(logEvt->ctx));
    TRACE_INFO("\n    Evt Engine:  0x%02X -> %s", logEvt->eng, hab_getEngineStr(logEvt->eng));
    
    if( (logEvt->ctx == HAB_CTX_ASSERT)
      | (logEvt->ctx == HAB_CTX_TARGET) )
    {
      uint32 type;
      uint32 addr;
      uint32 size;

      type = BYTES_TO_UINT32(logEvt->data[0], logEvt->data[1], logEvt->data[2], logEvt->data[3]);
      addr = BYTES_TO_UINT32(logEvt->data[4], logEvt->data[5], logEvt->data[6], logEvt->data[7]);
      size = BYTES_TO_UINT32(logEvt->data[8], logEvt->data[9], logEvt->data[10], logEvt->data[11]);
      TRACE_INFO("\n      TYPE=%08X", type);
      TRACE_INFO("\n      ADDR=%08X", addr);
      TRACE_INFO("\n      SIZE=%08X (%d)", size, size);
    }

    if(logEvt->ctx == HAB_CTX_CMD)
    {
      hab_dumpCmdContent(logEvt->data);
    }
  }
  return len;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Print the log event to the console
 *
 ******************************************************************************
 */

void hab_dumpLogEvent(uint8* evtData, uint32 bytes)
{
  uint32 i;
  if(!evtData)
  {
    TRACE_INFO("\n No data!");
  }
  else if(bytes <= 0)
  {
    TRACE_INFO("\n Too few data!");
  }
  else
  {
    for(i = 0; i < bytes; i++)
    {
      if( (i % 4) == 0)
      {
        TRACE_INFO("\n ");
      }
      TRACE_INFO(" 0x%02X", evtData[i]);
    }
    hab_dumpLogEvtContent(evtData);
  }
}



/*
 ******************************************************************************
 *
 ******************************************************************************
 * @param Print the CPU's HAB version to the console
 *
 ******************************************************************************
 */

void hab_dumpVersion(void)
{
  const T_HAB_RVT* rvt = hab_getRvt();
  const T_HAB_HDR* rvtHdr = &rvt->hdr;
  uint16 rvtSize = HAB_GET_STRUCT_SIZE(rvt->hdr);
  uint32_t habVer;

  if(HAB_TAG_RVT != rvtHdr->tag)
  {
    /* Invalid RVT */
    habVer = 0;
  }
  else if(rvtSize > (uint32)(void*)&(((T_HAB_RVT*)(void*)0)->get_version))
  {
    /* get_version() is is available */
    habVer = rvt->get_version();
  }
  else
  {
    /* get_version() is not available */
    habVer = (0
     | (HAB_GET_MAJOR_VERSION(rvtHdr->par) << 16)
     | (HAB_GET_MINOR_VERSION(rvtHdr->par) <<  8)
     );
  }

  TRACE_INFO("HAB Version: %d.%d.%d\n",
    (uint8_t)(habVer >> 16),  /* Major */
    (uint8_t)(habVer >>  8),  /* Minor */
    (uint8_t)(habVer >>  0)); /* Patch */
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @param Print the image vector table to the console
 *
 ******************************************************************************
 */

void hab_dumpIvt(const void* ivtAddr)
{
  const T_HAB_IVT* ivt;
  const T_HAB_HDR* ivtHdr = NULL;
  uint16 ivtSize = 0;

  if(NULL == ivtAddr)
  {
     /* Invalid IVT address given */
  }
  else
  {
     ivt = ivtAddr;
     ivtHdr = &ivt->hdr;
     ivtSize = HAB_GET_STRUCT_SIZE(ivt->hdr);
  }

  if(NULL == ivtHdr)
  {
     /* Invalid IVT address */
  }
  else if(HAB_TAG_IVT != ivtHdr->tag)
  {
     /* Invalid tag */
  }
  else if(ivtSize != sizeof(T_HAB_IVT))
  {
     /* Invalid IVT size */
  }
  else
  {
    const uint8* ivtData = ivtAddr;
    uint32 i;

    TRACE_INFO("Image Vector Table (IVT) at Addr: 0x%08X", (uint32)ivtAddr);
    for(i = 0; i < ivtSize; i++)
    {
      if( (i % 4) == 0)
      {
        TRACE_INFO("\n");
      }
      TRACE_INFO(" 0x%02X", ivtData[i]);
    }
    hab_dumpIvtContent(ivtData);
  }
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @param Print the ROM vector table to the console
 *
 ******************************************************************************
 */

void hab_dumpRvt(void)
{
  const T_HAB_RVT* rvt = hab_getRvt();
  const T_HAB_HDR* rvtHdr = &rvt->hdr;
  uint8* rvtData = (void*)rvt;
  
  if(HAB_TAG_RVT == rvtHdr->tag)
  {
    uint32 i;
    uint16 rvtSize = HAB_GET_STRUCT_SIZE(rvt->hdr);
    
    TRACE_INFO("ROM Vector Table (RVT) at Addr: 0x%08X", (uint32)(void*)rvt);
    for(i = 0; i < rvtSize; i++)
    {
      if( (i % 4) == 0)
      {
        TRACE_INFO("\n ");
      }
      TRACE_INFO(" 0x%02X", rvtData[i]);
    }
    TRACE_INFO("\n");
//    hab_dumpRvtContent();
  }
}


void hab_dumpCsf(const void* csfAddr)
{
  const T_HAB_CSF* csf = csfAddr;
  const T_HAB_HDR* csfHdr = &csf->hdr;
  const uint8* csfData = csfAddr;
  uint8 tag = csfHdr->tag;

  TRACE_INFO("Command Sequence File (CSF) at Addr: %08X", (uint32)csfAddr);
  if(tag == HAB_TAG_CSF)
  {
    uint8  par = csfHdr->par;
    uint16 len = HAB_GET_STRUCT_SIZE(csf->hdr);

    TRACE_INFO("\n  Tag: 0x%02X -> %s", tag, hab_getTagStr(tag));
    TRACE_INFO("\n  Len: 0x%04X -> %d Bytes", len, len);
    TRACE_INFO("\n  Par: 0x%02X -> HABVer: %d.%d\n", par, (par >> 4), (par & 0xF));
    hab_dumpCsfContent(csfData);
  }
  else
  {
    TRACE_INFO("\n Invalid Header!\n");
  }
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @param Print the status of the HAB boot to the console
 *
 ******************************************************************************
 */

void hab_dumpStatus(void)
{
  const T_HAB_RVT* rvt = hab_getRvt();
  uint32 i = 0;
  uint8  evtData[128];
  uint32 bytes = sizeof(evtData);

  T_HAB_STATUS result;
  T_HAB_CFG config = 0;
  T_HAB_STATE state = 0;

  libc_memset(evtData, 0, 128);
  result = rvt->reportStatus(&config, &state);

  TRACE_INFO("\nHAB Configuration: 0x%02x ", config);
  switch(config)
  {
  case HAB_CFG_FAB:
    TRACE_INFO("(fab) ");
    break;
  case HAB_CFG_RETURN:
    TRACE_INFO("(return) ");
    break;
  case HAB_CFG_OPEN:
    TRACE_INFO("(open) ");
    break;
  case HAB_CFG_CLOSED:
    TRACE_INFO("(closed) ");
    break;
  default:
    TRACE_INFO("(invalid) ");
    break;
  }
  
  TRACE_INFO("HAB State: 0x%02x ", state);    
  switch(state)
  {
  case HAB_STATE_INITIAL:
    TRACE_INFO("(initial)");
    break;
  case HAB_STATE_CHECK:
    TRACE_INFO("(check)");
    break;
  case HAB_STATE_NONSECURE:
    TRACE_INFO("(non-secure)");
    break;
  case HAB_STATE_TRUSTED:
    TRACE_INFO("(trusted)");
    break;
  case HAB_STATE_SECURE:
    TRACE_INFO("(secure)");
    break;
  case HAB_STATE_FAIL_SOFT:
    TRACE_INFO("(fail_soft)");
    break;
  case HAB_STATE_FAIL_HARD:
    TRACE_INFO("(fail_hard)");
    break;
  case HAB_STATE_NONE:
    TRACE_INFO("(none)");
    break;
  default:
    TRACE_INFO("(invalid)");
    break;
  }
  TRACE_INFO("\n");
  
  if( HAB_STATUS_SUCCESS !=  result)
  {
    while(1)
    {
      result = rvt->reportEvent(HAB_STATUS_ANY, i, evtData, &bytes);
      if(HAB_STATUS_SUCCESS == result)
      {
        TRACE_INFO("\n");
        TRACE_INFO("----- HAB Event %d -----\n", i + 1);
        TRACE_INFO("Event Data:");
        hab_dumpLogEvent(evtData, bytes);
        TRACE_INFO("\n");
        bytes = sizeof(evtData);
        i++;
      }
      else if(HAB_STATUS_FAIL == result)
      {
        TRACE_INFO("\n");
        TRACE_INFO("No more HAB Events found!\n");
        break;
      }
      else
      {
        TRACE_INFO("\n");
        TRACE_INFO("Invalid Status\n");
        break;
      }
    }
  }
  else
  {
    TRACE_INFO("No HAB Events Found!\n\n");
  }
}



