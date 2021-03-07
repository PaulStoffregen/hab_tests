#ifndef DLCF_H
#define DLCF_H


#define DLCF_OK              0
#define DLCF_FRAME_PENDING   1
#define DLCF_FRAME_FINISHED  2
#define DLCF_BYTE_RECEIVED   3
#define DLCF_ERROR_INVALID  -1

typedef enum DLCF_STATUS
{
  DLCF_STATUS_eOK = 0,
  DLCF_STATUS_eFRAME_PENDING,
  DLCF_STATUS_eFRAME_FINISHED,
  DLCF_STATUS_eBYTE_RECEIVED,
}T_DLCF_STATUS;


enum
{
  DLCF_CTL_BYTE_ID_eSOF = 0,
  DLCF_CTL_BYTE_ID_eESC,
  DLCF_CTL_BYTE_ID_eEOF,
};

typedef struct
{
  const uint8* ctlBytes;
  const uint8* escBytes; 
  uint8 numCtlEscBytes;
  uint8 dle;
}T_DLCF_CFG;


/* This structure encapsulates the device access functions.
 * There is:
 *  - 1 callback for write/send
 *  - 1 callback for read/receive
 *  - 1 device data pointer that is given as parameter to the callbacks
 *
 * The last member is a pointer to device specific data, which may be a
 * device specific structure or a device ID.
 */

typedef struct
{
  
  boolean (*wrByte)(void*, uint8);
  boolean (*rdByte)(void*, uint8*);
  void* devData;
}T_DLCF_DEV_INFO;


typedef enum DLCF_TX_STATE 
{
  DLCF_TX_STATE_eRESET = 0,
  DLCF_TX_STATE_eCONFIG,
  DLCF_TX_STATE_eIDLE,
  DLCF_TX_STATE_eSOF,
  DLCF_TX_STATE_eDATA,
  DLCF_TX_STATE_eESC,
  DLCF_TX_STATE_eEOF,
  DLCF_TX_STATE_eFINISHED,
}T_DLCF_TX_STATE;

typedef enum DLCF_RX_STATE
{
  DLCF_RX_STATE_eRESET = 0,
  DLCF_RX_STATE_eIDLE,
  DLCF_RX_STATE_eSOF,
  DLCF_RX_STATE_eDATA,
  DLCF_RX_STATE_eESC,
  DLCF_RX_STATE_eFINISHED,
  DLCF_RX_STATE_eERROR,
}T_DLCF_RX_STATE;


typedef struct
{
  T_DLCF_CFG* cfg;
  T_DLCF_TX_STATE txState;
  uint16 txPos;
  uint16 txLen;
  uint8* txData;
  void  (*txCbk)(void);
  T_DLCF_RX_STATE rxState;
  uint16 rxPos;
  T_PDU* rxPdu;
  void  (*rxCbk)(void);
  uint8  ctlID; /* ID of the control character to be escaped */
  const T_DLCF_DEV_INFO* devInfo;
}T_DLCF_CTX;


#if 0
extern T_STATUS dlcf_registerRdByteOp(T_DLCF_CTX* ctx, T_DLCF_RDBYTE_FUNC cbk);
extern T_STATUS dlcf_registerWrByteOp(T_DLCF_CTX* ctx, T_DLCF_WRBYTE_FUNC cbk);
extern T_STATUS dlcf_setDevData(T_DLCF_CTX* ctx, void* devData);
#endif

extern void dlcf_configCtx(T_DLCF_CTX* ctx, T_DLCF_CFG* cfg);
extern T_STATUS dlcf_setDevInfo(T_DLCF_CTX* ctx, const T_DLCF_DEV_INFO* devInfo);

extern T_STATUS dlcf_recvPdu(T_DLCF_CTX* ctx, T_PDU* rxPdu);
extern T_STATUS dlcf_sendPdu(T_DLCF_CTX* ctx, T_PDU* txPdu);

extern T_DLCF_STATUS dlcf_getRxStatus(T_DLCF_CTX* ctx);
extern void dlcf_clrRxStatus(T_DLCF_CTX* ctx);

extern T_DLCF_STATUS dlcf_getTxStatus(T_DLCF_CTX* ctx);
extern void dlcf_clrTxStatus(T_DLCF_CTX* ctx);

extern void dlcf_init(T_DLCF_CTX* ctx);
extern void dlcf_run(T_DLCF_CTX* ctx);

#endif /* DLCF_H */
