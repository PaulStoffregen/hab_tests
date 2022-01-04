#ifndef IMXRT_GPIO_H
#define IMXRT_GPIO_H


/*
 * Gerneral Purpose Input Output Registers
 */

#define GPIO_DR_OFFS            0x000 /*!< This register is used for setting the value of the GPIO output pins */
#define GPIO_GDIR_OFFS          0x004 /*!< This register is used to enable the pins output capabilities. Its only function is to carry the pads configuration. */
#define GPIO_PSR_OFFS           0x008 /*!< This register is used to register the data that is read from the GPIO pins. */
#define GPIO_ICR1_OFFS          0x00C /*!< Interrupt control register 1 */
#define GPIO_ICR2_OFFS          0x010 /*!< Interrupt control register 2 */
#define GPIO_IMR_OFFS           0x014 /*!< Interrupt mask register */
#define GPIO_ISR_OFFS           0x018 /*!< Interrupt status register */
#define GPIO_EDGE_SEL_OFFS      0x01C /*!< Edge select register */

#define GPIO_DR_SET_OFFS        0x084 /*!< Data register SET */
#define GPIO_DR_CLR_OFFS        0x088 /*!< Data register CLEAR */
#define GPIO_DR_TOG_OFFS        0x08C /*!< Data register TOGGLE */


#define GPIO_OE_OFFS            0x004 /*!< This register is used to enable the pins output capabilities. Its only function is to carry the pads configuration. */
#define GPIO_DIN_OFFS           0x008 /*!< This register is used to register the data that is read from the GPIO pins. */
#define GPIO_DOUT_OFFS          0x000 /*!< This register is used for setting the value of the GPIO output pins */


#define GPIO_PORT_P1    0
#define GPIO_PORT_P2    1
#define GPIO_PORT_P3    2
#define GPIO_PORT_P4    3
#define GPIO_PORT_P5    4
#define GPIO_PORT_P6    5
#define GPIO_PORT_P7    6
#define GPIO_PORT_P8    7

#define GPIO_PIN0     0
#define GPIO_PIN1     1
#define GPIO_PIN2     2
#define GPIO_PIN3     3
#define GPIO_PIN4     4
#define GPIO_PIN5     5
#define GPIO_PIN6     6
#define GPIO_PIN7     7
#define GPIO_PIN8     8
#define GPIO_PIN9     9
#define GPIO_PIN10   10
#define GPIO_PIN11   11
#define GPIO_PIN12   12
#define GPIO_PIN13   13
#define GPIO_PIN14   14
#define GPIO_PIN15   15
#define GPIO_PIN16   16
#define GPIO_PIN17   17
#define GPIO_PIN18   18
#define GPIO_PIN19   19
#define GPIO_PIN20   20
#define GPIO_PIN21   21
#define GPIO_PIN22   22
#define GPIO_PIN23   23
#define GPIO_PIN24   24
#define GPIO_PIN25   25
#define GPIO_PIN26   26
#define GPIO_PIN27   27
#define GPIO_PIN28   28
#define GPIO_PIN29   29
#define GPIO_PIN30   30
#define GPIO_PIN31   31

#endif /* IMXRT_GPIO_H */

