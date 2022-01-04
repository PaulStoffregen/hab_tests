#ifndef FBL_DEFS_H
#define FBL_DEFS_H


#define FBL_ALIGN_SIZE 0x400
#define FBL_BLK_SIZE   0x400

#define FBL_BOOTSTRAP_TOKEN  {'B', 'O', 'O', 'T'}

//#define FBL_BOOTSTRAP_TOKEN (('B' << 24) | ('O' << 16) | ('O' << 8) | ('T' << 0))

enum FBL_MSG_ID
{
  FBL_MSG_ID_eINVALID = 0xE0,
  FBL_MSG_ID_eBOOTSTRAP_REQ = 0xE1,
  FBL_MSG_ID_eSWINFO_REQ,
  FBL_MSG_eSEED_REQ,
  FBL_MSG_eKEY_REQ,
  FBL_MSG_ID_eINVALIDATE_REQ,
  FBL_MSG_ID_eERASE_REQ,
  FBL_MSG_ID_ePROGRAM_REQ,
  FBL_MSG_ID_eACTIVATE_REQ,
  FBL_MSG_ID_eRESET_REQ,
  FBL_MSG_ID_eALIVE_REQ,
  FBL_MSG_ID_eACK_RSP,
  FBL_MSG_ID_eNAK_RSP,
  FBL_MSG_ID_eSWINFO_RSP,
};

enum BCP_ERR_ID
{
  BCP_ERR_ID_eNONE = 0,
  BCP_ERR_ID_eINVALID_CRC,
  BCP_ERR_ID_eINVALID_SIZE,
  BCP_ERR_ID_eINVALID_TYPE,
  BCP_ERR_ID_eINVALID_DATA,
  BCP_ERR_ID_eINCONSISTENT,
  BCP_ERR_ID_eUNEXPECTED,
  BCP_ERR_ID_eNUM_ERR_IDs,
};

#if defined(BCP_C)
const char* bcp_errStr[BCP_ERR_ID_eNUM_ERR_IDs] =
{
  [BCP_ERR_ID_eNONE] = "NONE",
  [BCP_ERR_ID_eINVALID_CRC] = "Invalid CRC",
  [BCP_ERR_ID_eINVALID_SIZE] = "Invalid size",
  [BCP_ERR_ID_eINVALID_TYPE] = "Invalid message type",
  [BCP_ERR_ID_eINVALID_DATA] = "Invalid data",
  [BCP_ERR_ID_eINCONSISTENT] = "Inconsistent",
  [BCP_ERR_ID_eUNEXPECTED] = "Unexpected message",
};
#else
const char* bcp_errStr[BCP_ERR_ID_eNUM_ERR_IDs];
#endif
   

#define FBL_MSG__CRC_SIZE                   (2U)
#define FBL_MSG__CHUNK_SIZE                 (FBL_PROG_BLOCK_SZ)

#define FBL_MSG__PAYLOAD_SIZE(msgLen)       ((msgLen) - FBL_MSG__OPC_SIZE - FBL_MSG__CRC_SIZE])


#define FBL_MSG__CHUNK_OFFS                 (5U)
#define FBL_MSG__CRC_OFFS(msgLen)           ((msgLen) - FBL_MSG__CRC_SIZE)

#define FBL_MSG__GET_CRC_ADDR(msg, msgLen)  (&(msg)[(msgLen) - FBL_MSG__OPC_SIZE - FBL_MSG__CRC_SIZE])


#define FBL_MSG__GET_CHUNK(msg)        (&(msg)[FBL_MSG__CHUNK_OFFS])




#define FBL_MSG__OPC_SIZE                   (1U)
#define FBL_MSG__TOKEN_SIZE                 (4U)
#define FBL_MSG__CRC_SIZE                   (2U)

#define FBL_MSG__OPC_OFFS                   (0U)
#define FBL_MSG__TOKEN_OFFS                 (1U)
#define FBL_MSG__ADDR_OFFS                  (1U)
#define FBL_MSG__LEN_OFFS                   (5U)
#define FBL_MSG__DATA_OFFS                  (9U)

#define FBL_MSG__GET_OPC(msg)               ((msg)[FBL_MSG__OPC_OFFS])

#define FBL_MSG__SET_OPC(msg, opc)    \
  do                                  \
  {                                   \
    ((msg)[FBL_MSG__OPC_OFFS]) = opc; \
  }while(0)

#define FBL_MSG__SET_TOKEN(msg, token) \
  do                                   \
  {                                    \
    memcpy(                       \
      &(msg)[FBL_MSG__TOKEN_OFFS],     \
      token,                           \
      FBL_MSG__TOKEN_SIZE);            \
  }while(0)

#define FBL_MSG__GET_TOKEN(msg)       \
    (&(msg)[FBL_MSG__TOKEN_OFFS])     

#define FBL_MSG__ADDR(msg) ((msg)[FBL_MSG__ADDR_OFFS])


typedef struct
{
  uint32 msgType;
  uint32 bootMagic;
}T_FBL_MSG_ENTRY_REQ;


typedef struct
{
  uint32 msgType;
}T_FBL_MSG_SWINFO_REQ;


typedef struct
{
  uint32   msgType;
  uint32   tag;
  uint8    fblInfo[12];
  uint8    fblVerLen[2];
  uint8    fblVerInfo[64];
  uint8    appInfo[12];
  uint8    appVerLen[2];
  uint8    appVerInfo[64];
}T_FBL_MSG_SWINFO_RSP;


typedef struct
{
   uint32 msgType;
//   uint8  seedData[FBL_BLK_SIZE];
}T_FBL_MSG_SEED_REQ;


typedef struct
{
   uint32 msgType;
//   uint8  keyData[FBL_BLK_SIZE];
}T_FBL_MSG_KEY_REQ;


typedef struct
{
  uint32 msgType;
  uint32 blkAddr;
  uint32 blkSize;
}T_FBL_MSG_INVALIDATE_REQ;


typedef struct
{
  uint32 msgType;
  uint32 blkAddr;
  uint32 blkSize;
}T_FBL_MSG_ERASE_REQ;


typedef struct
{
   uint32 msgType;
   uint32 blkAddr;
   uint8  blkData[FBL_BLK_SIZE];
}T_FBL_MSG_PROGRAM_REQ;


typedef struct
{
  uint32 msgType;
  uint32 blkAddr;
  uint32 blkSize;
}T_FBL_MSG_ACTIVATE_REQ;


typedef struct
{
  uint32 msgType;
}T_FBL_MSG_RESET_REQ;


typedef struct
{
  uint32 msgType;
  uint32 reqType;
}T_FBL_MSG_ACK_RSP;


typedef struct
{
  uint32 msgType;
  uint32 reqType;
  uint32 errCode;
}T_FBL_MSG_NAK_RSP;


typedef struct
{
  uint32 msgType;
  uint8  msgData[0];
}T_BCP_MSG;



#endif /* FBL_DEFS_H */

