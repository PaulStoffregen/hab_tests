#ifndef ARMV7M_SCB_H
#define ARMV7M_SCB_H


/*
 * System Control Block (SCB)
 */

/* CPUID Base Register */
#define SCB_CPUID_OFFS           0x000

/* Interrupt Control State Register */
#define SCB_ICSR_OFFS            0x004

#define SCB_ICSR_NMIPENDSET_BF   31, 1
#define SCB_ICSR_PENDSVSET_BF    28, 1
#define SCB_ICSR_PENDSVCLR_BF    27, 1
#define SCB_ICSR_PENDSTSET_BF    26, 1
#define SCB_ICSR_PENDSTCLR_BF    25, 1

/* Vector Table Offset Register */
#define SCB_VTOR_OFFS            0x008

/* Application Interrupt/Reset Control Register */
#define SCB_AIRCR_OFFS           0x00C

#define SCB_AIRCR_VECTKEY_BF     16, 16
#define SCB_AIRCR_ENDIANESS_BF   15,  1
#define SCB_AIRCR_PRIGROP_BF      8,  3
#define SCB_AIRCR_SYSRESETREQ_BF  2,  1

#define SCB_AIRCR_VECTKEY_VALUE  0x5FAUL

/* System Control Register */
#define SCB_SCR_OFFS             0x010

/* Configuration Control Register */
#define SCB_CCR_OFFS             0x014


/* System Handlers Priority Registers */
#define SCB_SHPR_BASE_OFFS       0x018

/* System Handlers 4-7 Priority Register */
#define SCB_SHPR0_OFFS           0x018
/* System Handlers 8-11 Priority Register */
#define SCB_SHPR1_OFFS           0x01C
/* System Handlers 12-15 Priority Register */
#define SCB_SHPR2_OFFS           0x020

#define SCB_SHPR04_OFFS          0x018
#define SCB_SHPR05_OFFS          0x019
#define SCB_SHPR06_OFFS          0x01A
#define SCB_SHPR07_OFFS          0x01B
#define SCB_SHPR08_OFFS          0x01C
#define SCB_SHPR09_OFFS          0x01D
#define SCB_SHPR10_OFFS          0x01E
#define SCB_SHPR11_OFFS          0x01F
#define SCB_SHPR12_OFFS          0x020
#define SCB_SHPR13_OFFS          0x021
#define SCB_SHPR14_OFFS          0x022
#define SCB_SHPR15_OFFS          0x023

/* Satem Handler Control and State Register */
#define SCB_SHCSR_OFFS           0x024

/* Configurable Fault Status Register */
#define SCB_CFSR_OFFS            0x028

#define SCB_CFSR_UFSR_BF         16, 16
#define SCB_CFSR_BFSR_BF          8,  8
#define SCB_CFSR_MMFSR_BF         0,  8

/* Hard Fault Status Register */
#define SCB_HFSR_OFFS            0x02C

#define SCB_HFSR_VECTTBL_BF       1,  1
#define SCB_HFSR_FORCED_BF       30,  1
#define SCB_HFSR_DEBUGEVT_BF     31,  1

/* Debug Fault Status Register */
#define SCB_DFSR_OFFS            0x030

/* Memory Manage Fault Address Register */
#define SCB_MMFAR_OFFS           0x034

/* Bus Fault Address Register */
#define SCB_BFAR_OFFS            0x038

/* Auxiliary Fault Status Register */
#define SCB_AFSR_OFFS            0x03C


/* Cache Level ID Register (CLIDR) */
#define SCB_CLIDR_OFFS           0x078

#define SCB_CLIDR_CTYPE1_BF       0,  3
#define SCB_CLIDR_CTYPE2_BF       3,  3
#define SCB_CLIDR_CTYPE3_BF       6,  3
#define SCB_CLIDR_CTYPE4_BF       9,  3
#define SCB_CLIDR_CTYPE5_BF      12,  3
#define SCB_CLIDR_CTYPE6_BF      15,  3
#define SCB_CLIDR_CTYPE7_BF      18,  3
#define SCB_CLIDR_LOUIS_BF       21,  3
#define SCB_CLIDR_LOC_BF         24,  3
#define SCB_CLIDR_LOUU_BF        27,  3

#define CTYPE_NONE               0x0
#define CTYPE_ICACHE_ONLY        0x1
#define CTYPE_DCACHE_ONLY        0x2
#define CTYPE_SEPARATE_IDCACHE   0x3
#define CTYPE_UNIFIED_IDCACHE    0x4


/* Cache Type Register (CTR) */
#define SCB_CTR_OFFS             0x07C

#define SCB_CTR_LOG2_MIN_ICACHE_LINE_SIZE_BF  0,  3
#define SCB_CTR_LOG2_MIN_DCACHE_LINE_SIZE_BF 16,  4
#define SCB_CTR_EXCL_RESERVATION_GANULE_BF   20,  4
#define SCB_CTR_CACHE_WRITE_BACK_GANULE_BF   24,  4
#define SCB_CTR_FORMAT_BF                    29,  3


/* Cache Size ID Registers (CCSIDR) */
#define SCB_CCSIDR_OFFS          0x080

#define SCB_CCSIDR_LINE_SIZE_BF     0,  3
#define SCB_CCSIDR_ASSOCIATIVITY_BF 3, 10
#define SCB_CCSIDR_NUM_SETS_BF     13, 15
#define SCB_CCSIDR_WA_SUPPORT_BF   28,  1
#define SCB_CCSIDR_RA_SUPPORT_BF   29,  1
#define SCB_CCSIDR_WB_SUPPORT_BF   30,  1
#define SCB_CCSIDR_WT_SUPPORT_BF   31,  1

/* Cache Size Selection Register (CSELR) */
#define SCB_CSELR_OFFS           0x084

#define SCB_CSELR_InD_BF          0,  1
#define SCB_CSELR_LEVEL_BF        1,  3


/* Coprocessor Access Control Register */
#define SCB_CPACR_OFFS           0x088

/* Software Trigger Interrupt Register */
#define SCB_STIR_OFFS            0x200

#define SCB_FPCCR_OFFS           0x234
#define SCB_FPCAR_OFFS           0x238
#define SCB_FPDSCR_OFFS          0x23C

#define SCB_MVFR0_OFFS           0x240
#define SCB_MVFR1_OFFS           0x244
#define SCB_MVFR2_OFFS           0x248

#endif /* ARMV7M_SCB_H */

