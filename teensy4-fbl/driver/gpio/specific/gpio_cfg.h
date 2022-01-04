#ifndef GPIO_CFG_H
#define GPIO_CFG_H

// TODO: Make include board-/project-dependant.
#if (BSP_SOC_TYPE == BSP_SOC_IMXRT)
#include "gpio_cfg_imxrt.h"
#endif /* (BSP_SOC_TYPE) */

#endif /* GPIO_CFG_H */

