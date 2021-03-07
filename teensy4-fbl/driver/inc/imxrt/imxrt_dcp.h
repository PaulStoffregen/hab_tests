#ifndef IMXRT_DCP_H
#define IMXRT_DCP_H


#define DCP_CTRL_OFFS                  0x000
#define DCP_CTRL_SET_OFFS              0x004
#define DCP_CTRL_CLR_OFFS              0x008
#define DCP_CTRL_TOG_OFFS              0x00C

#define DCP_CTRL_SOFT_RST_BF           31, 1
#define DCP_CTRL_CLK_GATE_BF           30, 1
#define DCP_CTRL_PRESET_CRYPTO_BF      29, 1
#define DCP_CTRL_PRESET_SHA_BF         28, 1
#define DCP_CTRL_GATHER_RESIDUAL_WR_BF 23, 1
#define DCP_CTRL_CONTEXT_CACHE_ENA_BF  22, 1
#define DCP_CTRL_CONTEXT_SWITH_ENA_BF  21, 1
#define DCP_CTRL_CHANNEL_INT_ENA_BF     0, 8


#define DCP_STAT_OFFS                  0x010
#define DCP_STAT_SET_OFFS              0x014
#define DCP_STAT_CLR_OFFS              0x018
#define DCP_STAT_TOG_OFFS              0x01C

#define DCP_STAT_OP_KEY_READY_BF       28, 1
#define DCP_STAT_CUR_CHANNEL_BF        24, 4
#define DCP_STAT_READY_CHANNELS_BF     16, 8
#define DCP_STAT_IRQ_BF                 0, 4


#define DCP_CHANNEL_CTRL_OFFS          0x020
#define DCP_CAPABILITY0_OFFS           0x030
#define DCP_CAPABILITY1_OFFS           0x040
#define DCP_CONTEXT_OFFS               0x050
#define DCP_KEY_INDEX_OFFS             0x060

#define DCP_KEY_INDEX_SLOT_ID_BF        4, 2
#define DCP_KEY_INDEX_SUB_WORD_BF       0, 2


#define DCP_KEY_DATA_OFFS              0x070

#define DCP_PACKET0_STAT_OFFS          0x080
#define DCP_PACKET1_STAT_OFFS          0x090
#define DCP_PACKET2_STAT_OFFS          0x0A0
#define DCP_PACKET3_STAT_OFFS          0x0B0
#define DCP_PACKET4_STAT_OFFS          0x0C0
#define DCP_PACKET5_STAT_OFFS          0x0D0
#define DCP_PACKET6_STAT_OFFS          0x0E0


#define DCP_CH0_OFFS                   0x100
#define DCP_CH1_OFFS                   0x140
#define DCP_CH2_OFFS                   0x180
#define DCP_CH3_OFFS                   0x1C0


/* Channel register offests */
#define DCP_CHx_CMD_OFFS               0x000
#define DCP_CHx_CMD_SET_OFFS           0x004
#define DCP_CHx_CMD_CLR_OFFS           0x008
#define DCP_CHx_CMD_TOG_OFFS           0x00C
#define DCP_CHx_SEMA_OFFS              0x010
#define DCP_CHx_SEMA_SET_OFFS          0x014
#define DCP_CHx_SEMA_CLR_OFFS          0x018
#define DCP_CHx_SEMA_TOG_OFFS          0x01C
#define DCP_CHx_STAT_OFFS              0x020
#define DCP_CHx_STAT_SET_OFFS          0x024
#define DCP_CHx_STAT_CLR_OFFS          0x028
#define DCP_CHx_STAT_TOG_OFFS          0x02C

#define DCP_CH_STAT_TAG_BF             24, 8
#define DCP_CH_STAT_ERR_CODE_BF        16, 8
#define DCP_CH_STAT_ERR_PG_FAULT_BF     6, 1
#define DCP_CH_STAT_ERR_DST_BF          5, 1
#define DCP_CH_STAT_ERR_SRC_BF          4, 1
#define DCP_CH_STAT_ERR_PACKET_BF       3, 1
#define DCP_CH_STAT_ERR_SETUP_BF        2, 1
#define DCP_CH_STAT_ERR_HASH_BF         1, 1


#define DCP_CHx_OPTS_OFFS              0x030
#define DCP_CHx_OPTS_SET_OFFS          0x034
#define DCP_CHx_OPTS_CLR_OFFS          0x038
#define DCP_CHx_OPTS_TOG_OFFS          0x03C


#define DCP_DBG_SEL_OFFS               0x400
#define DCP_DBG_DATA_OFFS              0x410

#define DCP_PAGE_TBL_OFFS              0x420
#define DCP_VERSION_OFFS               0x430

#endif /* IMXRT_DCP_H */

