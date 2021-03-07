#ifndef IMXRT_LPUART_H
#define IMXRT_LPUART_H


/*
 * Low Power Universal Asynchronous Receiver Transmitter Registers
 */

/* LPUART Version ID Register */
#define LPUART_VER_ID_OFFS     0x000

/* LPUART Parameter Register */
#define LPUART_PARAM_OFFS      0x004

#define LPUART_PARAM_RXFIFO_BF  8,  8
#define LPUART_PARAM_TXFIFO_BF  1,  8

/* LPUART Global Register */
#define LPUART_GLOBAL_OFFS     0x008

#define LPUART_GLOBAL_RST_BF    1,  1

/* LPUART Pin Configuration Register */
#define LPUART_PIN_CFG_OFFS    0x00C

#define LPUART_PIN_CFG_TRGSEL_BF  0,  2

/* LPUART Baud Rate Register */
#define LPUART_BAUD_RATE_OFFS  0x010

#define LPUART_BAUD_MATCH_ADDR1_ENA_BF   31,  1
#define LPUART_BAUD_MATCH_ADDR2_ENA_BF   30,  1
#define LPUART_BAUD_M10_ENA_BF           29,  1
#define LPUART_BAUD_OVER_SAMPLE_RATIO_BF 24,  5
#define LPUART_BAUD_TX_DMA_ENA_BF        23,  1
#define LPUART_BAUD_RX_DMA_ENA_BF        21,  1
#define LPUART_BAUD_RX_IDLE_DMA_ENA_BF   20,  1
#define LPUART_BAUD_MATCH_CONFIG_BF      18,  2
#define LPUART_BAUD_BOTH_EDGE_ENA_BF     17,  1
#define LPUART_BAUD_RESYNC_DIS_BF        16,  1
#define LPUART_BAUD_LIN_BRK_IRQ_ENA_BF   15,  1
#define LPUART_BAUD_RX_EDGE_IRQ_ENA_BF   14,  1
#define LPUART_BAUD_STOP_BIT_NUM_BF      13,  1
#define LPUART_BAUD_RATE_MODULO_DIV_BF    0, 13


/* LPUART Status Register */
#define LPUART_STATUS_OFFS     0x014

#define LPUART_STATUS_LIN_BRK_BF  31,  1
#define LPUART_STATUS_RX_EDGE_BF  30,  1
#define LPUART_STATUS_MSB_1ST_BF  29,  1
#define LPUART_STATUS_RXINV_BF    28,  1
#define LPUART_STATUS_TDRE_BF     23,  1
#define LPUART_STATUS_TXC_BF      22,  1
#define LPUART_STATUS_RDRF_BF     21,  1
#define LPUART_STATUS_IDLE_BF     20,  1
#define LPUART_STATUS_RX_OVR_BF   19,  1
#define LPUART_STATUS_NE_BF       18,  1
#define LPUART_STATUS_FE_BF       17,  1
#define LPUART_STATUS_PE_BF       16,  1
#define LPUART_STATUS_MA1_BF      15,  1
#define LPUART_STATUS_MA2_BF      14,  1


/* LPUART Control Register */
#define LPUART_CTRL_OFFS       0x018

#define LPUART_CTRL_TXDIR_BF      29, 1
#define LPUART_CTRL_TXINV_BF      28, 1
#define LPUART_CTRL_ORIE_BF       27, 1
#define LPUART_CTRL_NEIE_BF       26, 1
#define LPUART_CTRL_FEIE_BF       25, 1
#define LPUART_CTRL_PEIE_BF       24, 1
#define LPUART_CTRL_TIE_BF        23, 1
#define LPUART_CTRL_TCIE_BF       22, 1
#define LPUART_CTRL_RIE_BF        21, 1
#define LPUART_CTRL_ILIE_BF       20, 1
#define LPUART_CTRL_TE_BF         19, 1
#define LPUART_CTRL_RE_BF         18, 1
#define LPUART_CTRL_RWU_BF        17, 1
#define LPUART_CTRL_SBK_BF        16, 1
#define LPUART_CTRL_MA1IE_BF      15, 1
#define LPUART_CTRL_MA2IE_BF      14, 1
#define LPUART_CTRL_M7_ENA_BF     13, 1
#define LPUART_CTRL_IDLE_CFG_BF    8, 3
#define LPUART_CTRL_LOOPBACK_BF    7, 1
#define LPUART_CTRL_DOZE_ENA_BF    6, 1
#define LPUART_CTRL_RX_SRC_BF      5, 1
#define LPUART_CTRL_M9_ENA_BF      4, 1
#define LPUART_CTRL_WAKE_BF        3, 1
#define LPUART_CTRL_IDLE_TYPE_BF   2, 1
#define LPUART_CTRL_PEN_BF         1, 1
#define LPUART_CTRL_PTYPE_BF       0, 1

/* LPUART Data Register */
#define LPUART_DATA_OFFS       0x01C
#define LPUART_MATCH_ADDR_OFFS 0x020
#define LPUART_MODEM_IRDA_OFFS 0x024
#define LPUART_FIFO_OFFS       0x028
#define LPUART_WATER_MARK_OFFS 0x02C

#endif /* IMXRT_LPUART_H */

