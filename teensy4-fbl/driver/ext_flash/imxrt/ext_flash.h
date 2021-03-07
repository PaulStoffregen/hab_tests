#ifndef EXT_FLASH_H
#define EXT_FLASH_H


extern void extflash_init(void);
extern T_STATUS extflash_write(uint32 dstLogAddr, uint8 srcBuf[], sint32 numBytes);
extern T_STATUS extflash_erase(uint32 logAddr, uint32 numBytes);

typedef struct
{
  uint32 sectAddr;
  uint32 sectSize;
}T_SECTOR_INFO;


#include "ext_flash_cfg.h"

#endif /* EXT_FLASH_H */
