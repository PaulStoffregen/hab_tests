#ifndef GPIO_CFG_IMXRT_H
#define GPIO_CFG_IMXRT_H

#if defined(GPIO_C)

#define GPIO_NUM_DEVS_AVAIL         (9u)

const uint32 gpio_ctlDevBaseTable[GPIO_NUM_DEVS_AVAIL] =
{
  GPIO1_BASE,
  GPIO2_BASE,
  GPIO3_BASE,
  GPIO4_BASE,
  GPIO5_BASE,
  GPIO6_BASE,
  GPIO7_BASE,
  GPIO8_BASE,
  GPIO9_BASE,
};

#endif /* defined(GPIO_C) */

#endif /* GPIO_CFG_IMXSX_H */

