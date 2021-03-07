#ifndef IVT_C
#define IVT_C
#endif /* IVT_C */

#include "bsp.h"
#include "reg.h"
#include "hab.h"



extern void       _start(void);
extern unsigned* __image_start;
extern unsigned* __image_size;
extern unsigned* __boot_data_start;
extern unsigned* __dcd_start;
extern unsigned* __dcd_end;
extern unsigned* __hab_data;

__csf_sect T_HAB_CSF csf;

__ivt_sect T_HAB_IVT ivt =
{
  /* Header word with tag = HAB_TAG_IVT, length and HAB version fields */
  HAB_IVT_HDR_DECL(sizeof(T_HAB_IVT), HAB_VERSION),
  /* Absolute address of the first instruction to execute from the image */
  (T_HAB_ENTRY)_start,
  /* Reserved in this version of HAB: should be NULL. */
  (const void*)NULL,
  /* Absolute address of the image DCD: may be NULL. */
  (const void*)&__dcd_start,
  /* Absolute address of the Boot Data.
   * May be NULL, but not interpreted any further by HAB
   */
  (const void*)&__boot_data_start,
  /* Absolute address of the IVT.*/
  (const void*)(&ivt),
  
  /* Absolute address of the image Command Sequence File (CSF): may be NULL. */
#if 1 // TODO: Make this dependent on whether HAB is used or not.
  (const void*)&csf,
#else
  (const void*)NULL,
#endif
  /* Reserved in this version of HAB: should be zero. */
  0
};

__boot_data_sect T_HAB_BOOT_DATA boot_data =
{
  (uint32)&__image_start,
  (uint32)&__image_size,
  0,
  0,
};

