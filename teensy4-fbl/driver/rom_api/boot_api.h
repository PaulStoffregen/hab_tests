#ifndef BOOT_API_H
#define BOOT_API_H


typedef void (*T_BOOT_API)(uint32* arg);


#define BOOT_API_ARG_TAG_BF           24, 8

#define BOOT_API_TAG_ENTER_BOOT        0xEB

#define BOOT_API_ARG_BOOT_MODE_BF     20, 4

#define BOOT_API_BOOT_MODE_INT         0
#define BOOT_API_BOOT_MODE_SDP         1

#define BOOT_API_ARG_SDP_DEV_BF       16, 4

#define BOOT_API_SDP_DEV_AUTO          0
#define BOOT_API_SDP_DEV_USB           1
#define BOOT_API_SDP_DEV_UART          2

#define BOOT_API_ARG_BOOT_IMG_SEL_BF  20, 4


#define BOOT_API_ENTER_SDP_ON_USB                              \
   ( 0                                                         \
   | BF_SET(BOOT_API_TAG_ENTER_BOOT, BOOT_API_ARG_TAG_BF)      \
   | BF_SET(BOOT_API_BOOT_MODE_SDP, BOOT_API_ARG_BOOT_MODE_BF) \
   | BF_SET(BOOT_API_SDP_DEV_USB, BOOT_API_ARG_SDP_DEV_BF)     \
   )

#define BOOT_API_ENTER_SDP_ON_UART                             \
   ( 0                                                         \
   | BF_SET(BOOT_API_TAG_ENTER_BOOT, BOOT_API_ARG_TAG_BF)      \
   | BF_SET(BOOT_API_BOOT_MODE_SDP, BOOT_API_ARG_BOOT_MODE_BF) \
   | BF_SET(BOOT_API_SDP_DEV_UART, BOOT_API_ARG_SDP_DEV_BF)    \
   )

#define BOOT_API_REBOOT                                        \
   ( 0                                                         \
   | BF_SET(BOOT_API_TAG_ENTER_BOOT, BOOT_API_ARG_TAG_BF)      \
   | BF_SET(BOOT_API_BOOT_MODE_INT, BOOT_API_ARG_BOOT_MODE_BF) \
   | BF_SET(BOOT_API_SDP_DEV_AUTO, BOOT_API_ARG_SDP_DEV_BF)    \
   | BF_SET(0, BOOT_API_ARG_BOOT_IMG_SEL_BF)                   \
   )

#endif /* BOOT_API_H */

