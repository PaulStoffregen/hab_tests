#ifndef CRC8_CFG_H
#define CRC8_CFG_H

#include "crc_cfg.h"

/*
 * Polynomial: x^8 + x^6 + x^3 + x^2 + 1
 * Nickname:   
 * Koopman:    0xA6 [10100110]
 * Normal:     0x4D [01001101]
 * HD3 up to 247 bits
 */

#if (STD_ON == CRC8_P4D)
#include "crc8_p4D.h"
#endif


/*
 * Polynomial: x^8 + x^7 + x^6 + x^4 + x^2 + 1
 * Nickname:   CRC-8
 * Koopman:    0xEA [11101010]
 * Normal:     0xD5 [11010101]
 * HD4 up to 85 bits
 * 
 */

#if (STD_ON == CRC8_PD5)
#include "crc8_pD5.h"
#endif


/*
 * Polynomial: x^8 + x^7 + x^6 + x^4 + x^2 + 1
 * Nickname:   C2
 * Koopman:    0x97 [10010111]
 * Normal:     0x2F [00101111]
 * HD4 up to 119 bits
 * 
 */

#if (STD_ON == CRC8_P2F)
#include "crc8_p2F.h"
#endif

/*
 * Polynomial: x^8 + x^2 + x + 1
 * Nickname:   ATM-8 HEC
 * Koopman:    0x83 [10000011]
 * Normal:     0x07 [00000111]
 * HD4 up to 119 bits
 * 
 */

#if (STD_ON == CRC8_P07)
#include "crc8_p07.h"
#endif


/*
 * Polynomial: x^8 + x^4 + x^3 + x^2 + 1
 * Nickname:   SAE J1850
 * Koopman:    0x8E [10001110]
 * Normal:     0x1D [00011101]
 * HD4 up to 119 bits
 * 
 */

#if (STD_ON == CRC8_P1D)
#include "crc8_p1D.h"
#endif

#endif /* CRC16_CFG_H */
