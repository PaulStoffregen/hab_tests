#ifndef CRC16_H
#define CRC16_H

typedef struct T_CRC16_DATA
{
  const uint16* lut;
  uint16  crc;
}T_CRC16_DATA;

void crc16_configCtx(T_CRC16_DATA* ctx, const uint16* crc_lookUpTbl);
void crc16_preset(T_CRC16_DATA* ctx, uint16 preset);
void crc16_finalize(T_CRC16_DATA* ctx, uint16 finit);
uint16 crc16_read(T_CRC16_DATA* ctx);
void crc16_write_le(T_CRC16_DATA* ctx, uint8 dstBuf[2]);
void crc16_write_be(T_CRC16_DATA* ctx, uint8 dstBuf[2]);
void crc16_updateNTabFwd(T_CRC16_DATA* ctx, const uint8* srcBuf, uint16 numBytes);
void crc16_updateSTabFwd(T_CRC16_DATA* ctx, const uint8* srcBuf, uint16 numBytes);
void crc16_updateFTabFwd(T_CRC16_DATA* ctx, const uint8* srcBuf, uint16 numBytes);


#include "crc16_cfg.h"

#endif /* CRC16_H */

