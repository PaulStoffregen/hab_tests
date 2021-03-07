#ifndef FBL_CTL_H
#define FBL_CTL_H

/* Callback function prototype */
typedef void (*T_FBLCTL_CBK)(void);

extern void fblctl_init(void);
extern void fblctl_run(void);
extern void fblctl_registerCallback(void (*cbk)(uint32));

extern T_STATUS fblctl_sendBootstrapReq(void);
extern T_STATUS fblctl_sendSwInfoReq(void);
extern T_STATUS fblctl_sendInvalidateReq(uint32 dstAddr, uint32 len);
extern T_STATUS fblctl_sendEraseReq(uint32 dstAddr, uint32 len);
extern T_STATUS fblctl_sendProgramReq(uint32 dstAddr, uint32 (*readSrcCbk)(uint8* msgBuf, uint32 bufSize, uint32 dstAddr));
extern T_STATUS fblctl_sendActivateReq(uint32 dstAddr, uint32 len);
extern T_STATUS fblctl_sendRestartReq(void);


void fblctl_injectError(uint32 err);

#endif /* FBL_CTL_H */
