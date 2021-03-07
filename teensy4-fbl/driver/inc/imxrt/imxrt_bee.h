#ifndef IMXRT_BEE_H
#define IMXRT_BEE_H


#define BEE_CTRL_OFFS                  0x000

#define BEE_CTRL_R1_KEY_LOCK_BF         21, 1
#define BEE_CTRL_CIPHER_MODE_R1_LOCK_BF 30, 1
#define BEE_CTRL_SEC_LEVEL_R1_LOCK_BF   28, 2
#define BEE_CTRL_R0_KEY_LOCK_BF         27, 1
#define BEE_CTRL_CIPHER_MODE_R0_LOCK_BF 26, 1
#define BEE_CTRL_SEC_LEVEL_R0_LOCK_BF   24, 2
#define BEE_CTRL_LITLE_ENIAN_LOCK_BF    23, 1
#define BEE_CTRL_AC_PROT_ENA_LOCK_BF    22, 1
#define BEE_CTRL_KEY_REGION_LOCK_BF     21, 1
#define BEE_CTRL_KEY_VALID_LOCK_BF      20, 1
#define BEE_CTRL_R1_ADDR_LOCK_BF        19, 1
#define BEE_CTRL_SOFT_RST_LOCK_BF       18, 1
#define BEE_CTRL_CLK_ENA_LOCK_BF        17, 1
#define BEE_CTRL_BEE_EABLE_LOCK_BF      16, 1
#define BEE_CTRL_CIPHER_MODE_R1_BF      14, 1
#define BEE_CTRL_SEC_LEVEL_R1_BF        12, 2
#define BEE_CTRL_CIPHER_MODE_R0_BF      10, 1
#define BEE_CTRL_SEC_LEVEL_R0_BF         8, 2
#define BEE_CTRL_LITTELE_ENDIAN_BF       7, 1
#define BEE_CTRL_AC_PROT_ENA_BF          6, 1
#define BEE_CTRL_KEY_REGION_SEL_BF       5, 1
#define BEE_CTRL_KEY_VALID_BF            4, 1
#define BEE_CTRL_SOFT_RST_BF             2, 1
#define BEE_CTRL_CLK_ENA_BF              1, 1
#define BEE_CTRL_BEE_ENABLE_BF           0, 1


#define BEE_ADDR_OFFS0_OFFS            0x004
#define BEE_ADDR_OFFS1_OFFS            0x008
#define BEE_KEY0_W0_OFFS               0x00C
#define BEE_KEY0_W1_OFFS               0x010
#define BEE_KEY0_W2_OFFS               0x014
#define BEE_KEY0_W3_OFFS               0x018

#define BEE_STAT_OFFS                  0x01C

#define BEE_STAT_BEE_IDLE_BF            8, 1
#define BEE_STAT_IRQ_R3_AC_VIOL_BF      7, 1
#define BEE_STAT_IRQ_R2_AC_VIOL_BF      6, 1
#define BEE_STAT_IRQ_R1_AC_VIOL_BF      5, 1
#define BEE_STAT_IRQ_R0_AC_VIOL_BF      4, 1
#define BEE_STAT_IRQ_R1_SEC_VIOL_BF     3, 1
#define BEE_STAT_IRQ_RD_ILLEGAL_AC_BF   2, 1
#define BEE_STAT_IRQ_R0_SEC_VIOL_BF     1, 1
#define BEE_STAT_IRQ_DISBLE_ABT_BF      0, 1
#define BEE_STAT_IRQ_VEC_BF             0, 8


#define BEE_CTR_NONCE0_W0_OFFS         0x020
#define BEE_CTR_NONCE0_W1_OFFS         0x024
#define BEE_CTR_NONCE0_W2_OFFS         0x028
#define BEE_CTR_NONCE0_W3_OFFS         0x02C

#define BEE_CTR_NONCE1_W0_OFFS         0x030
#define BEE_CTR_NONCE1_W1_OFFS         0x034
#define BEE_CTR_NONCE1_W2_OFFS         0x038
#define BEE_CTR_NONCE1_W3_OFFS         0x03C

#define BEE_REGION1_TOP_OFFS           0x040
#define BEE_REGION1_BOT_OFFS           0x044

#define BEE_CTR_NONCE0_W1_OFFS         0x024
#define BEE_CTR_NONCE0_W2_OFFS         0x028
#define BEE_CTR_NONCE0_W3_OFFS         0x02C

#endif /* IMXRT_PEE_H */

