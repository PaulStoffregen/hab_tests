#ifndef IMXRT_LPSPI_H
#define IMXRT_LPSPI_H


/*
 * Low Power Serial Peripherel Interface Registers
 */

/* LPSPI Version ID Register */
#define LPSPI_VER_ID_OFFS                 0x000

#define LPSPI_VER_ID_MAJ_BF               24,  8
#define LPSPI_VER_ID_MIN_BF               16,  8
#define LPSPI_VER_ID_FEAT_BF               0, 16

/* LPSPI Parameter Register */
#define LPSPI_PARAM_OFFS                  0x004

#define LPSPI_PARAM_PCSNUM_BF             16,  8
#define LPSPI_PARAM_RXFIFO_BF              8,  8
#define LPSPI_PARAM_TXFIFO_BF              1,  8

/* LPSPI Control Register */
#define LPSPI_CTRL_REG_OFFS               0x010
#define LPSPI_CTRL_RST_RX_FIFO_BF          9,  1
#define LPSPI_CTRL_RST_TX_FIFO_BF          8,  1
#define LPSPI_CTRL_DBG_ENA_BF              3,  1
#define LPSPI_CTRL_DOZE_ENA_BF             2,  1
#define LPSPI_CTRL_RST_BF                  1,  1
#define LPSPI_CTRL_ENA_BF                  0,  1


/* LPSPI Status Register */
#define LPSPI_STAT_REG_OFFS               0x014

#define LPSPI_STAT_BUSY_BF                24,  1
#define LPSPI_STAT_DATA_MATCH_BF          13,  1
#define LPSPI_STAT_RX_ERROR_BF            12,  1
#define LPSPI_STAT_TX_ERROR_BF            11,  1
#define LPSPI_STAT_TX_COMPL_BF            10,  1
#define LPSPI_STAT_RX_FRAME_COMPL_BF       9,  1
#define LPSPI_STAT_RX_WORD_COMPL_BF        8,  1
#define LPSPI_STAT_RX_DATA_BF              1,  1
#define LPSPI_STAT_TX_DATA_BF              0,  1


/* LPSPI Interrupt Enable Register */
#define LPSPI_INT_REG_OFFS                0x018

#define LPSPI_INT_DATA_MATCH_ENA_BF       13,  1
#define LPSPI_INT_RX_ERROR_ENA_BF         12,  1
#define LPSPI_INT_TX_ERROR_ENA_BF         11,  1
#define LPSPI_INT_TX_COMPL_ENA_BF         10,  1
#define LPSPI_INT_FRAME_COMPL_ENA_BF       9,  1
#define LPSPI_INT_RX_WORD_COMPL_ENA_BF     8,  1
#define LPSPI_INT_RX_DATA_ENA_BF           1,  1
#define LPSPI_INT_TX_DATA_ENA_BF           0,  1


/* LPSPI DMA Enable Register */
#define LPSPI_DMA_REG_OFFS                0x01C

#define LPSPI_DMA_RX_DMA_REQ_ENA_BF        1,  1
#define LPSPI_DMA_TX_DMA_REQ_ENA_BF        0,  1


/* LPSPI Config Register 0 */
#define LPSPI_CONF0_REG_OFFS              0x020

#define SPI_CONF_HW_TRIG_LEN_BF           24,  5
#define SPI_CONF_SCLK_INACT_STATE_BF      20,  4

#define SPI_SCLK_INACT_LO                      0
#define SPI_SCLK_INACT_HI                      1

#define SPI_CONF_DATA_INACT_STATE_BF      16,  4

#define SPI_DATA_INACT_HI                      0
#define SPI_DATA_INACT_LO                      1

#define SPI_CONF_CS_ACT_STATE_BF          12,  4

#define SPI_CS_ACT_LO                          0
#define SPI_CS_ACT_HI                          1

#define SPI_CONF_CS_CTRL_BF                8,  4
#define SPI_CONF_SCLK_POL_BF               4,  4
#define SPI_CONF_SCLK_PHA_BF               0,  4


/* LPSPI Config Register 0 */
#define LPSPI_CONF1_REG_OFFS              0x024

#define LPSPI_CONF1_PCS_CFG_BF            27,  1
#define LPSPI_CONF1_OUT_CFG_BF            26,  1
#define LPSPI_CONF1_PIN_CFG_BF            24,  2
#define LPSPI_CONF1_MATCH_CFG_BF          16,  3
#define LPSPI_CONF1_PCS_POL_BF             8,  4
#define LPSPI_CONF1_NO_STALL_BF            3,  1
#define LPSPI_CONF1_AUTO_PCS_BF            2,  1
#define LPSPI_CONF1_SAMPLE_PT_BF           1,  1
#define LPSPI_CONF1_MSTR_BF                0,  1

/* LPSPI Data Match Register 0 */
#define LPSPI_DATA_MATCH0_REG_OFFS        0x030

/* LPSPI Data Match Register 1 */
#define LPSPI_DATA_MATCH1_REG_OFFS        0x034

/* LPSPI Clock Configuration Register */
#define LPSPI_CLK_CONF_REG_OFFS           0x040

#define LPSPI_CLK_CONF_SCK_TO_CS_BF       24,  8
#define LPSPI_CLK_CONF_CS_TO_SCK_BF       16,  8
#define LPSPI_CLK_CONF_GAP_BF              8,  8
#define LPSPI_CLK_CONF_SCK_DIV_BF          0,  8

/* LPSPI FIFO Control Register */
#define LPSPI_FIFO_CTL_REG_OFFS           0x058

#define LPSPI_FIFO_CTL_RX_WATER_MARK_BF   16,  4
#define LPSPI_FIFO_CTL_TX_WATER_MARK_BF    0,  4

/* LPSPI FIFO Status Register */
#define LPSPI_FIFO_STAT_REG_OFFS          0x05C

#define LPSPI_FIFO_STAT_RX_COUNT_BF       16,  5
#define LPSPI_FIFO_STAT_TX_COUNT_BF        0,  5

/* LPSPI Transmit Command Register */
#define LPSPI_TX_CMD_REG_OFFS             0x060

#define LPSPI_TX_CMD_CPOL_BF              31,  1
#define LPSPI_TX_CMD_CPHA_BF              30,  1
#define LPSPI_TX_CMD_CLK_MODE_BF          30,  2
#define LPSPI_TX_CMD_PRESCALE_BF          27,  3

#define LPSPI_PRESCALE_DIV1               0
#define LPSPI_PRESCALE_DIV2               1
#define LPSPI_PRESCALE_DIV4               2
#define LPSPI_PRESCALE_DIV8               3
#define LPSPI_PRESCALE_DIV16              4
#define LPSPI_PRESCALE_DIV32              5
#define LPSPI_PRESCALE_DIV64              6
#define LPSPI_PRESCALE_DIV128             7

#define LPSPI_TX_CMD_PCS_BF               24,  2
#define LPSPI_TX_CMD_LSB_1ST_BF           23,  1
#define LPSPI_TX_CMD_BYTE_SWAP_BF         22,  1
#define LPSPI_TX_CMD_CONT_XFER_BF         21,  1
#define LPSPI_TX_CMD_CONT_CMD_BF          20,  1
#define LPSPI_TX_CMD_RX_MSK_BF            19,  1
#define LPSPI_TX_CMD_TX_MSK_BF            18,  1
#define LPSPI_TX_CMD_XFER_WIDTH_BF        16,  2
#define LPSPI_TX_CMD_FRAME_SIZE_BF         0, 12

/* LPSPI Transmit Data Register */
#define LPSPI_TX_DATA_REG_OFFS            0x064

/* LPSPI Receive Status Register */
#define LPSPI_RX_STAT_REG_OFFS            0x070

#define LPSPI_RX_STAT_RX_FIFO_EMPTY_BF     1,  1
#define LPSPI_RX_STAT_SOF_BF               0,  1

/* LPSPI Receive Data Register */
#define LPSPI_RX_DATA_REG_OFFS            0x074

#endif /* IMXRT_LPSPI_H */

