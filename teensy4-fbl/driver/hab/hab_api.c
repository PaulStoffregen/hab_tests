#ifndef HAB_API_C
#define HAB_API_C
#endif /* HAB_API_C */

#include "bsp.h"
#include "reg.h"
#include "trace_pub.h"
#include "libc.h"
#include "hab.h"
//#include "caam.h"
#include "hab_prv.h"
#include "hab_api.h"


typedef struct
{
  T_HAB_RVT* rvt;
}T_HAB_DATA;


static T_HAB_DATA hab_data =
{
  .rvt = (void*)(HAB_RVT_BASE),
};


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

const T_HAB_RVT* hab_getRvt(void)
{
  return hab_data.rvt;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Get the address of the image's image vector table (IVT) as
 *   defined by the linker
 *
 ******************************************************************************
 */

const void* hab_getOwnIvtAddr(void)
{
  return &ivt;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Get the address of the image's command sequence file (CSF) as
 *   defined by the linker
 *
 ******************************************************************************
 */

const void* hab_getOwnCsfAddr(void)
{
  return &csf;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Get the address of the image's device configuration data (DCD) as
 *   defined by the linker
 *
 ******************************************************************************
 */

const void* hab_getOwnDcdAddr(void)
{
  return __dcd_start;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

uint32 hab_getOwnDcdSize(void)
{
  return __dcd_end - __dcd_start;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Get the address of the image's device configuration data (DCD) from
 *   the image vector table.
 *
 ******************************************************************************
 */

const void* hab_getDcdAddrFromIvt(const void* ivtAddr)
{
  const T_HAB_IVT* hab_ivt = ivtAddr;
  return hab_ivt->dcd;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Get the address of the image's boot data from the image vector table.
 *
 ******************************************************************************
 */

const void* hab_getBootAddrFromIvt(const void* ivtAddr)
{
  const T_HAB_IVT* habIvt = ivtAddr;
  return habIvt->boot_data;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Get the image's entry address from the image vector table.
 *
 ******************************************************************************
 */

const void* hab_getEntryAddrFromIvt(const void* ivtAddr)
{
  const T_HAB_IVT* hab_ivt = ivtAddr;
  return hab_ivt->entry;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Get the image's payload address from the image vector table.
 *
 ******************************************************************************
 */

const void* hab_getPayloadAddrFromIvt(const void* ivtAddr)
{
  const T_HAB_IVT* ivt = ivtAddr;
  const T_HAB_BOOT_DATA* boot;
  const void* result;

  boot = ivt->boot_data;
  if(boot != NULL)
  {
    result = (void*)boot->start;
  }
  else
  {
    result = 0;
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Get the image's payload size from the image vector table.
 *
 ******************************************************************************
 */

uint32 hab_getPayloadSizeFromIvt(const void* ivtAddr)
{
  const T_HAB_IVT* ivt = ivtAddr;
  const T_HAB_BOOT_DATA* boot;
  uint32 result;

  boot = ivt->boot_data;
  if(boot != NULL)
  {
    result = boot->size - (uint32)((void*)ivt->entry - (void*)boot->start);
  }
  else
  {
    result = 0;
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Get the image's entire size from the image vector table.
 *
 ******************************************************************************
 */

uint32 hab_getImageSizeFromIvt(const void* ivtAddr)
{
  const T_HAB_IVT* ivt = ivtAddr;
  const T_HAB_BOOT_DATA* boot;
  uint32 result;

  boot = ivt->boot_data;
  if(boot != NULL)
  {
    result = boot->size;
  }
  else
  {
    result = 0;
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

int hab_getStatus(void)
{
  T_HAB_STATUS status;
  int result;

  status = hab_data.rvt->reportStatus(NULL, NULL);
  switch(status)
  {
  case HAB_STATUS_SUCCESS:
    result = HAB_API_OK;
    break;
  case HAB_STATUS_WARN:
    result = HAB_API_WARN;
    break;
  case HAB_STATUS_FAIL:
    result = HAB_API_FAIL;
    break;
  default:
    result = HAB_API_FAIL;
    break;
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

int hab_init(void)
{
  T_HAB_STATUS status;
  int result = HAB_API_OK;

  status = hab_data.rvt->entry();
  if(HAB_STATUS_SUCCESS != status)
  {
    result = HAB_API_FAIL;
    TRACE_INFO("Failed to enter HAB!\n");
  }
  else
  {
    TRACE_INFO("Successfully entered HAB!\n");
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

int hab_deinit(void)
{
  T_HAB_STATUS status;
  int result = HAB_API_OK;

  status = hab_data.rvt->exit();
  if(HAB_STATUS_SUCCESS != status)
  {
    TRACE_INFO("Failed to exit HAB!\n");
    result = HAB_API_FAIL;
  }
  else
  {
    TRACE_INFO("Successfully exited HAB!\n");
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

int hab_checkTargetMem(void* startAddr, uint32 length)
{
  T_HAB_STATUS status;
  int result = HAB_API_OK;

  status = hab_data.rvt->checkTarget(HAB_TRG_MEM, startAddr, length);
  if(status != HAB_STATUS_SUCCESS)
  {
    result = HAB_API_FAIL;
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

boolean hab_isIvtValid(const void* ivtAddr)
{
  boolean result = !FALSE;
  T_HAB_IVT* ivt = (void*)ivtAddr;
  
  if(HAB_TAG_IVT != ivt->hdr.tag)
  {
    /* Invalid tag */
    result = FALSE;
  }
  else if( sizeof(T_HAB_IVT) != HAB_GET_STRUCT_SIZE(ivt->hdr) )
  {
    /* Invalid size */
    result = FALSE;
  }
  else if(HAB_MAJOR_VERSION != HAB_GET_MAJOR_VERSION(ivt->hdr.par))
  {
    /* Invalid major version */
    result = FALSE;
  }
  else
  {
    /* Looks like a valid IVT header */
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */
#define IVT_ALIGNMENT 0x1000
#define CSF_PAD_SIZE  0x2000

int hab_authImage(uint32 imageAddr, uint32 imageSize, uint32 ivtOffs)
{
  T_HAB_STATUS status;
  T_HAB_ENTRY entry;
  int result = HAB_API_FAIL;

  /* Enable clocks of CAAM module */
//  caam_enableClocks();
  
  TRACE_INFO("Authenicate image at address %08X\n", imageAddr);
  status = hab_data.rvt->entry();
  if( HAB_STATUS_SUCCESS == status )
  {
    /* r0: cid
     * r1: ivt_offset
     * r2: sp + 0C &start
     * r3: sp + 08 &length
     * 
     * [sp + 0C] start
     * [sp + 08] length
     * [sp + 00] callback
     */
    TRACE_INFO("Authenticating image...");
    entry = hab_data.rvt->authenticate_image(1, ivtOffs, (void**)&imageAddr, (uint32*)&imageSize, NULL);
    if(entry == NULL)
    {
      /* Note that on failed authentication NULL is only returned
       * in closed configuration.
       * So in open configuration the authImage function will return the entry
       * even if the authentication failed.
       */
      result = HAB_API_FAIL;
      TRACE_INFO("failed\n");
    }
    else if(HAB_API_OK != hab_getStatus())
    {
      result = HAB_API_FAIL;
      TRACE_INFO("failed\n");
    }
    else
    {
      result = HAB_API_OK;
      TRACE_INFO("success\n");
      TRACE_INFO("Entry: 0x%08X\n", entry);
    }
  }

  if( HAB_STATUS_SUCCESS != status )
  {
    TRACE_INFO("HAB: Failed to enter!\n");
  }
  else if(HAB_STATUS_SUCCESS != hab_data.rvt->exit() )
  {
    TRACE_INFO("HAB: Failed to exit!\n");
  }
  return result;
}

