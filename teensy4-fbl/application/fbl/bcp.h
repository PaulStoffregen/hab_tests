#ifndef BCP_H
#define BCP_H

#define BCP_TX_PENDING     1
#define BCP_RX_PENDING     1
#define BCP_OK             0
#define BCP_ERROR_UNKNOWN -1
#define BCP_ERROR_RX_FAIL -2

typedef enum BCP_STATUS
{
  BCP_STATUS_eUNINIT = 0,
  BCP_STATUS_eIDLE,
  BCP_STATUS_eBUSY,
}T_BCP_STATUS;

typedef enum BCP_JOB_RESULT
{
  BCP_JOB_RESULT_eOK = 0,
  BCP_JOB_RESULT_eFAILED,
  BCP_JOB_RESULT_eCANCELLED,
  BCP_JOB_RESULT_ePENDING,
}T_BCP_JOB_RESULT;

void bcp_init(void);
void bcp_run(void);

void bcp_cancel(void);
T_STATUS bcp_listen(void);
T_STATUS bcp_sendMsg(T_PDU* txMsg);

T_BCP_STATUS bcp_getStatus(void);
T_BCP_JOB_RESULT bcp_getJobResult(void);

T_STATUS bcp_getRxMsg(T_PDU* rxMsg);
T_STATUS bcp_getTxPdu(T_PDU* txMsg);

void bcp_sendAckRsp(uint32 cmd);
void bcp_sendNakRsp(uint32 cmd, uint32 errorCode);

#endif /* BCP_H */
