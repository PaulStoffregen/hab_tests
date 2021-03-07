#ifndef SWINFO_C
#define SWINFO_C
#endif /* SWINFO_C */

#include "bsp.h"
#include "sw_release.h"
#include "swinfo.h"

#define __swinfo_sect __attribute__((section(".sw_info")))

extern unsigned* __image_size;
extern unsigned* __image_start;

static const __swinfo_sect T_SWINFO swinfo_data =
{
  .imgAddr = (uint32)&__image_start,
  .imgSize = (uint32)&__image_size,
  .crc = SWINFO_DEFAULT_CRC,
  .tag = SWINFO_TAG,
  .versionAddr = (uint32)(void*)&sw_rel_data,
  .versionLen = sizeof(sw_rel_data),
  .reserved = "STARTUP"
};


const T_SWINFO* swinfo_getOwnSwInfo(void)
{
  return &swinfo_data;
}

