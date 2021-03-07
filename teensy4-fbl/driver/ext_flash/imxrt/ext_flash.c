#ifndef EXT_FLASH_C
#define EXT_FLASH_C
#endif /* EXT_FLASH_C */


#include "bsp.h"

#include "libc.h"
#include "rom_api.h"

#include "ext_flash.h"


#if 0
T_STATUS extflash_write(uint32 dstAddr, uint8 srcBuf[], sint32 numBytes)
{
  return STATUS_eOK;
}
#else
T_STATUS extflash_write(uint32 dstAddr, uint8 srcBuf[], sint32 numBytes)
{
  T_ROM_API* romApi;
  T_FLEXSPI_NOR_CFG norCfg;
  T_SER_NOR_ONFIG_OPTION cfgOpt =
  {
    .option0.U = 0xC0000008,
    .option1.U = 0,
  };

  T_STATUS result = STATUS_eNOK;
  uint32 pageAddr = dstAddr & ~(FLASH_PAGE_SIZE - 1);
  uint32 pageOffs = dstAddr & (FLASH_PAGE_SIZE - 1);
  uint32 bytesToCopyToBuffer;
  uint32 bytesWritten;
  uint32 pageBuffer[FLASH_PAGE_SIZE / sizeof(uint32)];
  uint8* pagePtr = (uint8*)(void*)pageBuffer;
  uint32 devID = 0;

  romApi = romApi_getAddr(); 
  if(0 != romApi->norFlashApi->getConfig(devID, &norCfg, &cfgOpt))
  {
    /* Failed */
  }
  else if(0 != romApi->norFlashApi->init(devID, &norCfg))
  {
    /* Failed */
  }
  else
  {
    pageOffs = dstAddr & (FLASH_PAGE_SIZE - 1);
    pageAddr = dstAddr - pageOffs;

    for(bytesWritten = 0; bytesWritten < numBytes; bytesWritten += bytesToCopyToBuffer)
    {
      if(pageOffs > 0)
      {
        bytesToCopyToBuffer = FLASH_PAGE_SIZE - pageOffs;
      }
      else
      {
        bytesToCopyToBuffer = FLASH_PAGE_SIZE;
      }

      if( (numBytes - bytesWritten) < bytesToCopyToBuffer)
      {
        bytesToCopyToBuffer = numBytes - bytesWritten;
      }

      if(bytesToCopyToBuffer != FLASH_PAGE_SIZE)
      {
        libc_memset(pageBuffer, FLASH_BLANK_VALUE, sizeof(pageBuffer));
      }

      libc_memcpy(&pagePtr[pageOffs], &srcBuf[bytesWritten], bytesToCopyToBuffer);
      if(0 != romApi->norFlashApi->writePage(devID, &norCfg, pageAddr, (uint8*)pageBuffer))
      {
        /* Fail */
        result = STATUS_eNOK;
        break;
      }
      else
      {
        /* Success */
        pageAddr += FLASH_PAGE_SIZE;
        pageOffs = 0;
        result = STATUS_eOK;
      }
    }
  }
  return result;
}
#endif


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Erase specified region of flash
 *
 * @par Description:
 *   Function is used to erase the specified flash region where the minimum
 *   erase unit is one erase sector.
 *
 ******************************************************************************
 */
#if 0
T_STATUS extflash_erase(uint32 logAddr, uint32 numBytes)
{
  return STATUS_eOK;
}
#else
T_STATUS extflash_erase(uint32 logAddr, uint32 numBytes)
{
  T_ROM_API* romApi;
  T_FLEXSPI_NOR_CFG norCfg;
  T_SER_NOR_ONFIG_OPTION cfgOpt =
  {
    .option0.U = 0xC0000008,
    .option1.U = 0,
  };

  T_STATUS result = STATUS_eNOK;
  uint32 sectAddr = logAddr & ~(FLASH_ERASE_SECTOR_SIZE - 1);
  uint32 sectOffs = logAddr &  (FLASH_ERASE_SECTOR_SIZE - 1);
  uint32 regionSize  = sectOffs + numBytes;
  uint32 devID = 0;

  romApi = romApi_getAddr();

  /* Get the flash configuration block */
  if(0 != romApi->norFlashApi->getConfig(devID, &norCfg, &cfgOpt))
  {
    /* Failed */
  }
  /* Initialize the serial NOR device */
  else if(0 != romApi->norFlashApi->init(devID, &norCfg))
  {
    /* Failed */
  }
  /* Erase requested region */
  else if(0 != romApi->norFlashApi->erase(devID, &norCfg, sectAddr, regionSize))
  {
    /* Failed */
  }
  else
  {
    result = STATUS_eOK;
  }
  return result;
}
#endif

