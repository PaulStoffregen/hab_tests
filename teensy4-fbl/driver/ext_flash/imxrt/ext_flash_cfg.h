#ifndef EXT_FLASH_CFG_H
#define EXT_FLASH_CFG_H

/* Define write page size (number of bytes which can be programmed at a time) */
#define FLASH_WRITE_PAGE_SIZE  (sizeof(uint32))

/* Define blank flash value */
#define FLASH_BLANK_VALUE       (0xFFFFFFFFUL)

#define FLASH_PAGE_SIZE         (0x100)  /* 256 Byte */

#define FLASH_ERASE_SECTOR_SIZE (0x1000) /* 4 KiB */


#if defined(EXT_FLASH_C)
static const T_SECTOR_INFO flash_sectorMap[1] =
{
  [0] =
  {
    .sectAddr = 0x60000000,
	.sectSize = 0x00001000,
  },
};
#else /* !defined(EXT_FLASH_C) */
#endif /* (EXT_FLASH_C) */
#endif /* EXT_FLASH_CFG_H */