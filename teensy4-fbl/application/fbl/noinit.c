#ifndef NOINIT_C
#define NOINIT_C
#endif /* NOINIT_C */

#include "bsp.h"

#include "noinit.h"


#define __noinitSect __attribute__((section(".noInitData")))

__noinitSect static T_NOINIT_DATA noinit_data;


T_NOINIT_DATA* noinit_getData(void)
{
   return &noinit_data;
}


