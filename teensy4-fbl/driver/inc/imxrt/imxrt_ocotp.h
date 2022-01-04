#ifndef IMXRT_OCOTP_H
#define IMXRT_OCOTP_H

/* OTOCP register map */
#define OCOTP_CTRL_OFFS               0x000
#define OCOTP_CTRL_SET_OFFS           0x004
#define OCOTP_CTRL_CLR_OFFS           0x008
#define OCOTP_CTRL_TOG_OFFS           0x00C

#define OCOTP_CTRL_WR_UNLOCK_BF      16, 16
#define OCOTP_CTRL_RELOAD_SHADOWS_BF 10,  1
#define OCOTP_CTRL_ERROR_BF           9,  1
#define OCOTP_CTRL_BUSY_BF            8,  1
#define OCOTP_CTRL_ADDR_BF            0,  6 /* Specifies one of the 128 word address locations (0x00 - 0x7F) */


#define OCOTP_TIMING_OFFS             0x010
#define OCOTP_DATA_OFFS               0x020
#define OCOTP_READ_CTRL_OFFS          0x030

#define OCOTP_READ_CTRL_READ_FUSE_BF  0,  1


#define OCOTP_READ_FUSE_DATA_OFFS     0x040
#define OCOTP_SW_STICKY_OFFS          0x050
#define OCOTP_SCS_OFFS                0x060
#define OCOTP_SCS_SET_OFFS            0x064
#define OCOTP_SCS_CLR_OFFS            0x068
#define OCOTP_SCS_TOG_OFFS            0x06C

#define OCOTP_SCS_LOCK_BF             31,  1
#define OCOTP_SCS_SPARE_BF             2, 30
#define OCOTP_SCS_HAB_JTAG_DBG_ENA_BF  0,  1

#define OCOTP_CRC_ADDR_OFFS           0x070
#define OCOTP_CRC_VALUE_OFFS          0x080
#define OCOTP_VERSION_OFFS            0x090
#define OCOTP_TIMING2_OFFS            0x100


/* OCOTP fuse map */

#define OCOTP_FUSE_MAP_OFFS           0x400

/* Lock fuses */
#define OCOTP_LOCK_FUSE_OFFS          0x400

#define LOCK_FUSE_FIELD_RETURN_BF     28, 4
#define LOCK_FUSE_GP3_OP_LOCK_BF      27, 1
#define LOCK_FUSE_GP3_WR_LOCK_BF      26, 1
#define LOCK_FUSE_GP3_LOCK_BF         26, 2
#define LOCK_FUSE_GP4_OP_LOCK_BF      25, 1
#define LOCK_FUSE_GP4_WR_LOCK_BF      24, 1
#define LOCK_FUSE_GP4_LOCK_BF         24, 2
#define LOCK_FUSE_SW_GP2_RD_LOCK_BF   23, 1
#define LOCK_FUSE_MISC_CFG_LOCK_BF    22, 1
#define LOCK_FUSE_SW_GP2_LOCK_BF      21, 1
#define LOCK_FUSE_OTPMK_CRC_LOCK_BF   20, 1
#define LOCK_FUSE_ANALOG_OP_LOCK_BF   19, 1
#define LOCK_FUSE_ANALOG_WR_LOCK_BF   18, 1
#define LOCK_FUSE_ANALOG_LOCK_BF      18, 2
#define LOCK_FUSE_OTPMK_LSB_LOCK_BF   17, 1
#define LOCK_FUSE_SW_GP1_LOCK_BF      16, 1
#define LOCK_FUSE_OTPMK_MSB_LOCK_BF   15, 1
#define LOCK_FUSE_SRK_LOCK_BF         14, 1
#define LOCK_FUSE_GP2_OP_LOCK_BF      13, 1
#define LOCK_FUSE_GP2_WR_LOCK_BF      12, 1
#define LOCK_FUSE_GP2_LOCK_BF         12, 2
#define LOCK_FUSE_GP1_OP_LOCK_BF      11, 1
#define LOCK_FUSE_GP1_WR_LOCK_BF      10, 1
#define LOCK_FUSE_GP1_LOCK_BF         10, 2
#define LOCK_FUSE_MAC_ADDR_LOCK_BF     8, 2
#define LOCK_FUSE_GP4_RD_LOCK_BF       7, 1
#define LOCK_FUSE_SJC_RESP_LOCK_BF     6, 1
#define LOCK_FUSE_MEM_TRIM_LOCK_BF     4, 2
#define LOCK_FUSE_BOOT_CFG_LOCK_BF     2, 2
#define LOCK_FUSE_TESTER_LOCK_BF       0, 2

/* UID / SJC challenge fuses */
#define OCOTP_HW_UID_LO_FUSE_OFFS     0x410
#define OCOTP_HW_UID_HI_FUSE_OFFS     0x420

#define HW_UID_HI_FUSE_DIE_X_COORD_BF 24,  8
#define HW_UID_HI_FUSE_DIE_Y_COORD_BF 16,  8
#define HW_UID_HI_FUSE_WAFER_NO_BF    12,  4
#define HW_UID_HI_FUSE_LOT_NO_BF       0, 11

/* Silicon revision fuse */
#define OCOTP_SI_REV_FUSE_OFFS        0x430

#define SI_REV_FUSE_SI_REV_BF         16,  4

/* Speed grading fuse */
#define OCOTP_SPEED_GRADE_FUSE_OFFS   0x440

#define SPEED_GRADE_FUSE_SPEED_BF     16,  2

/* Boot fuses */
#define OCOTP_BOOT0_FUSE_OFFS         0x450

#define BOOT0_FUSE_BOOT_CFG4_BF       24,  8
#define BOOT0_FUSE_BOOT_CFG3_BF       16,  8
#define BOOT0_FUSE_BOOT_CFG2_BF        8,  8
#define BOOT0_FUSE_BOOT_CFG1_BF        0,  8

#define OCOTP_BOOT1_FUSE_OFFS         0x460

#define BOOT1_FUSE_SD_PWR_CYC_SEL_BF  30,  2
#define BOOT1_FUSE_PWR_STABLE_SEL_BF  29,  1
#define BOOT1_FUSE_JTAG_HEO_BF        27,  1
#define BOOT1_FUSE_KTE_BF             26,  1
#define BOOT1_FUSE_NAND_ECC_DIS_BF    25,  1
#define BOOT1_FUSE_DLL_ENABLE_BF      24,  1
#define BOOT1_FUSE_JTAG_SMODE_BF      22,  2

#define BOOT1_FUSE_JTAG_SMODE_JTAG_ENA  0
#define BOOT1_FUSE_JTAG_SMODE_JTAG_SEC  1
#define BOOT1_FUSE_JTAG_SMODE_JTAG_DIS  3

#define BOOT1_FUSE_WDOG_ENABLE_BF     21,  1
#define BOOT1_FUSE_SJC_DISABLE_BF     20,  1
#define BOOT1_FUSE_DAP_SJC_SWD_SEL_BF 19,  1
#define BOOT1_FUSE_SDP_RD_DIS_BF      18,  1
#define BOOT1_FUSE_SDP_DISABLE_BF     17,  1
#define BOOT1_FUSE_FORCE_INT_BOOT_BF  16,  1
#define BOOT1_FUSE_BEE_KEY2_SEL_BF    14,  2
#define BOOT1_FUSE_BEE_KEY0_SEL_BF    12,  2
#define BOOT1_FUSE_SDRAM_CONFIG_BF     8,  4
#define BOOT1_FUSE_FORCE_COLD_BOOT_BF  5,  1
#define BOOT1_FUSE_BT_FUSE_SEL_BF      4,  1
#define BOOT1_FUSE_DIR_BT_DIS_BF       3,  1
#define BOOT1_FUSE_BOOT_FREQ_BF        2,  1
#define BOOT1_FUSE_SEC_CONFIG_BF       1,  1

#define OCOTP_BOOT2_FUSE_OFFS         0x470
#define OCOTP_MEM0_FUSE_OFFS          0x480

#define MEM0_FUSE_MEM_TRIM_BF         8,  8

#define OCOTP_MEM1_FUSE_OFFS          0x490
#define OCOTP_MEM2_FUSE_OFFS          0x4A0
#define OCOTP_MEM3_FUSE_OFFS          0x4B0
#define OCOTP_MEM4_FUSE_OFFS          0x4C0

#define OCOTP_ANA0_FUSE_OFFS          0x4D0
#define OCOTP_ANA1_FUSE_OFFS          0x4E0
#define OCOTP_ANA2_FUSE_OFFS          0x4F0
#define OCOTP_ANALOG_FUSE_OFFS        0x4F0

#define ANALOG_FUSE_USB_PID_BF        16, 16
#define ANALOG_FUSE_USB_VID_BF         0, 16

/* OTP Master Key fuses */
#define OCOTP_OTPMK0_FUSE_OFFS        0x500
#define OCOTP_OTPMK1_FUSE_OFFS        0x510
#define OCOTP_OTPMK2_FUSE_OFFS        0x520
#define OCOTP_OTPMK3_FUSE_OFFS        0x530
#define OCOTP_OTPMK4_FUSE_OFFS        0x540
#define OCOTP_OTPMK5_FUSE_OFFS        0x550
#define OCOTP_OTPMK6_FUSE_OFFS        0x560
#define OCOTP_OTPMK7_FUSE_OFFS        0x570

/* SRK fuses */
#define OCOTP_SRK0_FUSE_OFFS          0x580
#define OCOTP_SRK1_FUSE_OFFS          0x590
#define OCOTP_SRK2_FUSE_OFFS          0x5A0
#define OCOTP_SRK3_FUSE_OFFS          0x5B0
#define OCOTP_SRK4_FUSE_OFFS          0x5C0
#define OCOTP_SRK5_FUSE_OFFS          0x5D0
#define OCOTP_SRK6_FUSE_OFFS          0x5E0
#define OCOTP_SRK7_FUSE_OFFS          0x5F0

#define OCOTP_SJC_RSP0_FUSE_OFFS      0x600
#define OCOTP_SJC_RSP1_FUSE_OFFS      0x610

#define OCOTP_MAC0_FUSE_OFFS          0x620
#define OCOTP_MAC1_FUSE_OFFS          0x630
#define OCOTP_MAC2_FUSE_OFFS          0x640

#define OCOTP_GP3_FUSE_OFFS           0x640

#define OCOTP_OTPMK_CRC_FUSE_OFFS     0x650

#define OCOTP_GP1_FUSE_OFFS           0x660
#define OCOTP_GP2_FUSE_OFFS           0x670

#define OCOTP_SW_GP1_FUSE_OFFS        0x680
#define OCOTP_SW_GP20_FUSE_OFFS       0x690
#define OCOTP_SW_GP21_FUSE_OFFS       0x6A0
#define OCOTP_SW_GP22_FUSE_OFFS       0x6B0
#define OCOTP_SW_GP23_FUSE_OFFS       0x6C0
#define OCOTP_SW_GP20_FUSE_OFFS       0x690

#define OCOTP_GP43_FUSE_OFFS          0x7F0
#define OCOTP_FUSE_MAP_END            OCOTP_GP43_FUSE_OFFS

#endif /* IMXRT_OCOTP_H */

