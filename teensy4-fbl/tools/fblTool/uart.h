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

void uart_setStdDev(uint32 devID);

int  uart_getchar(void);
int  uart_putchar(int c);

//#include "uart_cfg.h"

#endif /* UART_H */

