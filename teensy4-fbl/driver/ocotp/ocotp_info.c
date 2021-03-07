#ifndef OCOTP_INFO_C
#define OCOTP_INFO_C
#endif /* OCOTP_INFO_C */

#include "bsp.h"

#include "trace_pub.h"
#include "ocotp.h"


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void ocotp_dumpUid(void)
{
  uint32 uidLoWord;
  uint32 uidHiWord;

  REG32_RD_BASE_OFFS(uidHiWord, OCOTP_BASE, OCOTP_HW_UID_HI_FUSE_OFFS);
  REG32_RD_BASE_OFFS(uidLoWord, OCOTP_BASE, OCOTP_HW_UID_LO_FUSE_OFFS);
  TRACE_INFO("CPU UID:  %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n",
             (uint8)(uidHiWord >> 24),
             (uint8)(uidHiWord >> 16),
             (uint8)(uidHiWord >>  8),
             (uint8)(uidHiWord >>  0),
             (uint8)(uidLoWord >> 24),
             (uint8)(uidLoWord >> 16),
             (uint8)(uidLoWord >>  8),
             (uint8)(uidLoWord >>  0));
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void ocotp_dumpSrkHash(void)
{
  uint32 word;
  
  TRACE_INFO("SRK Hash:\n");
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_SRK0_FUSE_OFFS);
  TRACE_INFO(" SRK[0]=%08X\n", word);
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_SRK1_FUSE_OFFS);
  TRACE_INFO(" SRK[1]=%08X\n", word);
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_SRK2_FUSE_OFFS);
  TRACE_INFO(" SRK[2]=%08X\n", word);
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_SRK3_FUSE_OFFS);
  TRACE_INFO(" SRK[3]=%08X\n", word);
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_SRK4_FUSE_OFFS);
  TRACE_INFO(" SRK[4]=%08X\n", word);
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_SRK5_FUSE_OFFS);
  TRACE_INFO(" SRK[5]=%08X\n", word);
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_SRK6_FUSE_OFFS);
  TRACE_INFO(" SRK[6]=%08X\n", word);
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_SRK7_FUSE_OFFS);
  TRACE_INFO(" SRK[7]=%08X\n", word);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void ocotp_dumpTesterFuses(void)
{
  uint32 word;
  
  TRACE_INFO("TESTER Fuses:\n");
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_HW_UID_LO_FUSE_OFFS);
  TRACE_INFO(" TEST[0]=%08X\n", word);
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_HW_UID_HI_FUSE_OFFS);
  TRACE_INFO(" TEST[1]=%08X\n", word);
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_SI_REV_FUSE_OFFS);
  TRACE_INFO(" TEST[2]=%08X\n", word);
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_SPEED_GRADE_FUSE_OFFS);
  TRACE_INFO(" TEST[3]=%08X\n", word);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void ocotp_dumpAnalogFuses(void)
{
  uint32 word;
  
  TRACE_INFO("ANALOG Fuses:\n");
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_ANA0_FUSE_OFFS);
  TRACE_INFO(" ANA[0]=%08X\n", word);
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_ANA1_FUSE_OFFS);
  TRACE_INFO(" ANA[1]=%08X\n", word);
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_ANA2_FUSE_OFFS);
  TRACE_INFO(" ANA[2]=%08X\n", word);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void ocotp_dumpBootFuses(void)
{
  uint32 word;
  
  TRACE_INFO("BOOT Fuses:\n");
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_BOOT0_FUSE_OFFS);
  TRACE_INFO(" BOOT_FUSE[0]=%08X\n", word);
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_BOOT1_FUSE_OFFS);
  TRACE_INFO(" BOOT_FUSE[1]=%08X\n", word);
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_BOOT2_FUSE_OFFS);
  TRACE_INFO(" BOOT_FUSE[2]=%08X\n", word);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void ocotp_dumpMemFuses(void)
{
  uint32 word;
  
  TRACE_INFO("MEM Fuses:\n");
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_MEM0_FUSE_OFFS);
  TRACE_INFO(" MEM_TRIM[0]=%08X\n", word);
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_MEM1_FUSE_OFFS);
  TRACE_INFO(" MEM_TRIM[1]=%08X\n", word);
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_MEM2_FUSE_OFFS);
  TRACE_INFO(" MEM_TRIM[2]=%08X\n", word);
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_MEM3_FUSE_OFFS);
  TRACE_INFO(" MEM_TRIM[3]=%08X\n", word);
  REG32_RD_BASE_OFFS(word, OCOTP_BASE, OCOTP_MEM4_FUSE_OFFS);
  TRACE_INFO(" MEM_TRIM[4]=%08X\n", word);
}


