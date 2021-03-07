#ifndef HAB_INFO_H
#define HAB_INFO_H


void hab_dumpInsKeyCmd(const uint8* cmdData, uint32 bytes);
void hab_dumpAuthDataCmd(const uint8* cmdData, uint8 bytes);
void hab_dumpCmdContent(const uint8* cmdData);

void hab_dumpIvtContent(const uint8* ivtData);

uint32 hab_dumpLogEvtContent(uint8* evtData);
void hab_dumpLogEvent(uint8* evtData, uint32 bytes);


void hab_dumpRvt(void);
void hab_dumpIvt(const void* ivtAddr);
void hab_dumpCsf(const void* csfAddr);

void hab_dumpVersion(void);
void hab_dumpStatus(void);

#endif /* HAB_INFO_H */

