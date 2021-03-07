#ifndef ARMV7M_REGMAP_H
#define ARMV7M_REGMAP_H

/* If not already defined use this base address */ 
#if !defined(ARMV7_M_BASE)
#define ARMV7_M_BASE                         0xE0000000
#endif /* !defined(ARMV7_M_BASE) */


/* ARMv7-M System Control Space */
#define ARMV7_M_SCS_BASE                    (ARMV7_M_BASE + 0x0000E000)

/* ARMv7-M System Control Block (SCB) */
#define ARMV7_M_SCB_BASE                    (ARMV7_M_SCS_BASE + 0x0D00)

/* ARMv7-M System Timer */
#define ARMV7_M_SYS_TIM_BASE                (ARMV7_M_SCS_BASE + 0x0010)

/* ARMv7-M Nested Vector Interrupt Controller (NVIC) */
#define ARMV7_M_NVIC_BASE                   (ARMV7_M_SCS_BASE + 0x0100)

/* ARMv7-M Memory Protection Unit (MPU) */
#define ARMV7_M_MPU_BASE                    (ARMV7_M_SCS_BASE + 0x0100)


#endif /* ARMV7M_REGMAP_H */

