#ifndef CRC16_CFG_H
#define CRC16_CFG_H

#include "crc_cfg.h"


#if (STD_ON == CRC16_P1021)
#include "crc16_p1021.h"
#endif

#if (STD_ON == CRC16_P8005)
#include "crc16_p8005.h"
#endif

#endif /* CRC16_CFG_H */
