#ifndef IMXRT_IOMUX_H
#define IMXRT_IOMUX_H


/*
 * IOMUX General Purpose Registers
 */

#define IOMUXC_GPR0_OFFS            0x000 /*!<  */
#define IOMUXC_GPR1_OFFS            0x004 /*!<  */

#define GPR1_CM7_FORCE_HCLK_EN_BF   31, 1


#define IOMUXC_GPR2_OFFS            0x008 /*!<  */
#define IOMUXC_GPR3_OFFS            0x00C /*!<  */

#define GPR3_DCP_KEY_SEL_BF          4, 1

#define GPR3_DCP_KEY_SEL_KEY_LO      0
#define GPR3_DCP_KEY_SEL_KEY_HI      1


#define IOMUXC_GPR4_OFFS            0x010 /*!<  */
#define IOMUXC_GPR5_OFFS            0x014 /*!<  */
#define IOMUXC_GPR6_OFFS            0x018 /*!<  */
#define IOMUXC_GPR7_OFFS            0x01C /*!<  */
#define IOMUXC_GPR8_OFFS            0x020 /*!<  */
#define IOMUXC_GPR9_OFFS            0x024 /*!<  */
#define IOMUXC_GPR10_OFFS           0x028 /*!<  */

#define GPR10_DCP_KEY_SEL_BF         4, 1

#define GPR10_DCP_KEY_SEL_SNVS_KEY   0
#define GPR10_DCP_KEY_SEL_OCOTP_KEY  1


#define IOMUXC_GPR11_OFFS           0x02C /*!<  */

#define GPR11_BEE_DE_R3_EN_BF       11, 1
#define GPR11_BEE_DE_R2_EN_BF       10, 1
#define GPR11_BEE_DE_R1_EN_BF        9, 1
#define GPR11_BEE_DE_R0_EN_BF        8, 1

#define GPR11_BEE_ENCRYPT_Rx_DIS     0
#define GPR11_BEE_ENCRYPT_Rx_ENA     1

#define GPR11_APC_AP_R3_CTRL_BF      6, 2
#define GPR11_APC_AP_R2_CTRL_BF      4, 2
#define GPR11_APC_AP_R1_CTRL_BF      2, 2
#define GPR11_APC_AP_R0_CTRL_BF      0, 2

#define GPR11_APC_AP_Rx_NO_PROT      0
#define GPR11_APC_AP_Rx_DBG_PROT     1

#define IOMUXC_GPR12_OFFS           0x030 /*!<  */
#define IOMUXC_GPR13_OFFS           0x034 /*!<  */
#define IOMUXC_GPR14_OFFS           0x038 /*!<  */
#define IOMUXC_GPR15_OFFS           0x03C /*!<  */
#define IOMUXC_GPR16_OFFS           0x040 /*!<  */
#define IOMUXC_GPR17_OFFS           0x044 /*!<  */
#define IOMUXC_GPR18_OFFS           0x048 /*!<  */
#define IOMUXC_GPR19_OFFS           0x04C /*!<  */
#define IOMUXC_GPR20_OFFS           0x050 /*!<  */
#define IOMUXC_GPR21_OFFS           0x054 /*!<  */
#define IOMUXC_GPR22_OFFS           0x058 /*!<  */
#define IOMUXC_GPR23_OFFS           0x05C /*!<  */
#define IOMUXC_GPR24_OFFS           0x060 /*!<  */
#define IOMUXC_GPR25_OFFS           0x064 /*!<  */
#define IOMUXC_GPR26_OFFS           0x068 /*!<  */
#define IOMUXC_GPR27_OFFS           0x06C /*!<  */
#define IOMUXC_GPR28_OFFS           0x070 /*!<  */
#define IOMUXC_GPR29_OFFS           0x074 /*!<  */
#define IOMUXC_GPR30_OFFS           0x078 /*!<  */
#define IOMUXC_GPR31_OFFS           0x07C /*!<  */
#define IOMUXC_GPR32_OFFS           0x080 /*!<  */
#define IOMUXC_GPR33_OFFS           0x084 /*!<  */
#define IOMUXC_GPR34_OFFS           0x088 /*!<  */




/*
 * IOMUX Multiplexer Configuration Registers
 */

/* Bit-field definitions
 *
 * |31       5|   4  |     3    |2        0|
 * +----------+------+----------+----------+
 * | reserved | SION | reserved | MUX_MODE |
 * +----------+------+----------+----------+
 * |<-- 27 -->|<- 1 >|<--- 1 -->|<--- 3 -->|
 *
 */

#define MUXCONF_SION_BF              4, 1
#define MUXCONF_MUX_MODE_BF          0, 3


#define MUXCONF_MIN_OFFS             0x014

#define MUXCONF_GPIO_EMC_00_OFFS     0x014 /*!<  */

/* Aliases */
#define MUXCONF_LPSPI2_SCK__GPIO_EMC_00_ALT2_OFFS MUXCONF_GPIO_EMC_00_OFFS


#define MUXCONF_GPIO_EMC_01_OFFS     0x018 /*!<  */

/* Aliases */
#define MUXCONF_LPSPI2_nCS0__GPIO_EMC_01_ALT2_OFFS MUXCONF_GPIO_EMC_01_OFFS


#define MUXCONF_GPIO_EMC_02_OFFS     0x01C /*!<  */

/* Aliases */
#define MUXCONF_LPSPI2_MOSI__GPIO_EMC_02_ALT2_OFFS MUXCONF_GPIO_EMC_02_OFFS


#define MUXCONF_GPIO_EMC_03_OFFS     0x020 /*!<  */

/* Aliases */
#define MUXCONF_LPSPI2_MISO__GPIO_EMC_03_ALT2_OFFS MUXCONF_GPIO_EMC_03_OFFS


#define MUXCONF_GPIO_EMC_04_OFFS     0x024 /*!<  */
#define MUXCONF_GPIO_EMC_05_OFFS     0x028 /*!<  */
#define MUXCONF_GPIO_EMC_06_OFFS     0x02C /*!<  */
/* ... */
#define MUXCONF_GPIO_EMC_14_OFFS     0x04C /*!<  */

/* Aliases */
#define MUXCONF_LPSPI2_nCS1__GPIO_EMC_14_ALT4_OFFS MUXCONF_GPIO_EMC_14_OFFS


#define MUXCONF_GPIO_EMC_27_OFFS     0x080 /*!<  */

/* Aliases */
#define MUXCONF_LPSPI1_SCK__GPIO_EMC_27_ALT3_OFFS MUXCONF_GPIO_EMC_27_OFFS


#define MUXCONF_GPIO_EMC_28_OFFS     0x084 /*!<  */

/* Aliases */
#define MUXCONF_LPSPI1_MOSI__GPIO_EMC_28_ALT3_OFFS MUXCONF_GPIO_EMC_28_OFFS


#define MUXCONF_GPIO_EMC_29_OFFS     0x088 /*!<  */

/* Aliases */
#define MUXCONF_LPSPI1_MISO__GPIO_EMC_29_ALT3_OFFS MUXCONF_GPIO_EMC_29_OFFS


#define MUXCONF_GPIO_EMC_30_OFFS     0x08C /*!<  */

/* Aliases */
#define MUXCONF_LPSPI1_nCS0__GPIO_EMC_30_ALT3_OFFS MUXCONF_GPIO_EMC_30_OFFS


#define MUXCONF_GPIO_EMC_31_OFFS     0x090 /*!<  */

/* Aliases */
#define MUXCONF_LPSPI1_nCS1__GPIO_EMC_31_ALT3_OFFS MUXCONF_GPIO_EMC_31_OFFS


#define MUXCONF_GPIO_EMC_40_OFFS     0x0B4 /*!<  */

/* Aliases */
#define MUXCONF_LPSPI1_nCS2__GPIO_EMC_40_ALT2_OFFS MUXCONF_GPIO_EMC_40_OFFS


#define MUXCONF_GPIO_EMC_41_OFFS     0x0B8 /*!<  */

/* Aliases */
#define MUXCONF_LPSPI1_nCS3__GPIO_EMC_41_ALT2_OFFS MUXCONF_GPIO_EMC_41_OFFS


#define MUXCONF_GPIO_AD_B0_00_OFFS   0x0BC /*!<  */
#define MUXCONF_GPIO_AD_B0_01_OFFS   0x0C0 /*!<  */
#define MUXCONF_GPIO_AD_B0_02_OFFS   0x0C4 /*!<  */
#define MUXCONF_GPIO_AD_B0_03_OFFS   0x0C8 /*!<  */
#define MUXCONF_GPIO_AD_B0_04_OFFS   0x0CC /*!<  */
/* ... */
#define MUXCONF_GPIO_AD_B0_05_OFFS   0x0D0 /*!<  */
#define MUXCONF_GPIO_AD_B0_06_OFFS   0x0D4 /*!<  */
#define MUXCONF_GPIO_AD_B0_07_OFFS   0x0D8 /*!<  */
#define MUXCONF_GPIO_AD_B0_08_OFFS   0x0DC /*!<  */
#define MUXCONF_GPIO_AD_B0_09_OFFS   0x0E0 /*!<  */
#define MUXCONF_GPIO_AD_B0_10_OFFS   0x0E4 /*!<  */
#define MUXCONF_GPIO_AD_B0_11_OFFS   0x0E8 /*!<  */

/* Aliases */
#define MUXCONF_GPIO1_IO11__GPIO_AD_B0_11_ALT5_OFFS      MUXCONF_GPIO_AD_B0_11_OFFS

#define MUXCONF_GPIO_AD_B0_12_OFFS   0x0EC /*!<  */

/* Aliases */
#define MUXCONF_LPUART1_TX__GPIO_AD_B0_12_ALT2_OFFS      MUXCONF_GPIO_AD_B0_12_OFFS

#define MUXCONF_GPIO_AD_B0_13_OFFS   0x0F0 /*!<  */

/* Aliases */
#define MUXCONF_LPUART1_RX__GPIO_AD_B0_13_ALT2_OFFS      MUXCONF_GPIO_AD_B0_13_OFFS

#define MUXCONF_GPIO_AD_B0_14_OFFS   0x0F4 /*!<  */
#define MUXCONF_GPIO_AD_B0_15_OFFS   0x0F8 /*!<  */

#define MUXCONF_GPIO_AD_B1_00_OFFS   0x0FC /*!<  */
#define MUXCONF_GPIO_AD_B1_01_OFFS   0x100 /*!<  */
#define MUXCONF_GPIO_AD_B1_02_OFFS   0x104 /*!<  */
#define MUXCONF_GPIO_AD_B1_03_OFFS   0x108 /*!<  */
#define MUXCONF_GPIO_AD_B1_04_OFFS   0x10C /*!<  */
#define MUXCONF_GPIO_AD_B1_05_OFFS   0x110 /*!<  */

#define MUXCONF_GPIO_AD_B1_06_OFFS   0x114 /*!<  */

/* Aliases */
#define MUXCONF_FLEXSPIB_DATA01__GPIO_AD_B1_06_ALT0_OFFS MUXCONF_GPIO_AD_B1_06_OFFS
#define MUXCONF_LPI2C3_SDA__GPIO_AD_B1_06_ALT1_OFFS      MUXCONF_GPIO_AD_B1_06_OFFS
#define MUXCONF_LPUART3_TX__GPIO_AD_B1_06_ALT2_OFFS      MUXCONF_GPIO_AD_B1_06_OFFS
#define MUXCONF_SPDIF_LOCK__GPIO_AD_B1_06_ALT3_OFFS      MUXCONF_GPIO_AD_B1_06_OFFS
#define MUXCONF_CSI_VSYNC__GPIO_AD_B1_06_ALT4_OFFS       MUXCONF_GPIO_AD_B1_06_OFFS
#define MUXCONF_GPIO1_IO22__GPIO_AD_B1_06_ALT5_OFFS      MUXCONF_GPIO_AD_B1_06_OFFS
#define MUXCONF_USDHC_DATA2__GPIO_AD_B1_06_ALT6_OFFS     MUXCONF_GPIO_AD_B1_06_OFFS
#define MUXCONF_KPP_ROW04__GPIO_AD_B1_06_ALT7_OFFS       MUXCONF_GPIO_AD_B1_06_OFFS
#define MUXCONF_GPT2_COMP2__GPIO_AD_B1_06_ALT8_OFFS      MUXCONF_GPIO_AD_B1_06_OFFS
#define MUXCONF_FLEXIO3_IO06__GPIO_AD_B1_06_ALT9_OFFS    MUXCONF_GPIO_AD_B1_06_OFFS

#define MUXCONF_GPIO_AD_B1_07_OFFS   0x118 /*!<  */

/* Aliases */
#define MUXCONF_FLEXSPIB_DATA00__GPIO_AD_B1_07_ALT0_OFFS MUXCONF_GPIO_AD_B1_07_OFFS
#define MUXCONF_LPI2C3_SCL__GPIO_AD_B1_07_ALT1_OFFS      MUXCONF_GPIO_AD_B1_07_OFFS
#define MUXCONF_LPUART3_RX__GPIO_AD_B1_07_ALT2_OFFS      MUXCONF_GPIO_AD_B1_07_OFFS
#define MUXCONF_SPDIF_EXT_CLK__GPIO_AD_B1_07_ALT3_OFFS   MUXCONF_GPIO_AD_B1_07_OFFS
#define MUXCONF_CSI_HSYNC__GPIO_AD_B1_07_ALT4_OFFS       MUXCONF_GPIO_AD_B1_07_OFFS
#define MUXCONF_GPIO1_IO23__GPIO_AD_B1_07_ALT5_OFFS      MUXCONF_GPIO_AD_B1_07_OFFS
#define MUXCONF_USDHC2_DATA3__GPIO_AD_B1_07_ALT6_OFFS    MUXCONF_GPIO_AD_B1_07_OFFS
#define MUXCONF_GPT2_COMP3__GPIO_AD_B1_07_ALT7_OFFS      MUXCONF_GPIO_AD_B1_07_OFFS
#define MUXCONF_FLEXIO3_IO07__GPIO_AD_B1_07_ALT8_OFFS    MUXCONF_GPIO_AD_B1_07_OFFS


/* ... */
#define MUXCONF_GPIO_AD_B1_15_OFFS   0x138 /*!<  */

#define MUXCONF_GPIO_B0_00_OFFS      0x13C /*!<  */

/* Aliases */
#define MUXCONF_LPSPI4_nCS0__GPIO_B0_00_ALT3_OFFS  MUXCONF_GPIO_B0_00_OFFS


#define MUXCONF_GPIO_B0_01_OFFS      0x140 /*!<  */

/* Aliases */
#define MUXCONF_LPSPI4_MISO__GPIO_B0_01_ALT3_OFFS  MUXCONF_GPIO_B0_01_OFFS


#define MUXCONF_GPIO_B0_02_OFFS      0x144 /*!<  */

/* Aliases */
#define MUXCONF_LPSPI4_MOSI__GPIO_B0_02_ALT3_OFFS  MUXCONF_GPIO_B0_02_OFFS


#define MUXCONF_GPIO_B0_03_OFFS      0x148 /*!<  */

/* Aliases */
#define MUXCONF_LCD_VSYNC__GPIO_B0_03_ALT0_OFFS  MUXCONF_GPIO_B0_03_OFFS
#define MUXCONF_LPSPI4_SCK__GPIO_B0_03_ALT3_OFFS  MUXCONF_GPIO_B0_03_OFFS
#define MUXCONF_GPIO2_IO03__GPIO_B0_03_ALT5_OFFS MUXCONF_GPIO_B0_03_OFFS

#define MUXCONF_GPIO_B0_04_OFFS      0x14C /*!<  */
/* ... */
#define MUXCONF_GPIO_B1_15_OFFS      0x1B8 /*!<  */


#define MUXCONF_GPIO_SD_B0_00_OFFS   0x1BC /*!<  */

/* Aliases */
#define MUXCONF_USDHC1_CMD__GPIO_SD_B0_00_ALT0_OFFS  MUXCONF_GPIO_SD_B0_00_OFFS
#define MUXCONF_LPSPI1_SCK__GPIO_SD_B0_00_ALT4_OFFS  MUXCONF_GPIO_SD_B0_00_OFFS

#define MUXCONF_GPIO_SD_B0_01_OFFS   0x1C0 /*!<  */

/* Aliases */
#define MUXCONF_LPSPI1_nCS0__GPIO_SD_B0_01_ALT4_OFFS  MUXCONF_GPIO_SD_B0_01_OFFS


#define MUXCONF_GPIO_SD_B0_02_OFFS   0x1C4 /*!<  */

/* Aliases */
#define MUXCONF_LPSPI1_MOSI__GPIO_SD_B0_02_ALT4_OFFS  MUXCONF_GPIO_SD_B0_02_OFFS


#define MUXCONF_GPIO_SD_B0_03_OFFS   0x1C8 /*!<  */

/* Aliases */
#define MUXCONF_LPSPI1_MISO__GPIO_SD_B0_03_ALT4_OFFS  MUXCONF_GPIO_SD_B0_03_OFFS



/*
 * IOMUX Pad configuration registers
 */

 
/* Bit-field definitions
 *
 * |31 23|  22 |21 20|19     18|    17   |  16 |15 14|  13 |  12 |  11 |10  8|7     6|5   3|2   1|  0  |
 * +-----+-----+-----+---------+---------+-----+-----+-----+-----+-----+-----+-------+-----+-----+-----+
 * |/////|/////|/////| /////// | /////// | HYS | PUS | PUE | PKE | ODE | /// | SPEED | DSE |/////| SRE |
 * +-----+-----+-----+---------+---------+-----+-----+-----+-----+-----+-----+-------+-----+-----+-----+
 * |< 9 >|< 1 >|< 2 >|<---2--->|<---1--->|<-1->|<-2->|<-1->|<-1->|<-1->|<-2->|<--2-->|<-3->|<-2->|<-1->|
 */

#define PADCONF_DDR_SEL_BF          18, 2
#define PADCONF_RGMII_SEL_BF        18, 2
#define PADCONF_DDR_INPUT_BF        17, 1
#define PADCONF_HYS_BF              16, 1
#define PADCONF_PUS_BF              14, 2
#define PADCONF_PUE_BF              13, 1
#define PADCONF_PKE_BF              12, 1
#define PADCONF_ODE_BF              11, 1
#define PADCONF_ODT_BF               8, 3
#define PADCONF_SPEED_BF             6, 2
#define PADCONF_DSE_BF               3, 3
#define PADCONF_SRE_BF               0, 1


#if !defined(__ASSEMBLER__)
enum
{
  PADCONF_eHYS_CMOS = 0,
  PADCONF_eHYS_SCHMITT = 1,
};

enum
{
  PADCONF_eHYS_DIS = 0,
  PADCONF_eHYS_ENA = 1,
};

enum
{
  PADCONF_ePUS_PD_100K = 0,
  PADCONF_ePUS_PU_047K = 1,
  PADCONF_ePUS_PU_100K = 2,
  PADCONF_ePUS_PU_022K = 3,
};

enum
{
  PADCONF_ePUE_KEEP = 0,
  PADCONF_ePUE_PULL = 1,
};

enum
{
  PADCONF_ePKE_DIS = 0,
  PADCONF_ePKE_ENA = 1,
};

enum
{
  PADCONF_eODE_CMOS = 0,
  PADCONF_eODE_OPEN_DRAIN = 1,
};

enum
{
  PADCONF_eODE_DIS = 0,
  PADCONF_eODE_ENA = 1,
};

enum
{
  PADCONF_eSPEED_050MHZ = 0,
  PADCONF_eSPEED_100MHZ = 1,
  PADCONF_eSPEED_200MHZ = 3,
};

enum
{
  PADCONF_eDSE_HIZ = 0,
  PADCONF_eDSE_R0_BY_1 = 1, /* 150R @ 3V3 / 260R @ 1V8 */
  PADCONF_eDSE_R0_BY_2 = 2,
  PADCONF_eDSE_R0_BY_3 = 3,
  PADCONF_eDSE_R0_BY_4 = 4,
  PADCONF_eDSE_R0_BY_5 = 5,
  PADCONF_eDSE_R0_BY_6 = 6,
  PADCONF_eDSE_R0_BY_7 = 7,
};

enum
{
  PADCONF_eSRE_SLOW = 0,
  PADCONF_eSRE_FAST = 1,
};
#endif /* !defined(__ASSEMBLER__) */


#define PADCONF_MIN_OFFS             0x204

#define PADCONF_GPIO_EMC_00_OFFS     0x204 /*!<  */
/* ... */
#define PADCONF_GPIO_EMC_41_OFFS     0x2A8 /*!<  */

#define PADCONF_GPIO_AD_B0_00_OFFS   0x2AC /*!<  */
#define PADCONF_GPIO_AD_B0_01_OFFS   0x2B0 /*!<  */

#define PADCONF_GPIO_AD_B0_02_OFFS   0x2B4 /*!<  */
#define PADCONF_GPIO_AD_B0_03_OFFS   0x2B8 /*!<  */
#define PADCONF_GPIO_AD_B0_04_OFFS   0x2BC /*!<  */
/* ... */
#define PADCONF_GPIO_AD_B0_05_OFFS   0x2C0 /*!<  */
#define PADCONF_GPIO_AD_B0_06_OFFS   0x2C4 /*!<  */
#define PADCONF_GPIO_AD_B0_07_OFFS   0x2C9 /*!<  */
#define PADCONF_GPIO_AD_B0_08_OFFS   0x2CC /*!<  */
#define PADCONF_GPIO_AD_B0_09_OFFS   0x2D0 /*!<  */
#define PADCONF_GPIO_AD_B0_10_OFFS   0x2D4 /*!<  */
#define PADCONF_GPIO_AD_B0_11_OFFS   0x2D8 /*!<  */

/* Aliases */
#define PADCONF_GPIO1_IO11__GPIO_AD_B0_11_ALT5_OFFS      PADCONF_GPIO_AD_B0_11_OFFS

#define PADCONF_GPIO_AD_B0_12_OFFS   0x2DC /*!<  */

/* Aliases */
#define PADCONF_LPUART1_TX__GPIO_AD_B0_12_ALT2_OFFS      PADCONF_GPIO_AD_B0_12_OFFS

#define PADCONF_GPIO_AD_B0_13_OFFS   0x2E0 /*!<  */

/* Aliases */
#define PADCONF_LPUART1_RX__GPIO_AD_B0_13_ALT2_OFFS      PADCONF_GPIO_AD_B0_13_OFFS

#define PADCONF_GPIO_AD_B0_14_OFFS   0x2E4 /*!<  */
#define PADCONF_GPIO_AD_B0_15_OFFS   0x2E8 /*!<  */

#define PADCONF_GPIO_AD_B1_06_OFFS   0x304

/* Aliases */
#define PADCONF_FLEXSPIB_DATA01__GPIO_AD_B1_06_ALT0_OFFS PADCONF_GPIO_AD_B1_06_OFFS
#define PADCONF_LPI2C3_SDA__GPIO_AD_B1_06_ALT1_OFFS      PADCONF_GPIO_AD_B1_06_OFFS
#define PADCONF_LPUART3_TX__GPIO_AD_B1_06_ALT2_OFFS      PADCONF_GPIO_AD_B1_06_OFFS
#define PADCONF_SPDIF_LOCK__GPIO_AD_B1_06_ALT3_OFFS      PADCONF_GPIO_AD_B1_06_OFFS
#define PADCONF_CSI_VSYNC__GPIO_AD_B1_06_ALT4_OFFS       PADCONF_GPIO_AD_B1_06_OFFS
#define PADCONF_GPIO1_IO22__GPIO_AD_B1_06_ALT5_OFFS      PADCONF_GPIO_AD_B1_06_OFFS
#define PADCONF_USDHC_DATA2__GPIO_AD_B1_06_ALT6_OFFS     PADCONF_GPIO_AD_B1_06_OFFS
#define PADCONF_KPP_ROW04__GPIO_AD_B1_06_ALT7_OFFS       PADCONF_GPIO_AD_B1_06_OFFS
#define PADCONF_GPT2_COMP2__GPIO_AD_B1_06_ALT8_OFFS      PADCONF_GPIO_AD_B1_06_OFFS
#define PADCONF_FLEXIO3_IO06__GPIO_AD_B1_06_ALT9_OFFS    PADCONF_GPIO_AD_B1_06_OFFS

#define PADCONF_GPIO_AD_B1_07_OFFS   0x308

/* Aliases */
#define PADCONF_FLEXSPIB_DATA00__GPIO_AD_B1_07_ALT0_OFFS PADCONF_GPIO_AD_B1_07_OFFS
#define PADCONF_LPI2C3_SCL__GPIO_AD_B1_07_ALT1_OFFS      PADCONF_GPIO_AD_B1_07_OFFS
#define PADCONF_LPUART3_RX__GPIO_AD_B1_07_ALT2_OFFS      PADCONF_GPIO_AD_B1_07_OFFS
#define PADCONF_SPDIF_EXT_CLK__GPIO_AD_B1_07_ALT3_OFFS   PADCONF_GPIO_AD_B1_07_OFFS
#define PADCONF_CSI_HSYNC__GPIO_AD_B1_07_ALT4_OFFS       PADCONF_GPIO_AD_B1_07_OFFS
#define PADCONF_GPIO1_IO23__GPIO_AD_B1_07_ALT5_OFFS      PADCONF_GPIO_AD_B1_07_OFFS
#define PADCONF_USDHC2_DATA3__GPIO_AD_B1_07_ALT6_OFFS    PADCONF_GPIO_AD_B1_07_OFFS
#define PADCONF_GPT2_COMP3__GPIO_AD_B1_07_ALT7_OFFS      PADCONF_GPIO_AD_B1_07_OFFS
#define PADCONF_FLEXIO3_IO07__GPIO_AD_B1_07_ALT8_OFFS    PADCONF_GPIO_AD_B1_07_OFFS


#define PADCONF_GPIO_B0_00_OFFS      0x32C /*!<  */

/* Aliases */
#define PADCONF_LPSPI4_nCS0__GPIO_B0_00_ALT3_OFFS PADCONF_GPIO_B0_00_OFFS

#define PADCONF_GPIO_B0_01_OFFS      0x330 /*!<  */
/* Aliases */
#define PADCONF_LPSPI4_MISO__GPIO_B0_01_ALT3_OFFS PADCONF_GPIO_B0_01_OFFS


#define PADCONF_GPIO_B0_02_OFFS      0x334 /*!<  */
/* Aliases */
#define PADCONF_LPSPI4_MOSI__GPIO_B0_02_ALT3_OFFS PADCONF_GPIO_B0_02_OFFS


#define PADCONF_GPIO_B0_03_OFFS      0x338 /*!<  */

/* Aliases */
#define PADCONF_LCD_VSYNC__GPIO_B0_03_ALT0_OFFS  PADCONF_GPIO_B0_03_OFFS
#define PADCONF_LPSPI4_SCK__GPIO_B0_03_ALT3_OFFS PADCONF_GPIO_B0_03_OFFS
#define PADCONF_GPIO2_IO03__GPIO_B0_03_ALT5_OFFS PADCONF_GPIO_B0_03_OFFS

#define PADCONF_GPIO_B0_04_OFFS      0x33C /*!<  */
/* ... */

#define PADCONF_GPIO_B1_15_OFFS      0x368 /*!<  */

#define PADCONF_GPIO_SD_B0_00_OFFS   0x3AC /*!<  */

/* Aliases */
#define PADCONF_LPSPI1_SCK__GPIO_SD_B0_00_ALT4_OFFS PADCONF_GPIO_SD_B0_00_OFFS


#define PADCONF_GPIO_SD_B0_01_OFFS   0x3B0 /*!<  */

/* Aliases */
#define PADCONF_LPSPI1_nCS0__GPIO_SD_B0_01_ALT4_OFFS PADCONF_GPIO_SD_B0_01_OFFS


#define PADCONF_GPIO_SD_B0_02_OFFS   0x3B4 /*!<  */

/* Aliases */
#define PADCONF_LPSPI1_MOSI__GPIO_SD_B0_02_ALT4_OFFS PADCONF_GPIO_SD_B0_02_OFFS


#define PADCONF_GPIO_SD_B0_03_OFFS   0x3B8 /*!<  */

/* Aliases */
#define PADCONF_LPSPI1_MISO__GPIO_SD_B0_03_ALT4_OFFS PADCONF_GPIO_SD_B0_03_OFFS


#define INPCONF_LPSPI1_nCS0_OFFS          0x4EC
#define INPCONF_LPSPI1_SCK_OFFS           0x4F0
#define INPCONF_LPSPI1_MISO_OFFS          0x4F4
#define INPCONF_LPSPI1_MOSI_OFFS          0x4F8

#define INPCONF_LPSPI4_nCS0_OFFS          0x51C
#define INPCONF_LPSPI4_SCK_OFFS           0x520
#define INPCONF_LPSPI4_MISO_OFFS          0x524
#define INPCONF_LPSPI4_MOSI_OFFS          0x528

#define INPCONF_GPIO_EMC_15_ALT2_OFFS     0x534
#define INPCONF_GPIO_AD_B1_04_ALT2_OFFS   0x534


#define INPCONF_LPUART3_RX_OFFS           0x538

#endif /* IMXRT_IOMUX_H */

