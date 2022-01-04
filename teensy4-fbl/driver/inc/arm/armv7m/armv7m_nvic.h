#ifndef ARMV7M_NVIC_H
#define ARMV7M_NVIC_H


/* Interrupt Set Enable Register */
#define NVIC_ISER_BASE_OFFS  0x000

/* Interrupt Clear Enable Register */
#define NVIC_ICER_BASE_OFFS  0x080

/* Interrupt Set Pending Register */
#define NVIC_ISPR_BASE_OFFS  0x100

/* Interrupt Clear Pending Register */
#define NVIC_ICPR_BASE_OFFS  0x180

/* Interrupt Active Bit Register */
#define NVIC_IABR_BASE_OFFS  0x200

/* Interrupt Priority Register */
#define NVIC_IPR_BASE_OFFS   0x300

#endif /* ARMV7M_NVIC_H */

