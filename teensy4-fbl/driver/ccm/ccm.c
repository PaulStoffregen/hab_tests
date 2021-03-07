#ifndef CCM_C
#define CCM_C
#endif /* CCM_C */

#include "bsp.h"
#include "ccm.h"


/*
 ******************************************************************************
 * Function: ccm_setupSingleClkProp()
 ******************************************************************************
 * @par Description:
 *   ...
 *
 * @param desc [IN] - Pointer to single CCM configuration descriptor.
 *
 ******************************************************************************
 */

void ccm_setupSingleClkProp(const T_CCM_CLK_CFG* clkCfg)
{
  REG_WRBW(clkCfg->cfgVal, __REG32(CCM_DIGITAL_BASE + clkCfg->regOffs), clkCfg->bfLsb, clkCfg->bfWidth);
}


/*
 ******************************************************************************
 * Function: ccm_setupMultipleClkProps()
 ******************************************************************************
 * @par Description:
 *   ...
 *
 * @param desc [IN] - NULL-terminated array of CCM configuration descriptors.
 *
 ******************************************************************************
 */

void ccm_setupMultipleClkProps(const T_CCM_CLK_CFG* clkCfg)
{
  while(clkCfg->regOffs != 0)
  {
    ccm_setupSingleClkProp(clkCfg);
    clkCfg++;
  }
}

