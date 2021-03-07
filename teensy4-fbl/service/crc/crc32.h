#ifndef CRC32_H
#define CRC32_H

typedef struct T_CRC32_DATA
{
  const uint32* lut;
  uint32 crc;
}T_CRC32_DATA;

void crc32_configCtx(T_CRC32_DATA* ctx, const uint32* crc_lookUpTbl);
void crc32_preset(T_CRC32_DATA* ctx, uint32 preset);
void crc32_finalize(T_CRC32_DATA* ctx, uint32 finit);
uint32 crc32_read(T_CRC32_DATA* ctx);
void crc32_writeLE(T_CRC32_DATA* ctx, uint8 dstBuf[4]);
void crc32_writeBE(T_CRC32_DATA* ctx, uint8 dstBuf[4]);
void crc32_updateNTabFwd(T_CRC32_DATA* ctx, const uint8* srcBuf, uint16 numBytes);
void crc32_updateNTabRev(T_CRC32_DATA* ctx, const uint8* srcBuf, uint16 numBytes);
void crc32_udaeteSTabFwd(T_CRC32_DATA* ctx, const uint8* srcBuf, uint16 numBytes);
void crc32_updateSTabRev(T_CRC32_DATA* ctx, const uint8* srcBuf, uint16 numBytes);
void crc32_updateFTabFwd(T_CRC32_DATA* ctx, const uint8* srcBuf, uint16 numBytes);
void crc32_updateFTabRev(T_CRC32_DATA* ctx, const uint8* srcBuf, uint16 numBytes);


#include "crc32_cfg.h"

#endif /* CRC32_H */

