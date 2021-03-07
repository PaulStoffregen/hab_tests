#ifndef IMXRT_CCM_H
#define IMXRT_CCM_H


/*
 * Clock Control Module (Analog Part)
 */

/* Analog ARM PLL Control Register */
#define CCM_ANALOG_PLL_ARM_OFFS          0x000 /*!<  */
#define CCM_ANALOG_PLL_ARM_SET_OFFS      0x004 /*!<  */
#define CCM_ANALOG_PLL_ARM_CLR_OFFS      0x008 /*!<  */
#define CCM_ANALOG_PLL_ARM_TOG_OFFS      0x00C /*!<  */

#define PLL_LOCK_BF                      31, 1
#define PLL_BYPASS_BF                    16, 1
#define PLL_BYPASS_CLK_SRC_BF            14, 2
   
#if !defined(__ASSEMBLER__)
   enum
   {
     eREF_CLK_24M = 0,
     eCLK1        = 1,
   };
#endif /* defined(__ASSEMBLER__) */

#define PLL_ENABLE_BF                    13, 1

#define ARM_PLL_POWERDOWN_BF             12, 1
#define ARM_PLL_DIV_SELECT_BF             0, 7
   
   
#define CCM_ANALOG_PLL_USB1_OFFS         0x010 /*!<  */
#define CCM_ANALOG_PLL_USB1_SET_OFFS     0x014 /*!<  */
#define CCM_ANALOG_PLL_USB1_CLR_OFFS     0x018 /*!<  */
#define CCM_ANALOG_PLL_USB1_TOG_OFFS     0x01C /*!<  */
   
#define USB1_PLL_POWER                   12, 1
#define USB1_PLL_EN_USB_CLKS              6, 1
#define USB1_PLL_DIV_SELECT               1, 1
   
   
#define CCM_ANALOG_PLL_USB2_OFFS         0x020 /*!<  */
#define CCM_ANALOG_PLL_USB2_SET_OFFS     0x024 /*!<  */
#define CCM_ANALOG_PLL_USB2_CLR_OFFS     0x028 /*!<  */
#define CCM_ANALOG_PLL_USB2_TOG_OFFS     0x02C /*!<  */
   
#define USB2_PLL_POWER                   12, 1
#define USB2_PLL_EN_USB_CLKS              6, 1
#define USB2_PLL_DIV_SELECT               1, 1
   
   
#define CCM_ANALOG_PLL_SYS_OFFS          0x030 /*!<  */
#define CCM_ANALOG_PLL_SYS_SET_OFFS      0x034 /*!<  */
#define CCM_ANALOG_PLL_SYS_CLR_OFFS      0x038 /*!<  */
#define CCM_ANALOG_PLL_SYS_TOG_OFFS      0x03C /*!<  */
   
#define SYS_PLL_PFD_OFFSET_EN            18, 1
#define SYS_PLL_POWERDOWN                12, 1
#define SYS_PLL_DIV_SELECT                0, 1
   
   
#define CCM_ANALOG_PLL_SYS_SS_OFFS       0x040 /*!<  */
   
#define SYS_PLL_SS_STOP                  16, 16
#define SYS_PLL_SS_ENABLE                15,  1
#define SYS_PLL_SS_STEP                   0, 15
   
   
#define CCM_ANALOG_PLL_SYS_NUM_OFFS      0x050 /*!<  */
   
#define SYS_PLL_NUM                       0, 30
   
   
#define CCM_ANALOG_PLL_SYS_DENOM_OFFS    0x060 /*!<  */
   
#define SYS_PLL_DENOM                     0, 30
   
   
#define CCM_ANALOG_PLL_AUDIO_OFFS        0x070 /*!<  */
#define CCM_ANALOG_PLL_AUDIO_SET_OFFS    0x074 /*!<  */
#define CCM_ANALOG_PLL_AUDIO_CLR_OFFS    0x078 /*!<  */
#define CCM_ANALOG_PLL_AUDIO_TOG_OFFS    0x07C /*!<  */
   
#define AUDIO_PLL_POST_DIV_SELECT        19, 2
   
#if !defined(__ASSEMBLER__)
   enum
   {
     ePOST_DIV_4 = 0,
     ePOST_DIV_2 = 1,
     ePOST_DIV_1 = 2,
   };
#endif /* !defined(__ASSEMBLER__) */
   
#define AUDIO_PLL_POWERDOWN              12, 1
#define AUDIO_PLL_DIV_SELECT              0, 7
   
   
#define CCM_ANALOG_PLL_AUDIO_NUM_OFFS    0x080 /*!<  */
   
#define AUDIO_PLL_NUM                     0, 30
   
   
#define CCM_ANALOG_PLL_AUDIO_DENOM_OFFS  0x090 /*!<  */
   
#define AUDIO_PLL_DENOM                   0, 30
   
   
#define CCM_ANALOG_PLL_VIDEO_OFFS        0x0A0 /*!<  */
#define CCM_ANALOG_PLL_VIDEO_SET_OFFS    0x0A4 /*!<  */
#define CCM_ANALOG_PLL_VIDEO_CLR_OFFS    0x0A8 /*!<  */
#define CCM_ANALOG_PLL_VIDEO_TOG_OFFS    0x0AC /*!<  */
   
#define VIDEO_PLL_SSC_EN                 21, 1
#define VIDEO_PLL_POST_DIV_SELECT        19, 2
#define VIDEO_PLL_POWERDOWN              12, 1
#define VIDEO_PLL_DIV_SELECT              0, 7
   
   
#define CCM_ANALOG_PLL_VIDEO_NUM_OFFS    0x0B0 /*!<  */
   
#define VIDEO_PLL_NUM                     0, 30
   
   
#define CCM_ANALOG_PLL_VIDEO_DENOM_OFFS  0x0C0 /*!<  */
   
#define VIDEO_PLL_DENOM                   0, 30
   
   
#define CCM_ANALOG_PLL_ENET_OFFS         0x0E0 /*!<  */
#define CCM_ANALOG_PLL_ENET_SET_OFFS     0x0E4 /*!<  */
#define CCM_ANALOG_PLL_ENET_CLR_OFFS     0x0E8 /*!<  */
#define CCM_ANALOG_PLL_ENEt_TOG_OFFS     0x0EC /*!<  */
   
#define ENET_PLL_ENABLE_100M             20, 1
#define ENET_PLL_ENABLE_125M             19, 1
#define ENET_PLL_PFD_OFFSET_EN           18, 1
#define ENET_PLL_POWERDOWN               12, 1
#define ENET_PLL_DIV_SELECT               0, 2
   
   
#define CCM_ANALOG_PLL_PFD_480_OFFS      0x0F0 /*!<  */
#define CCM_ANALOG_PLL_PFD_480_SET_OFFS  0x0F4 /*!<  */
#define CCM_ANALOG_PLL_PFD_480_CLR_OFFS  0x0F8 /*!<  */
#define CCM_ANALOG_PLL_PFD_480_TOG_OFFS  0x0FC /*!<  */
   
#define PFD_480_PFD3_CLKGATE             31, 1
#define PFD_480_PFD3_STABLE              30, 1
#define PFD_480_PFD3_FRAC                24, 6
   
#define PFD_480_PFD2_CLKGATE             23, 1
#define PFD_480_PFD2_STABLE              22, 1
#define PFD_480_PFD2_FRAC                16, 6
   
#define PFD_480_PFD1_CLKGATE             15, 1
#define PFD_480_PFD1_STABLE              14, 1
#define PFD_480_PFD1_FRAC                 8, 6
   
#define PFD_480_PFD0_CLKGATE              7, 1
#define PFD_480_PFD0_STABLE               6, 1
#define PFD_480_PFD0_FRAC                 0, 6
   

#define CCM_ANALOG_PLL_PFD0_480_OFFS     0x0F0 /*!<  */
#define CCM_ANALOG_PLL_PFD0_480_SET_OFFS 0x0F4 /*!<  */
#define CCM_ANALOG_PLL_PFD0_480_CLR_OFFS 0x0F8 /*!<  */
#define CCM_ANALOG_PLL_PFD0_480_TOG_OFFS 0x0FC /*!<  */
   
#define CCM_ANALOG_PLL_PFD1_480_OFFS     0x0F1 /*!<  */
#define CCM_ANALOG_PLL_PFD1_480_SET_OFFS 0x0F5 /*!<  */
#define CCM_ANALOG_PLL_PFD1_480_CLR_OFFS 0x0F9 /*!<  */
#define CCM_ANALOG_PLL_PFD1_480_TOG_OFFS 0x0FD /*!<  */
   
#define CCM_ANALOG_PLL_PFD2_480_OFFS     0x0F2 /*!<  */
#define CCM_ANALOG_PLL_PFD2_480_SET_OFFS 0x0F6 /*!<  */
#define CCM_ANALOG_PLL_PFD2_480_CLR_OFFS 0x0FA /*!<  */
#define CCM_ANALOG_PLL_PFD2_480_TOG_OFFS 0x0FE /*!<  */
   
#define CCM_ANALOG_PLL_PFD3_480_OFFS     0x0F3 /*!<  */
#define CCM_ANALOG_PLL_PFD3_480_SET_OFFS 0x0F7 /*!<  */
#define CCM_ANALOG_PLL_PFD3_480_CLR_OFFS 0x0FB /*!<  */
#define CCM_ANALOG_PLL_PFD3_480_TOG_OFFS 0x0FF /*!<  */
   
#define PFD_480_PFDn_CLKGATE              7, 1
#define PFD_480_PFDn_STABLE               6, 1
#define PFD_480_PFDn_FRAC                 0, 6
   
   
#define CCM_ANALOG_PLL_PFD_528_OFFS      0x100 /*!<  */
#define CCM_ANALOG_PLL_PFD_528_SET_OFFS  0x104 /*!<  */
#define CCM_ANALOG_PLL_PFD_528_CLR_OFFS  0x108 /*!<  */
#define CCM_ANALOG_PLL_PFD_528_TOG_OFFS  0x10C /*!<  */
   
#define PFD_528_PFD3_CLKGATE             31, 1
#define PFD_528_PFD3_STABLE              30, 1
#define PFD_528_PFD3_FRAC                24, 6

#define PFD_528_PFD2_CLKGATE             23, 1
#define PFD_528_PFD2_STABLE              22, 1
#define PFD_528_PFD2_FRAC                16, 6
   
#define PFD_528_PFD1_CLKGATE             15, 1
#define PFD_528_PFD1_STABLE              14, 1
#define PFD_528_PFD1_FRAC                 8, 6
   
#define PFD_528_PFD0_CLKGATE              7, 1
#define PFD_528_PFD0_STABLE               6, 1
#define PFD_528_PFD0_FRAC                 0, 6
   
   
#define CCM_ANALOG_PLL_PFD0_528_OFFS     0x100 /*!<  */
#define CCM_ANALOG_PLL_PFD0_528_SET_OFFS 0x104 /*!<  */
#define CCM_ANALOG_PLL_PFD0_528_CLR_OFFS 0x108 /*!<  */
#define CCM_ANALOG_PLL_PFD0_528_TOG_OFFS 0x10C /*!<  */
   
#define CCM_ANALOG_PLL_PFD1_528_OFFS     0x101 /*!<  */
#define CCM_ANALOG_PLL_PFD1_528_SET_OFFS 0x105 /*!<  */
#define CCM_ANALOG_PLL_PFD1_528_CLR_OFFS 0x109 /*!<  */
#define CCM_ANALOG_PLL_PFD1_528_TOG_OFFS 0x10D /*!<  */
   
#define CCM_ANALOG_PLL_PFD2_528_OFFS     0x102 /*!<  */
#define CCM_ANALOG_PLL_PFD2_528_SET_OFFS 0x106 /*!<  */
#define CCM_ANALOG_PLL_PFD2_528_CLR_OFFS 0x10A /*!<  */
#define CCM_ANALOG_PLL_PFD2_528_TOG_OFFS 0x10E /*!<  */
   


/*
 * Clock Control Module (Digital Part)
 */

/* CCM Control Register */
#define CCM_CCR_OFFS                0x000 /*!<  */

#define CCM_CCR_RBC_ENA             27, 1
#define CCM_CCR_REG_BYPASS_COUNT    21, 6
#define CCM_CCR_COSC_ENA            12, 1
#define CCM_CCR_OSC_COUNT            0, 8

/* CCM Status Register */
#define CCM_CSR_OFFS                0x008 /*!<  */

#define CCM_CSR_COSC_READY           5, 1
#define CCM_CSR_CAMP2_READY          3, 1
#define CCM_CSR_REF_ENA_B            0, 1

/* CCM Clock Switcher Register */
#define CCM_CCSR_OFFS               0x00C /*!<  */

#define CCM_CCSR_PLL3_SW_CLK_SEL     0, 1

/* CCM ARM Clock Root Register */
#define CCM_CACRR_OFFS              0x010 /*!<  */

#define CCM_CACRR_ARM_PODF           0, 3

/* CCM Bus Clock Divider Register */
#define CCM_CBCDR_OFFS              0x014 /*!<  */

#define CCM_CBCDR_PERIPH_CLK2_PODF_BF   27, 3
#define CCM_CBCDR_PER1PH_CLK_SEL_BF     25, 1
#define CCM_CBCDR_SEMC_PODF_BF          16, 3
#define CCM_CBCDR_AHB_PODF_BF           10, 3
#define CCM_CBCDR_IPG_PODF_BF            8, 2
#define CCM_CBCDR_SEMC_ALT_CLK_SEL_BF    7, 1
#define CCM_CBCDR_SEMC_CLK_SEL_BF        6, 1

/* CCM Bus Clock Multiplexer Register */
#define CCM_CBCMR_OFFS              0x018 /*!<  */

#define CCM_CBCMR_FLEXSPI2_CLK_PODF_BF  29, 3
#define CCM_CBCMR_LPSPI_CLK_PODF_BF     26, 3
#define CCM_CBCMR_LCDIF_CLK_PODF_BF     23, 3
#define CCM_CBCMR_PRE_PERIPH_CLK_SEL_BF 18, 2
#define CCM_CBCMR_TRACE_CLK_SEL_BF      14, 2
#define CCM_CBCMR_PERIPH_CLK2_SEL_BF    12, 2
#define CCM_CBCMR_FLEXSPI2_CLK_SEL_BF    8, 2
#define CCM_CBCMR_LPSPI_CLK_SEL_BF       4, 2

/* CCM Serial Clock Multiplexer Register 1 */
#define CCM_CSCMR1_OFFS             0x01C /*!<  */

#define CCM_CSCMR1_PER_CLK_SEL_BF   6, 1
#define CCM_CSCMR1_PER_CLK_PODF_BF  0, 6


/* CCM Serial Clock Multiplexer Register 2 */
#define CCM_CSCMR2_OFFS             0x020 /*!<  */

/* CCM Serial Clock Divider Register 1 */
#define CCM_CSCDR1_OFFS             0x024 /*!<  */

#define CCM_CSCDR1_UART_CLK_SEL_BF   6, 1
#define CCM_CSCDR1_UART_CLK_PODF_BF  0, 6

/* CCM SAI1 / SAI3 Clock Divider Register */
#define CCM_CS1CDR_OFFS             0x028 /*!<  */
/* CCM SAI2 Clock Divider Register */
#define CCM_CS2CDR_OFFS             0x02C /*!<  */
/* CCM D1 Clock Divider Register */
#define CCM_CDCDR_OFFS              0x030 /*!<  */
/* CCM Serial Clock Divider Register 2 */
#define CCM_CSCDR2_OFFS             0x038 /*!<  */
/* CCM Serial Clock Divider Register 3 */
#define CCM_CSCDR3_OFFS             0x03C /*!<  */
/* CCM Divider Handshake In-Progress Register */
#define CCM_CDHIPR_OFFS             0x048 /*!<  */
/* CCM Low Power Control Register */
#define CCM_CLPCR_OFFS              0x054 /*!<  */
/* CCM Interrupt Status Register */
#define CCM_CISR_OFFS               0x058 /*!<  */
/* CCM Interrupt Mask Register */
#define CCM_CIMR_OFFS               0x05C /*!<  */
/* CCM Clock Output Source Register */
#define CCM_CCOSR_OFFS              0x060 /*!<  */
/* CCM General Purpose Register */
#define CCM_CGPR_OFFS               0x064 /*!<  */


#if !defined(__ASSEMBLER__)
enum
{
  CCM_CG_eCLK_OFF = 0,    /* Clock is off during all modes */
  CCM_CG_eCLK_ON_RUN = 1, /* Clock is on in RUN mode, but off in WAIT and STOP mode */
  CCM_CG_eCLK_ON_ALW = 3, /* Clock is on during all modes except STOP mode. */
};
#endif /* !defined(__ASSEMBLER__) */

#define CCM_CG15_BF                      30, 2
#define CCM_CG14_BF                      28, 2
#define CCM_CG13_BF                      26, 2
#define CCM_CG12_BF                      24, 2
#define CCM_CG11_BF                      22, 2
#define CCM_CG10_BF                      20, 2
#define CCM_CG9_BF                       18, 2
#define CCM_CG8_BF                       16, 2
#define CCM_CG7_BF                       14, 2
#define CCM_CG6_BF                       12, 2
#define CCM_CG5_BF                       10, 2
#define CCM_CG4_BF                        8, 2
#define CCM_CG3_BF                        6, 2
#define CCM_CG2_BF                        4, 2
#define CCM_CG1_BF                        2, 2
#define CCM_CG0_BF                        0, 2

/* CCM Clock Gating Register 0 */
#define CCM_CCGR0_OFFS                   0x068 /*!<  */

#define GPIO2_CLK_ENA_BF                 CCM_CG15_BF
#define LPUART2_CLK_ENA_BF               CCM_CG14_BF
#define GPT2_SER_CLK_ENA_BF              CCM_CG13_BF
#define GPT2_BUS_CLK_ENA_BF              CCM_CG12_BF
#define TRACE_CLK_ENA_BF                 CCM_CG11_BF
#define CAN2_SER_CLK_ENA_BF              CCM_CG10_BF
#define CAN2_CLK_ENA_BF                  CCM_CG9_BF
#define CAN1_SER_CLK_ENA_BF              CCM_CG8_BF
#define CAN1_CLK_ENA_BF                  CCM_CG7_BF
#define LPUART3_CLK_ENA_BF               CCM_CG6_BF
#define DCP_CLK_ENA_BF                   CCM_CG5_BF
#define SIM_MAIN_CLK_ENA_BF              CCM_CG4_BF
#define MQS_CLK_ENA_BF                   CCM_CG2_BF
#define AIPSTZ2_CLK_ENA_BF               CCM_CG1_BF
#define AIPSTZ1_CLK_ENA_BF               CCM_CG0_BF

/* CCM Clock Gating Register 1 */
#define CCM_CCGR1_OFFS                   0x06C /*!<  */

#define GPIO5_CLK_ENA_BF                 CCM_CG15_BF
#define CSU_CLK_ENA_BF                   CCM_CG14_BF
#define GPIO1_CLK_ENA_BF                 CCM_CG13_BF
#define LPUART4_CLK_ENA_BF               CCM_CG12_BF
#define GPT1_SER_CLK_ENA_BF              CCM_CG11_BF
#define GPT1_BUS_CLK_ENA_BF              CCM_CG10_BF
#define SEMC_CLK_ENA_BF                  CCM_CG9_BF
#define ADC1_CLK_ENA_BF                  CCM_CG8_BF
#define AOI_CLK_ENA_BF                   CCM_CG7_BF
#define PIT_CLK_ENA_BF                   CCM_CG6_BF
#define ENET_CLK_ENA_BF                  CCM_CG5_BF
#define ADC2_CLK_ENA_BF                  CCM_CG4_BF
#define LPSPI4_CLK_ENA_BF                CCM_CG3_BF
#define LPSPI3_CLK_ENA_BF                CCM_CG2_BF
#define LPSPI2_CLK_ENA_BF                CCM_CG1_BF
#define LPSPI1_CLK_ENA_BF                CCM_CG0_BF

/* CCM Clock Gating Register 2 */
#define CCM_CCGR2_OFFS                   0x070 /*!<  */

#define OCOTP_CTRL_CLK_ENA_BF            12, 2
#define IOMUXC_SNVS_CLK_ENA_BF            4, 2
#define OCRAM_EXSC_CLK_ENA_BF             0, 2

/* CCM Clock Gating Register 3 */
#define CCM_CCGR3_OFFS                   0x074 /*!<  */

#define IOMUXC_SNVS_GPR_CLK_ENA_BF       30, 2
#define OCRAM_CLK_ENA_BF                 28, 2

/* CCM Clock Gating Register 4 */
#define CCM_CCGR4_OFFS                   0x078 /*!<  */

#define IOMUXC_ENC4_CLK_ENA_BF           CCM_CG15_BF
#define IOMUXC_ENC3_CLK_ENA_BF           CCM_CG14_BF
#define IOMUXC_ENC2_CLK_ENA_BF           CCM_CG13_BF
#define IOMUXC_ENC1_CLK_ENA_BF           CCM_CG12_BF
#define IOMUXC_PWM4_CLK_ENA_BF           CCM_CG11_BF
#define IOMUXC_PWM3_CLK_ENA_BF           CCM_CG10_BF
#define IOMUXC_PWM2_CLK_ENA_BF           CCM_CG9_BF
#define IOMUXC_PWM1_CLK_ENA_BF           CCM_CG8_BF
#define IOMUXC_SIM_EMS_CLK_ENA_BF        CCM_CG7_BF
#define IOMUXC_SIM_M_CLK_ENA_BF          CCM_CG6_BF
#define IOMUXC_TSC_CLK_ENA_BF            CCM_CG5_BF
#define IOMUXC_SIM_M7_CLK_ENA_BF         CCM_CG4_BF
#define IOMUXC_BEE_CLK_ENA_BF            CCM_CG3_BF
#define IOMUXC_GPR_CLK_ENA_BF            CCM_CG2_BF
#define IOMUXC_CLK_ENA_BF                CCM_CG1_BF
#define IOMUXC_SIM_M7_REG_CLK_ENA_BF     CCM_CG0_BF

/* CCM Clock Gating Register 5 */
#define CCM_CCGR5_OFFS                   0x07C /*!<  */

#define LPUART7_CLK_ENA_BF               26, 2
#define LPUART1_CLK_ENA_BF               24, 2

/* CCM Clock Gating Register 6 */
#define CCM_CCGR6_OFFS                   0x080 /*!<  */

/* CCM Clock Gating Register 7 */
#define CCM_CCGR8_OFFS                   0x084 /*!<  */

/* CCM Module Enable Override Register */
#define CCM_CMEOR_OFFS                   0x088 /*!<  */


#endif /* IMXRT_CCM_H */

