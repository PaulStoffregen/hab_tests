#ifndef ROM_API_C
#define ROM_API_C
#endif /* ROM_API_C */

#include "bsp.h"
#include "rom_api.h"


T_ROM_API* romApi_getAddr(void)
{
   return (T_ROM_API*)(void*)(*(uint32*)(void*)ROM_API_BASE);
}


