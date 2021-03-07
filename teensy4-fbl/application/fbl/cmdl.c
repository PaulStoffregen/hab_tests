#ifndef CMDL_C
#define CMDL_C
#endif /* CMDL_C */


#include "bsp.h"
#include "config.h"
#include "reg.h"
#include "hab.h"
#include "cli.h"
#include "libc.h"
#include "uart.h"
#include "rom_api.h"
#include "swinfo.h"
#include "sw_release.h"
#include "misc.h"
#include "fbl.h"
#include "bmgr.h"
#include "ocotp.h"
#include "ocotp_info.h"
#include "hab_api.h"
#include "hab_info.h"
#include "target_cfg.h"

#include "cmdl.h"

#include <string.h> /* for memset() */
#include <stdlib.h>

#define CMDL_MAX_CMD_BUF_LEN 32
#define CMDL_PROMPT "FBL>"

typedef struct
{
  uint8  echoOff;  /* */
  char   cmdBuffer[CMDL_MAX_CMD_BUF_LEN];
  int    bufPos;
  uint8  lastCmdSize;
}T_CMDL_DATA;

static T_CMDL_DATA cmdl_data;


/* Implemantation of the commands */

/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_helpCmd(const char* argStr)
{
  libc_puts("help...\n");
  libc_puts("boot .. Enter FBL\n");
  libc_puts("authApp .. Authenticate application\n");
  libc_puts("bootInfo .. Get boot related info\n");
  libc_puts("lockInfo .. Get fuse lock info\n");
  libc_puts("sdpUsb .. Enter serial boot via USB\n");
  libc_puts("sdpUart .. Enter serial boot via UART\n");
  libc_puts("fblInfo .. Show FBL's SW info\n");
  libc_puts("appInfo .. Show App's SW info\n");
  libc_puts("ldInfo .. Show FBL linker info\n");
  libc_puts("ivtInfo .. Show FBL IVT\n");
  libc_puts("csfInfo .. Show FBL CSF\n");
  libc_puts("habInfo .. Show HAB info\n");
  libc_puts("rvtInfo .. Show RVT info\n");
  libc_puts("srkInfo .. Show SRK hash\n");
  libc_puts("habCheck .. Check HAB status\n");
  libc_puts("habStatus .. Show HAB status\n");
  libc_puts("cpuID .. Show CPU UID\n");
  
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_fblInfoCmd(const char* argStr)
{
  const T_SWINFO* swInfo;
  const T_SW_REL_INFO* swRelInfo;

  swInfo = swinfo_getOwnSwInfo();

  libc_printf("FBL Software Info:\n");
  libc_printf(" ImgAddr:  %08x\n", swInfo->imgAddr);
  libc_printf(" ImgSize:  %08x (%d Bytes)\n", swInfo->imgSize, swInfo->imgSize);
  libc_printf(" ImgCRC:   %08x\n", swInfo->crc);

  swRelInfo = (const T_SW_REL_INFO*)swInfo->versionAddr;
  uint16 layoutID = BYTES_TO_UINT16(swRelInfo->layoutID[0], swRelInfo->layoutID[1]);

  if(swInfo->versionLen != sizeof(T_SW_REL_INFO))
  {
    /* Invalid SW release info */
  }
  else if(layoutID != 0)
  {
    /* Invalid layout ID */
  }
  else
  {
    /* */
    uint16 major = BYTES_TO_UINT16(swRelInfo->major[0], swRelInfo->major[1]);
    uint16 minor = BYTES_TO_UINT16(swRelInfo->minor[0], swRelInfo->minor[1]);
    uint16 patch = BYTES_TO_UINT16(swRelInfo->patch[0], swRelInfo->patch[1]);
    uint32 rev = BYTES_TO_UINT32(swRelInfo->revision[0], swRelInfo->revision[1], swRelInfo->revision[2], swRelInfo->revision[3]);

    libc_printf("FBL Release Info:\n");
    libc_printf(" Version:  %d.%d.%d\n", major, minor, patch);
    libc_printf(" Revision: %d\n", rev);
    libc_printf(" Commit:   %s\n", swRelInfo->shortHash);
    libc_printf(" Release:  %s %s\n", swRelInfo->relDate, swRelInfo->relTime);
    libc_printf(" Built:    %s %s\n", swRelInfo->buildDate, swRelInfo->buildTime);
  }
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_appInfoCmd(const char* argStr)
{
  const T_SWINFO* swInfo;
  const T_SW_REL_INFO* swRelInfo;
  uint16 versionLen = 0;
  uint16 layoutID;

  libc_printf("APP Software Info:\n");
  swInfo = (const T_SWINFO*)FBL_APP_SWINFO_ADDR;
  if(swInfo->tag != SWINFO_TAG)
  {
    /* Invalid application SW */
    libc_printf(" No valid application image\n");
  }
  else
  {
    libc_printf(" ImgAddr:  %08x\n", swInfo->imgAddr);
    libc_printf(" ImgSize:  %08x (%d Bytes)\n", swInfo->imgSize, swInfo->imgSize);
    libc_printf(" ImgCRC:   %08x\n", swInfo->crc);

    versionLen = swInfo->versionLen;
    swRelInfo = (const T_SW_REL_INFO*)swInfo->versionAddr;
    layoutID = BYTES_TO_UINT16(swRelInfo->layoutID[0], swRelInfo->layoutID[1]);
  }
  
  if(versionLen != sizeof(T_SW_REL_INFO))
  {
    /* Invalid SW release info */
  }
  else if(layoutID != 0)
  {
    /* Invalid layout ID */
  }
  else
  {
    /* */
    uint16 major = BYTES_TO_UINT16(swRelInfo->major[0], swRelInfo->major[1]);
    uint16 minor = BYTES_TO_UINT16(swRelInfo->minor[0], swRelInfo->minor[1]);
    uint16 patch = BYTES_TO_UINT16(swRelInfo->patch[0], swRelInfo->patch[1]);
    uint32 rev = BYTES_TO_UINT32(swRelInfo->revision[0], swRelInfo->revision[1], swRelInfo->revision[2], swRelInfo->revision[3]);

    libc_printf("APP Release Info:\n");
    libc_printf(" Version:  %d.%d.%d\n", major, minor, patch);
    libc_printf(" Revision: %d\n", rev);
    libc_printf(" Commit:   %s\n", swRelInfo->shortHash);
    libc_printf(" Release:  %s %s\n", swRelInfo->relDate, swRelInfo->relTime);
    libc_printf(" Built:    %s %s\n", swRelInfo->buildDate, swRelInfo->buildTime);
  }
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_sdpUsbCmd(const char* argStr)
{
  libc_puts("Run SDP on USB\n");
  T_ROM_API* romApi;
  uint32 arg = BOOT_API_ENTER_SDP_ON_USB;
  
  romApi = romApi_getAddr();
  romApi->bootApi(&arg);
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_sdpUartCmd(const char* argStr)
{
  libc_puts("Run SDP on UART\n");
  T_ROM_API* romApi;
  uint32 arg = BOOT_API_ENTER_SDP_ON_UART;
  
  romApi = romApi_getAddr();
  romApi->bootApi(&arg);
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_bootCmd(const char* argStr)
{
  libc_puts("Enter FBL\n");
  bmgr_exit();
  fbl_enter();
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_ldInfoCmd(const char* argStr)
{
  /* Print linker Info */
  ldr_dumpLinkerInfo();
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_cpuidCmd(const char* argStr)
{
  ocotp_dumpUid();
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_srkInfoCmd(const char* argStr)
{
  ocotp_dumpSrkHash();
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_ivtInfoCmd(const char* argStr)
{
  const void* ownIvt = hab_getOwnIvtAddr();
  libc_printf("IVT detected at: %08X\n", (uint32)ownIvt);
  hab_dumpIvt(ownIvt);
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_csfInfoCmd(const char* argStr)
{
  const void* ownCsf = hab_getOwnCsfAddr();
  libc_printf("CSF detected at: %08X\n", (uint32)ownCsf);
  hab_dumpCsf(ownCsf);
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_rvtInfoCmd(const char* argStr)
{
  hab_dumpRvt();
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_wrMacCmd(const char* argStr)
{
  ocotp_writeFuse(OCOTP_MAC0_FUSE_OFFS, 0x1);
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_wrSrkHashCmd(const char* argStr)
{
#if 0 /* EVK */
  ocotp_writeFuse(OCOTP_SRK0_FUSE_OFFS, 0xC7E27F62);
  ocotp_writeFuse(OCOTP_SRK1_FUSE_OFFS, 0x19CB69D6);
  ocotp_writeFuse(OCOTP_SRK2_FUSE_OFFS, 0x172EF506);
  ocotp_writeFuse(OCOTP_SRK3_FUSE_OFFS, 0xD958B020);
  ocotp_writeFuse(OCOTP_SRK4_FUSE_OFFS, 0x9A5D0384);
  ocotp_writeFuse(OCOTP_SRK5_FUSE_OFFS, 0xE66957AE);
  ocotp_writeFuse(OCOTP_SRK6_FUSE_OFFS, 0x471747D8);
  ocotp_writeFuse(OCOTP_SRK7_FUSE_OFFS, 0xD571E48D);
#else /* TEENSY */
  ocotp_writeFuse(OCOTP_SRK0_FUSE_OFFS, 0x357F286C);
  ocotp_writeFuse(OCOTP_SRK1_FUSE_OFFS, 0x9D03F0B5);
  ocotp_writeFuse(OCOTP_SRK2_FUSE_OFFS, 0x0FD17D0B);
  ocotp_writeFuse(OCOTP_SRK3_FUSE_OFFS, 0xBC95D1AE);
  ocotp_writeFuse(OCOTP_SRK4_FUSE_OFFS, 0xA4845637);
  ocotp_writeFuse(OCOTP_SRK5_FUSE_OFFS, 0x0F8C747F);
  ocotp_writeFuse(OCOTP_SRK6_FUSE_OFFS, 0xF6E81875);
  ocotp_writeFuse(OCOTP_SRK7_FUSE_OFFS, 0xF973F092);
#endif
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_wrSecCfgCmd(const char* argStr)
{
  uint32 fuseMask = BF_MASK(BOOT1_FUSE_SEC_CONFIG_BF);

  libc_printf("SEC_CFG: %03x=%08x\n", OCOTP_BOOT1_FUSE_OFFS, fuseMask);
  ocotp_writeFuse(OCOTP_BOOT1_FUSE_OFFS, fuseMask);
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_wrSjcRspCmd(const char* argStr)
{
  ocotp_writeFuse(OCOTP_SJC_RSP0_FUSE_OFFS, 0x00000001);
  ocotp_writeFuse(OCOTP_SJC_RSP1_FUSE_OFFS, 0x80000000);
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_wrSecJtagCmd(const char* argStr)
{
  uint32 fuseMask;

  fuseMask = ( 0
             | BF_SET(BOOT1_FUSE_JTAG_SMODE_JTAG_SEC, BOOT1_FUSE_JTAG_SMODE_BF)
             | BF_SET(1, BOOT1_FUSE_KTE_BF)
             );

  libc_printf("SEC_JTAG: %03x=%08x\n", OCOTP_BOOT1_FUSE_OFFS, fuseMask);
  ocotp_writeFuse(OCOTP_BOOT1_FUSE_OFFS, fuseMask);
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_wrDisJtagCmd(const char* argStr)
{
  uint32 fuseMask;

  fuseMask = ( 0
             | BF_SET(BOOT1_FUSE_JTAG_SMODE_JTAG_DIS, BOOT1_FUSE_JTAG_SMODE_BF)
             | BF_SET(1, BOOT1_FUSE_KTE_BF)
             );

  libc_printf("DIS_JTAG: %03x=%08x\n", OCOTP_BOOT1_FUSE_OFFS, fuseMask);
  ocotp_writeFuse(OCOTP_BOOT1_FUSE_OFFS, fuseMask);
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_wrDisSjcCmd(const char* argStr)
{
  uint32 fuseMask = BF_MASK(BOOT1_FUSE_SJC_DISABLE_BF);

  libc_printf("SJC_DISABLE: %03x=%08x\n", OCOTP_BOOT1_FUSE_OFFS, fuseMask);
  ocotp_writeFuse(OCOTP_BOOT1_FUSE_OFFS, fuseMask);
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_enaJtagCmd(const char* argStr)
{
  uint32 regMask = BF_MASK(OCOTP_SCS_HAB_JTAG_DBG_ENA_BF);

  libc_printf("ENA_JTAG: %03x=%08x\n", OCOTP_SCS_OFFS, regMask);
  REG32_WR_BASE_OFFS(regMask, OCOTP_BASE, OCOTP_SCS_OFFS);
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_authCmd(const char* argStr)
{
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_otpInfoCmd(const char* argStr)
{
  ocotp_dumpTesterFuses();
  ocotp_dumpBootFuses();
  ocotp_dumpMemFuses();
  ocotp_dumpAnalogFuses();
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_snvsInfoCmd(const char* argStr)
{
  uint32 reg;

  REG32_RD_BASE_OFFS(reg, SNVS_BASE, SNVS_HP_SEC_VIOL_CTRL_OFFS);
  libc_printf("SNVS_HPSVC:   %08x\n", reg);

  REG32_RD_BASE_OFFS(reg, SNVS_BASE, SNVS_HP_STAT_OFFS);
  libc_printf("SNVS_HPSR:    %08x\n", reg);

  REG32_RD_BASE_OFFS(reg, SNVS_BASE, SNVS_HP_SEC_VIOL_STAT_OFFS);
  libc_printf("SNVS_HPSVS:   %08x\n", reg);

  REG32_RD_BASE_OFFS(reg, SNVS_BASE, SNVS_LP_CTRL_OFFS);
  libc_printf("SNVS_LPC:     %08x\n", reg);

  REG32_RD_BASE_OFFS(reg, SNVS_BASE, SNVS_LP_MKEY_CTRL_OFFS);
  libc_printf("SNVS_LPMK:    %08x\n", reg);

  REG32_RD_BASE_OFFS(reg, SNVS_BASE, SNVS_LP_SEC_VIOL_CTRL_OFFS);
  libc_printf("SNVS_LPSVC:   %08x\n", reg);

  REG32_RD_BASE_OFFS(reg, SNVS_BASE, SNVS_LP_STAT_OFFS);
  libc_printf("SNVS_LPS:     %08x\n", reg);

  REG32_RD_BASE_OFFS(reg, SNVS_BASE, SNVS_LP_SEC_RTC_MSB_OFFS);
  libc_printf("SNVS_LPSRTCM: %08x\n", reg);

  REG32_RD_BASE_OFFS(reg, SNVS_BASE, SNVS_LP_SEC_RTC_LSB_OFFS);
  libc_printf("SNVS_LPSRTCL: %08x\n", reg);

  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_lockInfoCmd(const char* argStr)
{
  
  uint32 reg;
  uint32 tmp;

  /* */
  REG32_RD_BASE_OFFS(reg, OCOTP_BASE, OCOTP_LOCK_FUSE_OFFS);
  libc_printf("FUSE_LOCK = %08x\n", reg);

  tmp = BF_GET(reg, LOCK_FUSE_TESTER_LOCK_BF);
  libc_printf(" TESTER_LOCK:   %x\n", tmp);
  tmp = BF_GET(reg, LOCK_FUSE_BOOT_CFG_LOCK_BF);
  libc_printf(" BOOT_CFG_LOCK: %x\n", tmp);
  tmp = BF_GET(reg, LOCK_FUSE_MEM_TRIM_LOCK_BF);
  libc_printf(" MEM_TRIM_LOCK: %x\n", tmp);
  tmp = BF_GET(reg, LOCK_FUSE_SJC_RESP_LOCK_BF);
  libc_printf(" SJC_RESP_LOCK: %x\n", tmp);
  tmp = BF_GET(reg, LOCK_FUSE_GP4_RD_LOCK_BF);
  libc_printf(" GP4_RD_LOCK:   %x\n", tmp);
  tmp = BF_GET(reg, LOCK_FUSE_MAC_ADDR_LOCK_BF);
  libc_printf(" MAC_ADDR_LOCK: %x\n", tmp);

#if 0
  /* */
  REG32_RD_BASE_OFFS(reg, OCOTP_BASE, OCOTP_MAC0_FUSE_OFFS);
  libc_printf(" FUSE_MAC0 = %08X\n", reg);

  /* */
  REG32_RD_BASE_OFFS(reg, OCOTP_BASE, OCOTP_MAC1_FUSE_OFFS);
  libc_printf(" FUSE_MAC0 = %08X\n", reg);
#endif
  return 0;
}  


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_bootInfoCmd(const char* argStr)
{
  uint32 reg;
  uint32 tmp;

  /* Read Sytem Reset Controller boot mode register 2 */
  REG32_RD_BASE_OFFS(reg, SRC_BASE, SRC_SBMR2_OFFS);

  /* Get boot mode */
  tmp = BF_GET(reg, SRC_SBMR2_BMOD_BF);
  libc_printf("BOOT_MODE[1:0] = %d\n", tmp);
  
  /* Get boot fuse select */
  tmp = BF_GET(reg, SRC_SBMR2_BT_FUSE_SEL_BF);
  libc_printf("BT_FUSE_SEL = %d\n", tmp);

  /* Get direct external boot disable */
  tmp = BF_GET(reg, SRC_SBMR2_DIR_BT_DIS_BF);
  libc_printf("DIR_BT_DIS = %d\n", tmp);

  /* Get security config */
  tmp = BF_GET(reg, SRC_SBMR2_SEC_CONFIG_BF);
  libc_printf("SEC_CONFIG = %d\n", tmp);


  /* Read Sytem Reset Controller boot mode register 1 */
  REG32_RD_BASE_OFFS(reg, SRC_BASE, SRC_SBMR1_OFFS);

  tmp = BF_GET(reg, SRC_SBMR1_BOOT_CFG1_BF);
  libc_printf("BOOT_CFG1 = 0x%02X\n", tmp);

  tmp = BF_GET(reg, SRC_SBMR1_BOOT_CFG2_BF);
  libc_printf("BOOT_CFG2 = 0x%02X\n", tmp);

  tmp = BF_GET(reg, SRC_SBMR1_BOOT_CFG3_BF);
  libc_printf("BOOT_CFG3 = 0x%02X\n", tmp);

  tmp = BF_GET(reg, SRC_SBMR1_BOOT_CFG4_BF);
  libc_printf("BOOT_CFG4 = 0x%02X\n", tmp);

  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_scsInfoCmd(const char* argStr)
{
  uint32 reg;

  REG32_RD_BASE_OFFS(reg, OCOTP_BASE, OCOTP_SCS_OFFS);
  libc_printf("OCOTP_SCS:   %08x\n", reg);
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_habInfoCmd(const char* argStr)
{
  hab_dumpVersion();
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_habCheckCmd(const char* argStr)
{
  if(HAB_API_OK != hab_getStatus())
  {
    libc_puts("ROM-Boot detected HAB failue in image.\n");
//    hab_dumpStatus();
  }
  else
  {
    libc_puts("ROM-Boot ok.\n");
  }
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_habStatusCmd(const char* argStr)
{
  hab_dumpStatus();
  return 0;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_unknownCmd(const char* argStr)
{
  libc_puts("Unknown command\n");
  return cmdl_helpCmd(NULL);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static T_CLI_ENTRY cmdl_cmdTable[] =
{
  {"?", &cmdl_helpCmd},
  {"help", &cmdl_helpCmd},
  {"sdpUsb", &cmdl_sdpUsbCmd},
  {"sdpUart", &cmdl_sdpUartCmd},
  {"fblInfo", &cmdl_fblInfoCmd},
  {"appInfo", &cmdl_appInfoCmd},
  {"authApp", &cmdl_authCmd},
  {"ldInfo", &cmdl_ldInfoCmd},
  {"srkInfo", &cmdl_srkInfoCmd},
  {"bootInfo", &cmdl_bootInfoCmd},
  {"scsInfo", &cmdl_scsInfoCmd},
  {"lockInfo", &cmdl_lockInfoCmd},
  {"otpInfo", &cmdl_otpInfoCmd},
  {"ivtInfo", &cmdl_ivtInfoCmd},
  {"csfInfo", &cmdl_csfInfoCmd},
  {"rvtInfo", &cmdl_rvtInfoCmd},
  {"habInfo", &cmdl_habInfoCmd},
  {"habCheck", &cmdl_habCheckCmd},
  {"habStatus", &cmdl_habStatusCmd},
  {"boot", &cmdl_bootCmd},
  {"cpuID", &cmdl_cpuidCmd},
  {"snvsInfo", &cmdl_snvsInfoCmd},
  {"enaJtag", &cmdl_enaJtagCmd},
  {"wrMac", &cmdl_wrMacCmd},
  {"wrSrkHash", &cmdl_wrSrkHashCmd},
  {"wrSecCfg", &cmdl_wrSecCfgCmd},
  {"wrSjcRsp", &cmdl_wrSjcRspCmd},
  {"wrSecJtag", &cmdl_wrSecJtagCmd},
  {"wrDisJtag", &cmdl_wrDisJtagCmd},
  {"wrDisSjc", &cmdl_wrDisSjcCmd},
  {"\\*", &cmdl_unknownCmd},
  {NULL,   NULL},
};


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static void cmdl_pompt(void)
{
  libc_puts(CMDL_PROMPT);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static void cmdl_echo(char c)
{
  T_CMDL_DATA* cmdData = &cmdl_data;

  /* Process echo */
  if(cmdData->echoOff != 0)
  {
    /* Echo is off */
  }
  else if(c == '\r')
  {
    /* Received CR */
    libc_putchar('\n');
  }
  else if(c == 0x7F)
  {
    /* Received DEL */
    libc_puts("\b \b");
  }
  else
  {
    /* Received other charachter */
    libc_putchar(c);
  }
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void cmdl_init(void)
{
  libc_memset(&cmdl_data, 0, sizeof(cmdl_data));

  libc_registerGetcharCallback(&uart_getchar);
  libc_registerPutcharCallback(&uart_putchar);

  cmdl_pompt();
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void cmdl_run(void)
{
  T_CMDL_DATA* cmdData = &cmdl_data;
  int c;

  /* Get character from stdin */
  c = libc_getchar();
  if(c == -1)
  {
    /* Nothing received */
  }
  else
  {
    /* Received new character */

    /* Check for end of line */
    if( (c == '\r') || (c == '\n') )
    {
      const char* argStr;
      int tblPos;

      /* Send newline */
      cmdl_echo('\n');

      if(cmdData->bufPos != 0)
      {
        /* Terminate string */
        cmdData->cmdBuffer[cmdData->bufPos] = '\0';

        /* Parse received command */
        tblPos = cli_parseCmd(cmdl_cmdTable, cmdData->cmdBuffer, &argStr);

        /* Process command */
        cli_execCmd(cmdl_cmdTable, tblPos, argStr);

        cmdData->lastCmdSize = cmdData->bufPos;

        /* Reset buffer position */
        cmdData->bufPos = 0;
      }
      /* Start new command line */
      cmdl_pompt();      
    }
#if 0
    else if(c == cursor)
    {
      if(cmdData->bufPos == 0)
      {
        cmdData->bufPos = cmdData->lastCmdSize;
        libc_puts(cmdData->cmdBuffer);
      }
      else
      {
      }
    }
#endif
    else if(c == 0x7F)
    {
      /* Received DEL */
      if(cmdData->bufPos > 0)
      {
        cmdData->bufPos--;
        cmdl_echo(c);
      }
    }
    else if(cmdData->bufPos < sizeof(cmdData->cmdBuffer) - 1)
    {
      /* Received other char */
      cmdData->cmdBuffer[cmdData->bufPos++] = c;
      cmdl_echo(c);
    }
  }
}

