#ifndef UART_CFG_MINGW_H
#define UART_CFG_MINGW_H

//TODO: This is Board-/Project-specific and shall be defined in that manner
#define UART_NUM_DEVS_USED          (1u)
#if defined (UART_DDM_C)

/* Reserve space for runtime data for all used UART devices.
 * A chunk is given to the driver with every initialization and used
 * by the driver to store device specific data
 */
static T_UART_CTL_DATA uart_ctlDevData[UART_NUM_DEVS_USED];


#endif /* defined (UART_DDM_C) */
#endif /* UART_CFG_MINGW_H */

