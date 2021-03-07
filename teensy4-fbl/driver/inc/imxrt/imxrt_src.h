#ifndef IMXRT_SRC_H
#define IMXRT_SRC_H

/* SRC control register */
#define SRC_SCR_OFFS     0x000

/* SRC Boot Mode Register 1 */
#define SRC_SBMR1_OFFS   0x004

#define SRC_SBMR1_BOOT_CFG4_BF  24, 8
#define SRC_SBMR1_BOOT_CFG3_BF  16, 8
#define SRC_SBMR1_BOOT_CFG2_BF   8, 8
#define SRC_SBMR1_BOOT_CFG1_BF   0, 8

/* SRC Reset Status Register */
#define SRC_SRSR_OFFS    0x008

/* SRC Boot Mode Register 2 */
#define SRC_SBMR2_OFFS   0x01C

#define SRC_SBMR2_BMOD_BF         24, 2
#define SRC_SBMR2_BT_FUSE_SEL_BF   4, 1
#define SRC_SBMR2_DIR_BT_DIS_BF    3, 1
#define SRC_SBMR2_SEC_CONFIG_BF    1, 1


#endif /* IMXRT_SRC_H */

