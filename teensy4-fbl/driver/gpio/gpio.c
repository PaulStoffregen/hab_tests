#ifndef GPIO_C
#define GPIO_C
#endif /* GPIO_C */


#include "bsp.h"
#include "reg.h"
#include "gpio.h"


/*!
 ******************************************************************************
 * Function: gpio_getPin
 ******************************************************************************
 * @par Description:
 *
 * @param pinID - Number identifying the port-pin.
 *
 * @return State of the pin
 *
 * @retval 1 if pin is high.
 * @retval 0 if pin is low.
 *
 ******************************************************************************
 */

uint8_least gpio_getPin(uint8_least pinID)
{
  uint8_least portNum;
  uint8_least pinNum;
  uint8_least base;
  uint8_least reg;

  portNum = GPIO_PORT_NUM(pinID);
  pinNum  = GPIO_PIN_NUM(pinID);

  base = gpio_ctlDevBaseTable[portNum];

  REG32_RD_BASE_OFFS(reg, base, GPIO_DIN_OFFS);
  return BW_GET(reg, pinNum, 1);
}


/*!
 ******************************************************************************
 * Function: gpio_setPin
 ******************************************************************************
 * @par Description:
 *
 * @param[in] pinID - Number identifying the port-pin.
 *
 * @return none
 *
 ******************************************************************************
 */

void gpio_setPin(uint8_least pinID)
{
  uint8_least portNum;
  uint8_least pinNum;
  uint8_least base;
  uint8_least reg;

  portNum = GPIO_PORT_NUM(pinID);
  pinNum  = GPIO_PIN_NUM(pinID);

  base = gpio_ctlDevBaseTable[portNum];

  REG32_RD_BASE_OFFS(reg, base, GPIO_DOUT_OFFS);
  reg |= (uint8)(((uint8)1) << pinNum);//BW_SET(1, pinNum, 1);
  REG32_WR_BASE_OFFS(reg, base, GPIO_DOUT_OFFS);
}


/*!
 ******************************************************************************
 * Function: gpio_clrPin()
 ******************************************************************************
 * @par Description:
 *
 * @param[in] pinID - Number identifying the port-pin.
 *
 * @return none
 *
 ******************************************************************************
 */

void gpio_clrPin(uint8_least pinID)
{
  uint8_least portNum;
  uint8_least pinNum;
  uint8_least base;
  uint8_least reg;

  portNum = GPIO_PORT_NUM(pinID);
  pinNum  = GPIO_PIN_NUM(pinID);

  base = gpio_ctlDevBaseTable[portNum];

  REG32_RD_BASE_OFFS(reg, base, GPIO_DOUT_OFFS);
  reg &= ~ BW_SET(1, pinNum, 1);
  REG32_WR_BASE_OFFS(reg, base, GPIO_DOUT_OFFS);
}


/*!
 ******************************************************************************
 * Function: gpio_pinDirOut
 ******************************************************************************
 * @par Description:
 *
 * @param[in] pinID - Number identifying the port-pin.
 *
 * @return none
 *
 ******************************************************************************
 */

void gpio_setPinDirOut(uint8_least pinID)
{
  uint8_least portNum;
  uint8_least pinNum;
  uint8_least base;
  uint8_least reg;

  portNum = GPIO_PORT_NUM(pinID);
  pinNum  = GPIO_PIN_NUM(pinID);

  base = gpio_ctlDevBaseTable[portNum];

  REG32_RD_BASE_OFFS(reg, base, GPIO_OE_OFFS);
  reg |= BW_SET(1, pinNum, 1);
  REG32_WR_BASE_OFFS(reg, base, GPIO_OE_OFFS);
}


/*!
 ******************************************************************************
 * Function: gpio_setPinDirIn
 ******************************************************************************
 * @par Description:
 *
 * @param[in] pinID - Number identifying the port-pin.
 *
 * @return none
 *
 ******************************************************************************
 */

void gpio_setPinDirIn(uint8_least pinID)
{
  uint8_least portNum;
  uint8_least pinNum;
  uint8_least base;
  uint8_least reg;

  portNum = GPIO_PORT_NUM(pinID);
  pinNum  = GPIO_PIN_NUM(pinID);

  base = gpio_ctlDevBaseTable[portNum];

  REG32_RD_BASE_OFFS(reg, base, GPIO_OE_OFFS);
  reg &= ~ BW_SET(1, pinNum, 1);
  REG32_WR_BASE_OFFS(reg, base, GPIO_OE_OFFS);
}

