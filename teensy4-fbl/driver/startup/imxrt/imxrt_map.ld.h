
MEMORY
{
    ITCM_RAM            (rwx): ORIGIN = 0x00000000, LENGTH = 512K
    BOOT_ROM            (rx) : ORIGIN = 0x00200000, LENGTH = 128K
    DTCM_RAM            (rwx): ORIGIN = 0x20000000, LENGTH = 512K
 
    ONCHIP_RAM2         (rwx): ORIGIN = 0x20200000, LENGTH = 512K     /* Internal SRAM */
    HAB_RAM             (rwx): ORIGIN = 0x20200000, LENGTH =   7K
    OCRAM               (rwx): ORIGIN = 0x20208000, LENGTH = 480K

    ONCHIP_RAM1         (rwx): ORIGIN = 0x20280000, LENGTH = 512K     /* Internal SRAM */
    FLEX_RAM            (rwx): ORIGIN = 0x20280000, LENGTH = 512K     /* Internal SRAM */
    ONCHIP_RAM3         (rwx): ORIGIN = 0x20300000, LENGTH = 512K     /* Internal SRAM reserved */

    M7SW_CODE           (rwx): ORIGIN = 0x20300000, LENGTH = 32K
    M7SW_DATA           (rwx): ORIGIN = 0x20308000, LENGTH = 32K
    M7SW_STACK          (rwx): ORIGIN = 0x20316000, LENGTH = 8K

    CRASH_STACK         (rwx): ORIGIN = 0x20318000, LENGTH = 8K       /* */
    M7SW_HEAP_STACK     (rw) : ORIGIN = 0x2001C000, LENGTH = 128K
    EXT_FLASH           (rwx): ORIGIN = 0x60000000, LENGTH = 1984K    /* 0x001F0000*/

    FBL_CODE            (rwx): ORIGIN = 0x60000000, LENGTH = 64K
    APP_CODE            (rwx): ORIGIN = 0x60010000, LENGTH = 1920K
}

