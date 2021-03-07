#ifndef CONFIG_H
#define CONFIG_H

#if (BSP_BOARD_TYPE == BSP_BRD_MIMXRT1060_EVK)
#define STD_UART 2
#define COM_UART 0
#elif (BSP_BOARD_TYPE == BSP_BRD_TEENSY40)
#define STD_UART 2
#define COM_UART 0
#endif /* (BSP_BOARD_TYPE) */

#define MAIN_STACK_SIZE 1024

#endif /* CONFIG_H */

