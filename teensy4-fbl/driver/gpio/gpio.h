#ifndef GPIO_H
#define GPIO_H


/*
 * Structure defining the identifier of a single pin
 */

typedef struct
{
  uint8 pinID;
}T_GPIO_PINID;


/* Bit-field definitions for the 'pinID' member:
 *
 *  |7        5|4       0|
 *  +----------+---------+
 *  | port-num | pin-num |
 *  +----------+---------+
 *       (3)       (4)
 *
 */

#define GPIO_PINID_PORT_NUM_BF 5, 3
#define GPIO_PINID_PIN_NUM_BF  0, 5


/*!
 ******************************************************************************
 * Macro: GPIO_PIN_ID
 ******************************************************************************
 * @hideinitializer
 *
 * @brief Function macro generating a pin ID from straight port- and pin-
 *   number
 *
 * @par Description:
 *   The function like preprocessor macro is used to generate an appropriate
 *   pin ID from straight port- and pin- number to be used in the GPIO
 *   functions.
 *
 * @param portNum [IN] - Number defining the IO port
 * @param pinNum [IN] - Number defining the IO pin
 *
 * @return the pinID
 *
 ******************************************************************************
 */

#define GPIO_PIN_ID(portNum, pinNum) (BF_SET((portNum), GPIO_PINID_PORT_NUM_BF) + BF_SET((pinNum), GPIO_PINID_PIN_NUM_BF))


/*!
 ******************************************************************************
 * Macro: GPIO_PIN_NUM
 ******************************************************************************
 * @hideinitializer
 *
 ******************************************************************************
 */

#define GPIO_PIN_NUM(pinID) (BF_GET((pinID), GPIO_PINID_PIN_NUM_BF))


/*!
 ******************************************************************************
 * Macro: GPIO_PORT_NUM
 ******************************************************************************
 * @hideinitializer
 *
 ******************************************************************************
 */

#define GPIO_PORT_NUM(pinID) (BF_GET((pinID), GPIO_PINID_PORT_NUM_BF))


/*!
 ******************************************************************************
 * Function: gpio_getPin
 ******************************************************************************
 * @par Description:
 *   Gather the current state of the port-pin identified by 'pinID'.
 *
 * @param pinID - Number identifying the port-pin.
 *
 * @return State of the port-pin
 *
 * @retval 1 if pin is high.
 * @retval 0 if pin is low.
 *
 ******************************************************************************
 */

extern uint8_least gpio_getPin(uint8_least pinID);


/*!
 ******************************************************************************
 * Function: gpio_setPin
 ******************************************************************************
 * @par Description:
 *   Set the state of the port-pin identified by 'pinID' to HI.
 *
 * @param[in] pinID - Number identifying the port-pin.
 *
 * @return none
 *
 ******************************************************************************
 */

extern void gpio_setPin(uint8_least pinID);


/*!
 ******************************************************************************
 * Function: gpio_clrPin()
 ******************************************************************************
 * @par Description:
 *   Set the state of the port-pin identified by 'pinID' to LO.
 *
 * @param[in] pinID - Number identifying the port-pin.
 *
 * @return none
 *
 ******************************************************************************
 */

extern void gpio_clrPin(uint8_least pinID);


/*!
 ******************************************************************************
 * Function: gpio_pinDirOut
 ******************************************************************************
 * @par Description:
 *   Set the direction of the port-pin identified by 'pinID' to output.
 *
 * @param[in] pinID - Number identifying the port-pin.
 *
 * @return none
 *
 ******************************************************************************
 */

extern void gpio_setPinDirOut(uint8_least pinID);


/*!
 ******************************************************************************
 * Function: gpio_setPinDirIn
 ******************************************************************************
 * @par Description:
 *   Set the direction of the port-pin identified by 'pinID' to input.
 *
 * @param[in] pinID - Number identifying the port-pin.
 *
 * @return none
 *
 ******************************************************************************
 */

extern void gpio_setPinDirIn(uint8_least pinID);


#include "gpio_cfg.h"

#endif /* GPIO_H */

