#ifndef IOMUX_C
#define IOMUX_C
#endif /* IOMUX_C */

#include "bsp.h"
#include "iomux.h"


/*
 ******************************************************************************
 * Function: iomux_setupSinglePad()
 ******************************************************************************
 * @par Description:
 *   ...
 *
 * @param desc [IN] - Pointer to single IOMUX configuration descriptor.
 *
 ******************************************************************************
 */

void iomux_setupSinglePad(const T_IOMUX_DESC* desc)
{
  uint32 conf;
  uint32 offs;

  /* Setup pad config for pin */
  offs = BF_GET(desc->padConf, PAD_CNF_OFFS_BF);
  conf = BF_GET(desc->padConf, PAD_CNF_CONF_BF);

  REG32_WR_BASE_OFFS(conf, IOMUXC_BASE, offs << 2);

  /* Setup mux config for pin */
  offs = BF_GET(desc->muxConf, MUX_CNF_OFFS_BF);
  if(offs != 0)
  {
    conf = BF_GET(desc->muxConf, MUX_CNF_CONF_BF);
    REG32_WR_BASE_OFFS(conf, IOMUXC_BASE, offs << 2);
  }

  /* Setup input config for pin */
  offs = BF_GET(desc->inpConf, INP_CNF_OFFS_BF);
  if(offs != 0)
  {
    conf = BF_GET(desc->inpConf, INP_CNF_SEL_BF);
    REG32_WR_BASE_OFFS(conf, IOMUXC_BASE, offs << 2);
  }
}


/*
 ******************************************************************************
 * Function: iomux_setupSinglePad()
 ******************************************************************************
 * @par Description:
 *   ...
 *
 * @param descArray [IN] - NULL-terminated array of IOMUX configuration
 *   descriptors.
 *
 ******************************************************************************
 */

void iomux_setupMultiplePads(const T_IOMUX_DESC* descArray)
{
  while(descArray->padConf != 0)
  {
    iomux_setupSinglePad(descArray);
    descArray++;
  }
}

