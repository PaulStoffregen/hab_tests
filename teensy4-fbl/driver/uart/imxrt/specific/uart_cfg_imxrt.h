#ifndef UART_CFG_IMXRT_H
#define UART_CFG_IMXRT_H

//TODO: This is Board-/Project-specific and shall be defined in that manner
#define UART_NUM_DEVS_USED          (2u)

/* Define UART instance IDs */
#define UART_CTL_ID_UART1           (0u)
#define UART_CTL_ID_UART3           (2u)

/* define COM UART dependent stuff */
#define UART1_BAUDRATE           115200u
#define UART1_TX_BUF_SIZE        (256u)
#define UART1_RX_BUF_SIZE        (1088u)

/* define DBG UART dependent stuff */
#define UART3_BAUDRATE           115200u
#define UART3_TX_BUF_SIZE        (1024u)
#define UART3_RX_BUF_SIZE        (128u)


#if defined (UART_DDM_C)

/* Definition of all static properties for the device instance used as UART0 */


/* Pad configuration value for the UART pads */
#define UART_PAD_CONF (0                            \
  | BF_SET(PADCONF_eHYS_SCHMITT, PADCONF_HYS_BF)    \
  | BF_SET(PADCONF_eODE_CMOS, PADCONF_ODE_BF)       \
  | BF_SET(PADCONF_ePKE_ENA, PADCONF_PKE_BF)        \
  | BF_SET(PADCONF_ePUE_PULL, PADCONF_PUE_BF)       \
  | BF_SET(PADCONF_ePUS_PU_100K, PADCONF_PUS_BF)    \
  | BF_SET(PADCONF_eSPEED_100MHZ, PADCONF_SPEED_BF) \
  | BF_SET(PADCONF_eDSE_R0_BY_6, PADCONF_DSE_BF)    \
  | BF_SET(PADCONF_eSRE_SLOW, PADCONF_SRE_BF)       \
  )


/* Pin configuration */
T_IOMUX_DESC uart1_pinCfg[] =
{
  /* UART1_RXD */
  {
    .padConf = PAD_CNF_DEF(PADCONF_LPUART1_RX__GPIO_AD_B0_13_ALT2_OFFS, UART_PAD_CONF),
    .muxConf = MUX_CNF_DEF(MUXCONF_LPUART1_RX__GPIO_AD_B0_13_ALT2_OFFS, 2, 0),
    .inpConf = INP_CNF_DEF(0, 0),
  },
  /* UART1_TXD */
  {
    .padConf = PAD_CNF_DEF(PADCONF_LPUART1_TX__GPIO_AD_B0_12_ALT2_OFFS, UART_PAD_CONF),
    .muxConf = MUX_CNF_DEF(MUXCONF_LPUART1_TX__GPIO_AD_B0_12_ALT2_OFFS, 2, 0),
    .inpConf = INP_CNF_DEF(0, 0),
  },
  { 0 },
};


/* Clock configuration */
T_CCM_CLK_CFG uart1_clkCfg[] =
{
  /*          (    offs, lsb, width, cfg) */
  // CCM_CSCDR1_UART_CLK_SEL_BF 
  // CCM_CSCDR1_UART_CLK_PODF_BF /* PODF default */
  CLK_CNF_DEF(CCM_CCGR5_OFFS, LPUART1_CLK_ENA_BF, CCM_CG_eCLK_ON_ALW),
  CLK_CNF_END(),
};


/* IRQ configuration */
T_IRQC_IRQ_CFG uart1_irqCfg[] =
{
  IRQC_CFG_IRQ(IRQ_ID_LPUART1, 0, 1),
  IRQC_CFG_IRQ_END(),
};


/* Pin configuration */
T_IOMUX_DESC uart3_pinCfg[] =
{
  /* UART3_RXD */
  {
    .padConf = PAD_CNF_DEF(PADCONF_LPUART3_RX__GPIO_AD_B1_07_ALT2_OFFS, UART_PAD_CONF),
    .muxConf = MUX_CNF_DEF(MUXCONF_LPUART3_RX__GPIO_AD_B1_07_ALT2_OFFS, 2, 0),
    .inpConf = INP_CNF_DEF(INPCONF_LPUART3_RX_OFFS, 0),
  },
  /* UART3_TXD */
  {
    .padConf = PAD_CNF_DEF(PADCONF_LPUART3_TX__GPIO_AD_B1_06_ALT2_OFFS, UART_PAD_CONF),
    .muxConf = MUX_CNF_DEF(MUXCONF_LPUART3_TX__GPIO_AD_B1_06_ALT2_OFFS, 2, 0),
    .inpConf = INP_CNF_DEF(0, 0),
  },
  { 0 },
};


/* Clock configuration */
T_CCM_CLK_CFG uart3_clkCfg[] =
{
  /*          (    offs, lsb, width, cfg) */
  // CCM_CSCDR1_UART_CLK_SEL_BF 
  // CCM_CSCDR1_UART_CLK_PODF_BF /* PODF default */
  CLK_CNF_DEF(CCM_CCGR0_OFFS, LPUART3_CLK_ENA_BF, CCM_CG_eCLK_ON_ALW), /* CG on */
  CLK_CNF_END(),
};


/* IRQ configuration */
T_IRQC_IRQ_CFG uart3_irqCfg[] =
{
  IRQC_CFG_IRQ(IRQ_ID_LPUART3, 1, 1),
  IRQC_CFG_IRQ_END(),
};

static uint8 uart1_txBuf[UART1_TX_BUF_SIZE];
static uint8 uart1_rxBuf[UART1_RX_BUF_SIZE];
static uint8 uart3_txBuf[UART3_TX_BUF_SIZE];
static uint8 uart3_rxBuf[UART3_RX_BUF_SIZE];


/* Table of the device instance descriptors.
 * This table defines for every used device, which ressources
 * shall be used by this instance.
 */
const T_UART_CTL_DESC uart_ctlDevDescTbl[UART_NUM_DEVS_USED] =
{
  [0] =
  {
    .devID  = UART_CTL_ID_UART3,
    .base   = UART3_BASE,
    .pinCfg = uart3_pinCfg,
    .clkCfg = uart3_clkCfg,
    .irqCfg = uart3_irqCfg,
    .handler = &uart3_handler,
    .flags = 0x0,
    .txBuffer = uart3_txBuf,
    .rxBuffer = uart3_rxBuf,
    .txBufSize = sizeof(uart3_txBuf),
    .rxBufSize = sizeof(uart3_rxBuf),
  },
  [1] =
  {
    .devID  = UART_CTL_ID_UART1,
    .base   = UART1_BASE,
    .pinCfg = uart1_pinCfg,
    .clkCfg = uart1_clkCfg,
    .irqCfg = uart1_irqCfg,
    .handler = &uart1_handler,
    .flags = 0x0,
    .txBuffer = uart1_txBuf,
    .rxBuffer = uart1_rxBuf,
    .txBufSize = sizeof(uart1_txBuf),
    .rxBufSize = sizeof(uart1_rxBuf),
  },
};


/* Table of device settings.
 * This table defines the settings used to configure devices.
 * Every entry contains a dedicated configuration, which
 * might be shared amongst multiple device instances.
 * It is not strictly necessary to define the configuations
 * in a table, so they might be defined within multiple
 * individual config descriptors.
 */

const T_UART_CTL_CFG uart_ctlDevCfgTbl[UART_NUM_DEVS_USED] =
{
  [0] =
  {
    .baudrate = UART3_BAUDRATE,
    .speed = 0, //UART_SPEED(115200),
    .dataBits = 8,
    .stopBits = 1,
    .parity = 0,
    .mode = 0,
    .flags = ( 0
             | BF_MASK(UART_DEV_DATA_FLAGS_FIFO_ENA_BF)
//             | BF_MASK(UART_DEV_DATA_FLAGS_NOBLOCK_BF)
             ),
    .opMode = (UART_OPMODE_RX | UART_OPMODE_TX),
    .clkPolarity = 0,
    .clkPhase = 0,
  },
  [1] =
  {
    .baudrate = UART1_BAUDRATE,
    .speed = 0, //UART_SPEED(115200),
    .dataBits = 8,
    .stopBits = 1,
    .parity = 0,
    .mode = 0,
    .flags = ( 0
             | BF_MASK(UART_DEV_DATA_FLAGS_FIFO_ENA_BF)
             | BF_MASK(UART_DEV_DATA_FLAGS_NOBLOCK_BF)
             ),
    .opMode = (UART_OPMODE_RX | UART_OPMODE_TX),
    .clkPolarity = 0,
    .clkPhase = 0,
  },
};


/* Reserve space for runtime data for all used UART devices.
 * A chunk is given to the driver with every initialization and used
 * by the driver to store device specific data
 */
static T_UART_CTL_DATA uart_ctlDevData[UART_NUM_DEVS_USED];


// TODO: This is SoC-specific and should be defined in a SoC specific file
#if (BSP_SOC_TYPE == BSP_SOC_IMXRT)
#define UART_NUM_DEVS_AVAIL         (8u)
#endif /* BSP_SOC_TYPE */

// TODO: This is SoC-specific and should be defined in a SoC specific file
T_UART_CTL_DATA* uart_ctlDevDataPtr[UART_NUM_DEVS_AVAIL];

// TODO: This is SoC-specific and should be defined in a SoC specific file
#if (BSP_SOC_TYPE == BSP_SOC_IMXRT)
const uint32 uart_ctlDevBaseTable[UART_NUM_DEVS_AVAIL] =
{
  UART1_BASE,
  UART2_BASE,
  UART3_BASE,
  UART4_BASE,
  UART5_BASE,
  UART6_BASE,
  UART7_BASE,
  UART8_BASE,
};
#endif /* BSP_SOC_TYPE */

#endif /* defined (UART_DDM_C) */

extern const T_UART_CTL_CFG uart_ctlDevCfgTbl[UART_NUM_DEVS_USED];

#endif /* UART_CFG_IMXRT_H */

