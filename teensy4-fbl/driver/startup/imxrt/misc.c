#ifndef MISC_C
#define MISC_C
#endif /* MISC_C */


#include "bsp.h"
#include "config.h"
#include "trace_pub.h"
#include "libc.h"
#include "reg.h"
#include "misc.h"


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void earlyDbg_init(void)
{
  volatile uint32 reg;

//  IOMUXC_SW_MUX_CTL_PAD_GPIO_AD_B1_06 = 2; // Arduino pin 17
  REG32_WR_BASE_OFFS(2, IOMUXC_BASE, MUXCONF_GPIO_AD_B1_06_OFFS);

//  CCM_CCGR0 |= CCM_CCGR0_LPUART3(CCM_CCGR_ON); // turn on Serial4
  REG32_RD_BASE_OFFS(reg, CCM_DIGITAL_BASE, CCM_CCGR0_OFFS);
  reg |= ( 0
         | BF_SET(CCM_CG_eCLK_ON_ALW, LPUART3_CLK_ENA_BF)
         );
  REG32_WR_BASE_OFFS(reg, CCM_DIGITAL_BASE, CCM_CCGR0_OFFS);

//  LPUART3_BAUD = LPUART_BAUD_OSR(25) | LPUART_BAUD_SBR(8); // ~115200 baud
  REG32_RD_BASE_OFFS(reg, UART3_BASE, LPUART_BAUD_RATE_OFFS);
  reg = ( 0
        | BF_SET(25, LPUART_BAUD_OVER_SAMPLE_RATIO_BF)
        | BF_SET(8, LPUART_BAUD_RATE_MODULO_DIV_BF)
        );
  REG32_WR_BASE_OFFS(reg, UART3_BASE, LPUART_BAUD_RATE_OFFS);

//  LPUART3_CTRL = LPUART_CTRL_TE;
  REG32_RD_BASE_OFFS(reg, UART3_BASE, LPUART_CTRL_OFFS);
  reg = ( 0
        | BF_SET(1, LPUART_CTRL_TE_BF)
        );
  REG32_WR_BASE_OFFS(reg, UART3_BASE, LPUART_CTRL_OFFS);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void earlyDbg_sendChar(char c)
{
  uint32 statusReg;
//  while(!(LPUART3_STAT & LPUART_STAT_TDRE)) ; // wait
  do
  {
    REG32_RD_BASE_OFFS(statusReg, UART3_BASE, LPUART_STATUS_OFFS);
  } while(0 == (statusReg & BF_MASK(LPUART_STATUS_TDRE_BF)));
//  LPUART3_DATA = c;
  REG32_WR_BASE_OFFS(c, UART3_BASE, LPUART_DATA_OFFS);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void earlyDbg_sendStr(const char* str)
{
  while(*str != '\0')
  {
    earlyDbg_sendChar(*str);
    str++;
  }
}


void boot_dumpBootInfo(void)
{
  uint32 reg;
  uint32 tmp;

  TRACE_INFO("SRC Boot Info\n");

  REG32_RD_BASE_OFFS(reg, SRC_BASE, SRC_SBMR2_OFFS);
  TRACE_INFO(" SRC_SBMR2 = %08X\n", reg);

  tmp = BF_GET(reg, SRC_SBMR2_BMOD_BF);
  TRACE_INFO("  BOOT_MODE[1:0] = %d\n", tmp);

  tmp = BF_GET(reg, SRC_SBMR2_BT_FUSE_SEL_BF);
  TRACE_INFO("  BT_FUSE_SEL =    %d\n", tmp);

  tmp = BF_GET(reg, SRC_SBMR2_DIR_BT_DIS_BF);
  TRACE_INFO("  DIR_BT_DIS =     %d\n", tmp);

  tmp = BF_GET(reg, SRC_SBMR2_SEC_CONFIG_BF);
  TRACE_INFO("  SEC_CONFIG =     %d\n", tmp);


  REG32_RD_BASE_OFFS(reg, SRC_BASE, SRC_SBMR1_OFFS);
  TRACE_INFO(" SRC_SBMR1 = %08X\n", reg);

  tmp = BF_GET(reg, SRC_SBMR1_BOOT_CFG1_BF);
  TRACE_INFO("  BOOT_CFG1 =      0x%02X\n", tmp);

  tmp = BF_GET(reg, SRC_SBMR1_BOOT_CFG2_BF);
  TRACE_INFO("  BOOT_CFG2 =      0x%02X\n", tmp);

  tmp = BF_GET(reg, SRC_SBMR1_BOOT_CFG3_BF);
  TRACE_INFO("  BOOT_CFG3 =      0x%02X\n", tmp);

  tmp = BF_GET(reg, SRC_SBMR1_BOOT_CFG4_BF);
  TRACE_INFO("  BOOT_CFG4 =      0x%02X\n", tmp);
}


static void xdelay(uint32 dly)
{
   volatile int timeout = dly;
   while(timeout > 0)
   {
     timeout--;
   }
}

static void xblink(uint16 cnt)
{
  uint32 onTime = 0x02000000;
  uint32 offTime = 0x02800000;

  while(cnt > 0)
  {
    REG32_WR_BASE_OFFS((7<<2), GPIO7_BASE, GPIO_DR_SET_OFFS);
    xdelay(onTime);
    REG32_WR_BASE_OFFS((7<<2), GPIO7_BASE, GPIO_DR_CLR_OFFS);  
    xdelay(offTime);
    cnt--;
  }
}

static void xflash(uint16 cnt)
{
  uint32 onTime = 0x00800000;
  uint32 offTime = 0x00A00000;

  while(cnt > 0)
  {
    REG32_WR_BASE_OFFS((7<<2), GPIO7_BASE, GPIO_DR_SET_OFFS);
    xdelay(onTime);
    REG32_WR_BASE_OFFS((7<<2), GPIO7_BASE, GPIO_DR_CLR_OFFS);  
    xdelay(offTime);
    cnt--;
  }
}

void xhit(uint32 nr)
{
  xblink(1);
  xflash(nr);
  xdelay(0x08000000);
}


void reset_PFD()
{
  //Reset PLL2 PFDs, set default frequencies:
//  CCM_ANALOG_PFD_528_SET = (1 << 31) | (1 << 23) | (1 << 15) | (1 << 7);
  REG32_WR_BASE_OFFS((1 << 31) | (1 << 23) | (1 << 15) | (1 << 7), CCM_ANALOG_BASE, CCM_ANALOG_PLL_PFD_528_SET_OFFS);
//  CCM_ANALOG_PFD_528 = 0x2018101B; // PFD0:352, PFD1:594, PFD2:396, PFD3:297 MHz
  REG32_WR_BASE_OFFS(0x2018101B, CCM_ANALOG_BASE, CCM_ANALOG_PLL_PFD_528_OFFS);
  //PLL3:
//  CCM_ANALOG_PFD_480_SET = (1 << 31) | (1 << 23) | (1 << 15) | (1 << 7);
  REG32_WR_BASE_OFFS((1 << 31) | (1 << 23) | (1 << 15) | (1 << 7), CCM_ANALOG_BASE, CCM_ANALOG_PLL_PFD_480_SET_OFFS);
//  CCM_ANALOG_PFD_480 = 0x13110D0C; // PFD0:720, PFD1:664, PFD2:508, PFD3:454 MHz
  REG32_WR_BASE_OFFS(0x13110D0C, CCM_ANALOG_BASE, CCM_ANALOG_PLL_PFD_480_OFFS);
}


void armv7m_default_handler(void)
{
   while(1)
   {
      xflash(3);
      xdelay(0x02000000);
      xblink(3);
      xdelay(0x02000000);
      xflash(3);
      xdelay(0x08000000);
   }
}


void ldr_dumpLinkerInfo(void)
{
  extern const uint32 __image_start;
  extern const uint32 __image_size;
  
  extern const uint32 __fls_cfg_start;
  extern const uint32 __fls_cfg_end;

  extern const uint32 __sw_info_start;
  extern const uint32 __sw_info_end;

  extern const uint32 __boot_load;
  extern const uint32 __boot_start;
  extern const uint32 __boot_end;

  extern const uint32 __ivt_load;
  extern const uint32 __ivt_start;
  extern const uint32 __ivt_end;

  extern const uint32 __boot_data_start;
  extern const uint32 __boot_data_end;
  
  extern const uint32 __text_load;
  extern const uint32 __text_start;
  extern const uint32 __text_end;

  extern const uint32 __data_load;
  extern const uint32 __data_start;
  extern const uint32 __data_end;

  extern const uint32 __bss_start;
  extern const uint32 __bss_end;

  extern const uint32 __pad_start;
  extern const uint32 __pad_end;

  extern const uint32 __csf_start;
  extern const uint32 __csf_end;

  uint32 len;
  uint32 sAddr;
  uint32 eAddr;
  uint32 lAddr;
  uint32 totalLen = 0;
  
  TRACE_INFO("Linker Info\n");
  TRACE_INFO("Binary    Size      StartAddr EndAddr\n");

  sAddr = (uint32)&__image_start;
  eAddr = (uint32)&__image_start + (uint32)&__image_size;
  len = eAddr - sAddr;
  TRACE_INFO(" image    %08X  %08X  %08X (%d Bytes)\n\n",
    len,
    sAddr,
    eAddr,
    len);

  TRACE_INFO("Sections:\n");
  TRACE_INFO("Name      Size      VMA       LMA\n");

  sAddr = (uint32)&__fls_cfg_start;
  eAddr = (uint32)&__fls_cfg_end;
  lAddr = (uint32)&__fls_cfg_start;
  len = eAddr - sAddr;
  totalLen += len;
  TRACE_INFO(" .flsCfg  %08X  %08X  %08X (%d Bytes)\n",
    len,
    sAddr,
    lAddr,
    len);

  sAddr = (uint32)&__sw_info_start;
  eAddr = (uint32)&__sw_info_end;
  lAddr = (uint32)&__sw_info_start;
  len = eAddr - sAddr;
  totalLen += len;
  TRACE_INFO(" .sw_info %08X  %08X  %08X (%d Bytes)\n",
    len,
    sAddr,
    lAddr,
    len);

  sAddr = (uint32)&__boot_start;
  eAddr = (uint32)&__boot_end;
  lAddr = (uint32)&__boot_load;
  len = eAddr - sAddr;
  totalLen += len;
  TRACE_INFO(" .boot    %08X  %08X  %08X (%d Bytes)\n",
    len,
    sAddr,
    lAddr,
    len);

  sAddr = (uint32)&__ivt_start;
  eAddr = (uint32)&__ivt_end;
  lAddr = (uint32)&__ivt_load;
  len = eAddr - sAddr;
  totalLen += len;
  TRACE_INFO(" .ivt     %08X  %08X  %08X (%d Bytes)\n",
    len,
    sAddr,
    lAddr,
    len);

  sAddr = (uint32)&__boot_data_start;
  eAddr = (uint32)&__boot_data_end;
  lAddr = (uint32)&__boot_data_start;
  len = eAddr - sAddr;
  totalLen += len;
  TRACE_INFO(" .bdata   %08X  %08X  %08X (%d Bytes)\n",
    len,
    sAddr,
    lAddr,
    len);

  sAddr = (uint32)&__text_start;
  eAddr = (uint32)&__text_end;
  lAddr = (uint32)&__text_load;
  len = eAddr - sAddr;
  totalLen += len;
  TRACE_INFO(" .text    %08X  %08X  %08X (%d Bytes)\n",
    len,
    sAddr,
    lAddr,
    len);

#if 0
  sAddr = (uint32)&_stext;
  eAddr = (uint32)&_etext;
  lAddr = (uint32)&_stextload;
  len = eAddr - sAddr;
  TRACE_INFO(" .itcm   %08X %08X %08X (%d Bytes)\n",
    len,
    sAddr,
    lAddr,
    len);

  sAddr = (uint32)&_sflashloader;
  eAddr = (uint32)&_eflashloader;
  lAddr = (uint32)&_sflashloaderload;
  len = eAddr - sAddr;
  TRACE_INFO(" .flsldr %08X %08X %08X (%d Bytes)\n",
    len,
    sAddr,
    lAddr,
    len);
#endif

  sAddr = (uint32)&__data_start;
  eAddr = (uint32)&__data_end;
  lAddr = (uint32)&__data_load;
  len = eAddr - sAddr;
  totalLen += len;
  TRACE_INFO(" .data    %08X  %08X  %08X (%d Bytes)\n",
    len,
    sAddr,
    lAddr,
    len);

  sAddr = (uint32)&__pad_start;
  eAddr = (uint32)&__pad_end;
  lAddr = (uint32)&__pad_start;
  len = eAddr - sAddr;
  totalLen += len;
  TRACE_INFO(" .pad     %08X  %08X  %08X (%d Bytes)\n",
    len,
    sAddr,
    lAddr,
    len);

  sAddr = (uint32)&__bss_start;
  eAddr = (uint32)&__bss_end;
  lAddr = 0;
  len = eAddr - sAddr;
  TRACE_INFO(" .bss     %08X  %08X  %08X (%d Bytes)\n",
    len,
    sAddr,
    lAddr,
    len);

  sAddr = (uint32)&__csf_start;
  eAddr = (uint32)&__csf_end;
  lAddr = (uint32)&__csf_start;
  len = eAddr - sAddr;
  TRACE_INFO(" .csf     %08X  %08X  %08X (%d Bytes)\n",
    len,
    sAddr,
    lAddr,
    len);

  TRACE_INFO(" total size: %08X (%dBytes)\n", totalLen, totalLen);
}

