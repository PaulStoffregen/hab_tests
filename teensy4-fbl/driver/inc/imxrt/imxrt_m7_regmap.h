#ifndef IMXRT_M7_REGMAP_H
#define IMXRT_M7_REGMAP_H


#include "reg.h"

#define ROM_API_BASE           0x0020001C

/* High Assurance Boot ROM Vector Table */
#define HAB_RVT_BASE           0x00200300

/* AHB/ABP Peripherals */
#define PERIPH_BASE            0x40000000U

#define AIPS1_BASE             (PERIPH_BASE + 0x00000000U)
#define AIPS2_BASE             (PERIPH_BASE + 0x00100000U)
#define AIPS3_BASE             (PERIPH_BASE + 0x00200000U)
#define AIPS4_BASE             (PERIPH_BASE + 0x00300000U)
#define AIPS5_BASE             (PERIPH_BASE + 0x02000000U)

/* AIPS1 Configuration */
#define AIPS1_CONFIG_BASE      (AIPS1_BASE + 0x0007C000)

/* DCDC */
#define DCDC_BASE              (AIPS1_BASE + 0x00080000)

/* System Reset Controller */
#define SRC_BASE               (AIPS1_BASE + 0x000F8000)

/* Clock Control Module (Digital Part) */
#define CCM_DIGITAL_BASE       (AIPS1_BASE + 0x000FC000)

/* Clock Control Module (Analog Part) */
#define CCM_ANALOG_BASE        (AIPS1_BASE + 0x000D8000)

/* Secure Non-Volatile Storage  */
#define SNVS_BASE              (AIPS1_BASE + 0x000D4000)

/* System Control Module Padconf Core */
#define IOMUXC_GPR_BASE        (AIPS1_BASE + 0x000AC000)
#define IOMUXC_SNVS_BASE       (AIPS1_BASE + 0x000A8000)
#define IOMUXC_SNVS_GPR_BASE   (AIPS1_BASE + 0x000A4000)

/* IO Multiplexer Module */
#define IOMUXC_BASE            (AIPS2_BASE + 0x000F8000)

/* Gerneral Purpose Input Output */
#define GPIO1_BASE             (AIPS2_BASE + 0x000B8000)
#define GPIO2_BASE             (AIPS2_BASE + 0x000BC000)
#define GPIO3_BASE             (AIPS2_BASE + 0x000C0000)
#define GPIO4_BASE             (AIPS2_BASE + 0x000C4000)
#define GPIO5_BASE             (AIPS1_BASE + 0x000C0000)
#define GPIO6_BASE             (AIPS5_BASE + 0x00000000)
#define GPIO7_BASE             (AIPS5_BASE + 0x00004000)
#define GPIO8_BASE             (AIPS5_BASE + 0x00008000)
#define GPIO9_BASE             (AIPS5_BASE + 0x0000C000)

/* Universal Asynchronous Receiver Transmitter */
#define UART1_BASE             (AIPS2_BASE + 0x00084000)
#define UART2_BASE             (AIPS2_BASE + 0x00088000)
#define UART3_BASE             (AIPS2_BASE + 0x0008C000)
#define UART4_BASE             (AIPS2_BASE + 0x00090000)
#define UART5_BASE             (AIPS2_BASE + 0x00094000)
#define UART6_BASE             (AIPS2_BASE + 0x00098000)
#define UART7_BASE             (AIPS2_BASE + 0x0009C000)
#define UART8_BASE             (AIPS2_BASE + 0x000A0000)

/* Central Security Unit */
#define CSU_BASE               (AIPS1_BASE + 0x000DC000)

/* Data Co-Processor  */
#define DCP_BASE               (AIPS3_BASE + 0x000FC000)

/* Bit Encryption Engine  */
#define BEE_BASE               (AIPS4_BASE + 0x000EC000)

/* Level 2 Cache Controller */
//#define PL310_BASE               (ARM_MP_BASE + 0x2000)

/* System Reset Controller (SRC) */
#define SRC_BASE               (AIPS1_BASE + 0x000F8000)

/* On-Chip One Time Programmable Controller (OCOTP) */
#define OCOTP_BASE               (AIPS2_BASE + 0x000F4000)

/* AIPSTZ */
#define AIPSTZ1_BASE             (AIPS1_BASE + 0x0007C000)
#define AIPSTZ2_BASE             (AIPS2_BASE + 0x0007C000)
#define AIPSTZ3_BASE             (AIPS3_BASE + 0x0007C000)
#define AIPSTZ4_BASE             (AIPS4_BASE + 0x0007C000)

/* Ultra Secured Digital Host Controller (USDHC) */
#define USDHC1_BASE              (AIPS3_BASE + 0x000C0000)
#define USDHC2_BASE              (AIPS3_BASE + 0x000C4000)

/* Low-Power Serial Peripherel Interface (LPSPI) */
#define SPI1_BASE                (AIPS4_BASE + 0x00094000)
#define SPI2_BASE                (AIPS4_BASE + 0x00098000)
#define SPI3_BASE                (AIPS4_BASE + 0x0009C000)
#define SPI4_BASE                (AIPS4_BASE + 0x000A0000)

/* Low-Power Inter Integrated Circuit Controller (I2C) */
#define I2C1_BASE                (AIPS4_BASE + 0x000F0000)
#define I2C2_BASE                (AIPS4_BASE + 0x00F40000)
#define I2C3_BASE                (AIPS4_BASE + 0x00F80000)
#define I2C4_BASE                (AIPS4_BASE + 0x00FC0000)

/* Watchdog Timer (WDOG) */
#define WDOG1_BASE               (AIPS1_BASE + 0x000B8000)
#define WDOG2_BASE               (AIPS1_BASE + 0x000D0000)
#define WDOG3_BASE               (AIPS1_BASE + 0x000BC000)
/* Data Co-Processor with Cryptographic Accelleration (DCP) */
#define DCP_BASE                 (AIPS3_BASE + 0x000FC000)

/* True Random Number Generator (TRNG) */
#define TRNG_BASE                (AIPS1_BASE + 0x000CC000)

/* ARMv7M Private Peripheral Bus */
#define ARMV7_M_BASE              0xE0000000

#include "armv7m_regmap.h"

#endif /* IMXRT_M7_REGMAP_H */

