#ifndef UART_DDM_H
#define UART_DDM_H
#endif /* UART_DDM_H */

/* Device Data Manager for the UART driver */
#include "uart.h"

extern const T_UART_CTL_DESC* uart_getDevProps(uint32 devID);
extern T_UART_CTL_DATA* uart_getDevData(uint32 devID);
extern uint32 uart_getDevBase(uint32 devID);
extern T_STATUS uart_allocDevData(uint32 devID);
extern T_STATUS uart_freeDevData(uint32 devID);

