#ifndef IMXRT_SNVS_H
#define IMXRT_SNVS_H

/* High Power register map */
#define SNVS_HP_LOCK_OFFS 0x000
#define SNVS_HP_CMD_OFFS  0x004
#define SNVS_HP_CTRL_OFFS  0x008
#define SNVS_HP_SEC_INT_CTRL_OFFS  0x00C
#define SNVS_HP_SEC_VIOL_CTRL_OFFS 0x010
#define SNVS_HP_STAT_OFFS  0x014
#define SNVS_HP_SEC_VIOL_STAT_OFFS  0x018
#define SNVS_HP_HAC_IV_OFFS         0x01C
#define SNVS_HP_HA_CNT_OFFS         0x020
#define SNVS_HP_RTC_MSB_OFFS        0x024
#define SNVS_HP_RTC_LSB_OFFS        0x028
#define SNVS_HP_TIME_ALARM_MSB_OFFS 0x02C
#define SNVS_HP_TIME_ALARM_LSB_OFFS 0x030


/* Low Power register map */
#define SNVS_LP_LOCK_OFFS              0x034
#define SNVS_LP_CTRL_OFFS              0x038
#define SNVS_LP_MKEY_CTRL_OFFS         0x03C
#define SNVS_LP_SEC_VIOL_CTRL_OFFS     0x040
#define SNVS_LP_TAMPER_DET_CFG_OFFS    0x048
#define SNVS_LP_STAT_OFFS              0x04C
#define SNVS_LP_SEC_RTC_MSB_OFFS       0x050
#define SNVS_LP_SEC_RTC_LSB_OFFS       0x054
#define SNVS_LP_TIME_ALARM_OFFS        0x058
#define SNVS_LP_SEC_MONO_CNT_MSB_OFFS  0x05C
#define SNVS_LP_SEC_MONO_CNT_LSB_OFFS  0x060
#define SNVS_LP_PWR_GLITCH_DET_OFFS    0x064
#define SNVS_LP_GPR0_LEGECY_ALIAS_OFFS 0x068

#define SNVS_LP_ZMK0_OFFS              0x06C
#define SNVS_LP_ZMK1_OFFS              0x070
#define SNVS_LP_ZMK2_OFFS              0x074
#define SNVS_LP_ZMK3_OFFS              0x078
#define SNVS_LP_ZMK4_OFFS              0x07C
#define SNVS_LP_ZMK5_OFFS              0x080
#define SNVS_LP_ZMK6_OFFS              0x084
#define SNVS_LP_ZMK7_OFFS              0x088

#define SNVS_LP_GPR0_ALIAS_OFFS        0x090
#define SNVS_LP_GPR1_ALIAS_OFFS        0x094
#define SNVS_LP_GPR2_ALIAS_OFFS        0x098
#define SNVS_LP_GPR3_ALIAS_OFFS        0x09C

#define SNVS_LP_GPR0_OFFS              0x100
#define SNVS_LP_GPR2_OFFS              0x104
#define SNVS_LP_GPR3_OFFS              0x108
#define SNVS_LP_GPR4_OFFS              0x10C

#define SNVS_HP_VERSION_ID1_OFFS       0xBF8
#define SNVS_HP_VERSION_ID2_OFFS       0xBFC

#endif /* IMXRT_SNVS_H */

