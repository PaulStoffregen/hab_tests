#ifndef UART_DDM_C
#define UART_DDM_C
#endif /* UART_DDM_C */

#include "bsp.h"
#include "uart_irq.h"
#include "uart_prv.h"
#include "uart_ddm.h"


#if (FF_DDM_USE_BITMAP != 0)
static uint8 usedMap;
#endif /* (FF_DDM_USE_BITMAP != 0) */

/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Get device properties for given device ID
 *
 ******************************************************************************
 */

const T_UART_CTL_DESC* uart_getDevProps(uint32 ctlID)
{
  const T_UART_CTL_DESC* ctlDesc;
  sint8 match = -1;
  uint8 i;

  if(ctlID < UART_NUM_DEVS_AVAIL)
  {
    /* Device ID is valid. So iterate over the number of devices used in the
     * context of the project and try to find one matching the given device ID.
     */
    for(i = 0; i < UART_NUM_DEVS_USED; i++)
    {
      ctlDesc = &uart_ctlDevDescTbl[i];
      if(ctlDesc->devID == ctlID)
      {
        /* Device ID matched */
        match = i;
        break;
      }
    }
  }

  if(match == -1)
  {
    ctlDesc = NULL;
  }
  return ctlDesc;
}


/*!
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   This function is used to gather the runtime data block for a UART
 *   controller device identified by the given device ID.
 *
 ******************************************************************************
 */
 
T_UART_CTL_DATA* uart_getDevData(uint32 devID)
{
  T_UART_CTL_DATA* ctlData = NULL;
  
  if(devID < UART_NUM_DEVS_AVAIL)
  {
    /* Device ID is valid */
    ctlData = uart_ctlDevDataPtr[devID];
  }
  return ctlData;
}


/*!
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   This function is used to gather the base address for a UART controller
 *   device instance, identified by the given device ID.
 *
 ******************************************************************************
 */
 
uint32 uart_getDevBase(uint32 devID)
{
  uint32 base = 0;

#if 0
  if(devID < UART_NUM_DEVS_AVAIL)
  {
    /* Device ID is valid */
    base = uart_ctlDevBaseTable[devID];
  }
#else
  T_UART_CTL_DATA* ctlData = uart_getDevData(devID);
  if(NULL != ctlData)
  {
    base = ctlData->props->base;
  }
#endif
  return base;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   This function is called by the driver's init function when a new device
 *   instance has to be initialized, in order to allocate a new device runtime
 *   data block.
 *   The given parameter tells the device data manager which physical device
 *   instance the block shall be allocated for.
 *   The given device ID must not exceed the number of devices available on
 *   the given SoC. Otherwise the function returns with an error code.
 *
 ******************************************************************************
 */

T_STATUS uart_allocDevData(uint32 devID)
{
  T_UART_CTL_DATA* ctlData = NULL;
  T_STATUS result = UART_OK;
  uint8 i;

  if(devID < UART_NUM_DEVS_AVAIL)
  {
    /* Device ID is valid, so check whether device data has already
     * been allocated for this device instance.
     */
    ctlData = uart_ctlDevDataPtr[devID];
  }
  else
  {
    /* Given device ID is invalid */
    result = UART_ERROR_INVALID;
  }

  if(result != UART_OK)
  {
    /* Previous error:
     * - SPI_ERROR_INVALID
     */
  }
  else if(ctlData != NULL)
  {
    /* Device ID is already associated with a data block,
     * so no need to allocate new one
     */
    // TODO: [FM] Shall a this report an error?
  }
  else
  {
    /* Device ID is valid and is not yet associated with a data block.
     * So search for a free data block.
     */
    result = UART_ERROR_NOMEM;

    /* Iterate over the number of devices and thus device data blocks
     * used in the context of the project and check for each of them
     * if it is already used.
     */
#if (FF_DDM_USE_BITMAP != 0)
    uint8 mask = 1;
    for(i = 0; i < UART_NUM_DEVS_USED; i++)
    {
      if( (usedMap & mask) != 0)
      {
        /* Data block is in use */
      }
      else
      {
        ctlData = &uart_ctlDevData[i];
        result = SPI_OK;
        break;
      }
      mask <<= 1;
    }
#else /* (FF_DDM_USE_BITMAP == 0) */
    for(i = 0; i < UART_NUM_DEVS_USED; i++)
    {
      T_UART_CTL_DATA* dataBlkAddr;
      T_UART_CTL_DATA* devDataPtr;
      int k;
      
      /* Get the next device data block */
      dataBlkAddr = &uart_ctlDevData[i];

      /* Check if it is already used, so compare it against all device's
       * runtime data pointers.
       */
      for(k = 0; k < UART_NUM_DEVS_AVAIL; k++)
      {
        devDataPtr = uart_ctlDevDataPtr[k];
        if(dataBlkAddr == devDataPtr)
        {
          /* Address of currently selected device data block (i) is already
           * assosicated with a device, as it is part of the device's block
           * list, so get address of next device data block.
           */
           break;
        }
        else
        {
          /* The currently selected device ID (k) is not associated with the
           * address of the currently selected device data block (i), so
           * continue with next device ID.
           */
        }
      }
      /* We leave the for-loop by either of the following two conditions:
       * 1. If we reach the end of the loop without break, in which case
       *    the both compared adresses are non-equal.
       * 2. Due to a break statement, in which case the selected device data
       *    block is already in use and thus equal to the recently checked
       *    device data pointer.
       *
       * In the 1st case we need found an appropriate device data block.
       * In the 2nd case we need to continue until we find an appropriate
       * device data block or the loop ends.
       */
      if(dataBlkAddr != devDataPtr)
      {
        /* We found a an appropriate device data block */
        ctlData = dataBlkAddr;
        result = UART_OK;
        break;
      }
      else
      {
        /* Continue with next block */
      }
    }
#endif /* (FF_DDM_USE_BITMAP != 0) */
    uart_ctlDevDataPtr[devID] = ctlData;
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   This function is called by the driver's deinit function when a new device
 *   instance has to be deinitialized, in order to free the previously used
 *   device runtime data block.
 *   The given parameter tells the device data manager which physical device
 *   instance the block shall be freeed for.
 *   The given device ID must not exceed the number of devices available on
 *   the given SoC. Otherwise the function returns with an error code.
 *
 ******************************************************************************
 */

T_STATUS uart_freeDevData(uint32 devID)
{
  T_UART_CTL_DATA* ctlData = NULL;
  T_STATUS result = UART_OK;

  if(devID < UART_NUM_DEVS_AVAIL)
  {
    /* Device ID is valid */
    ctlData = uart_ctlDevDataPtr[devID];
  }
  else
  {
    result = UART_ERROR_INVALID;
  }

  if(ctlData != NULL)
  {
    /* Device owns a data block, remove association. */    
    uart_ctlDevDataPtr[devID] = NULL;
#if (FF_DDM_USE_BITMAP != 0)
    uint8 i;
    uint8 mask = 1;

    /* Find the index of that block. */
    for(i = 0; i < UART_NUM_DEVS_USED; i++)
    {
      if(&uart_ctlDevData[i] != ctlData)
      {
        /* Not the right data block */
      }
      else
      {
        /* Found the right data block, so mark
         * as free.
         */
        usedMap &= ~mask;
        break;
      }
      mask <<= 1;
    }
#endif /* (FF_DDM_USE_BITMAP != 0) */
  }
  else
  {
    result = UART_ERROR_INVALID;
  }
  return result;
}

