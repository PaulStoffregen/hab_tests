#ifndef ROM_API_H
#define ROM_API_H

#include "boot_api.h"
#include "norflash_api.h"
#include "clk_api.h"
#include "rtwdg_api.h"
#include "wdog_api.h"


typedef struct
{
  const uint32 version;
  const char* copyright;
  const T_BOOT_API bootApi;
  const uint32* reserved0;
  const T_NORFLASH_API* norFlashApi;
  const uint32* reserved1;
  const T_CLK_API* clkApi;
  const T_RTWDG_API* rtwdgApi;
  const T_WDOG_API* wdogApi;
  const uint32* reserved2;
}T_ROM_API;


T_ROM_API* romApi_getAddr(void);

#endif /* ROM_API_H */

