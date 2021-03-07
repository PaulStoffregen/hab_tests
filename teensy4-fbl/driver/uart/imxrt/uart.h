#ifndef UART_H
#define UART_H



/* UART status codes */
// TODO: need common platform independant strategy
#define UART_OK                0x00
#define UART_ERROR_FORMAT      0x01
#define UART_ERROR_RX_EMPTY    0x02
#define UART_ERROR_TX_BUSY     0x03
#define UART_ERROR_DATA_FAULTY 0x04
#define UART_ERROR_INT_ENA     0x05
#define UART_ERROR_DATA_LOST   0x06
#define UART_ERROR_RUNNING     0x07
#define UART_ERROR_NO_BUFFER   0x08
#define UART_ERROR_INVALID     0x09
#define UART_ERROR_OCCUPIED    0x0A
#define UART_ERROR_UNRESERVED  0x0B
#define UART_ERROR_NOMEM       0x0C


#define UART_OPMODE_RX   0x02
#define UART_OPMODE_TX   0x04

#define UART_OPMODE_MUX0 0x08
#define UART_OPMODE_MUX1 0x10

#define UART_OPMODE_SYNC 0x40
#define UART_OPMODE_CLK  0x01

#define UART_OPMODE_MASK 0x5F


enum UART_OP_MODE
{
  UART_OP_MODE_eASYNC_RD  = 0x1A, /* 0b000 11 01 0 */
  UART_OP_MODE_eASYNC_WR  = 0x1C, /* 0b000 11 10 0 */
  UART_OP_MODE_eASYNC_RW  = 0x1E, /* 0b000 11 11 0 */
  UART_OP_MODE_eASYNC_RD0 = 0x0A, /* 0b000 01 01 0 */
  UART_OP_MODE_eASYNC_WR0 = 0x0C, /* 0b000 01 10 0 */
  UART_OP_MODE_eASYNC_RW0 = 0x0E, /* 0b000 01 11 0 */
  
  UART_OP_MODE_eSYNCF_RD  = 0x4A, /* 0b010 01 01 0 */
  UART_OP_MODE_eSYNCF_WR  = 0x4C, /* 0b010 01 10 0 */
  UART_OP_MODE_eSYNCF_RW  = 0x4E, /* 0b010 01 11 0 */
  UART_OP_MODE_eSYNCR_RD  = 0x4B, /* 0b010 01 01 1 */
  UART_OP_MODE_eSYNCR_WR  = 0x4D, /* 0b010 01 10 1 */
  UART_OP_MODE_eSYNCR_RW  = 0x4F, /* 0b010 01 11 1 */
};

/* Parity flags */
#define UART_PARITY_NONE   0x00
#define UART_PARITY_EVEN   0x20
#define UART_PARITY_ODD    0x30

/* Parity mask */
#define UART_PARITY_MASK   0x30



/*! Structure for UART controller configuration data */
typedef struct
{
  uint32 baudrate; 
  uint32 speed;
  uint32 flags;
  uint8  dataBits;
  uint8  stopBits;
  uint8  parity;
  uint8  opMode;
  uint8  mode;
  uint8  clkPolarity;
  uint8  clkPhase;
}T_UART_CTL_CFG;


/* Public interface functions of uart module */
extern void uart_initDev(uint32 devID);
extern T_STATUS uart_configCtl(uint32 devID, const T_UART_CTL_CFG* ctlCfg);

void uart_setSpeed(uint32 devID, uint32 clockSpeed);
extern T_STATUS uart_recvByte(uint32 devID, uint8* rxByte);
extern T_STATUS uart_sendByte(uint32 devID, uint8 txByte);

int  uart_bufWrite(uint32 devID, const uint8* buf, uint32 len);

void uart_setStdDev(uint32 devID);

T_STATUS uart_setTxFifo(uint32 devID, uint8* buffer, uint16 size);
T_STATUS uart_clrTxFifo(uint32 devID);
T_STATUS uart_setRxFifo(uint32 devID, uint8* buffer, uint16 size);
T_STATUS uart_clrRxFifo(uint32 devID);

T_STATUS uart_disTxFifo(uint32 devID);
T_STATUS uart_enaTxFifo(uint32 devID);
T_STATUS uart_enaTxBlocking(uint32 devID);
T_STATUS uart_disTxBlocking(uint32 devID);

int  uart_getchar(void);
int  uart_putchar(int c);
int  uart_puts(const char* str);

#include "uart_cfg.h"

#endif /* UART_H */

