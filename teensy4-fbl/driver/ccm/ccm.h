#ifndef CCM_H
#define CCM_H


/*
 * ARM Clock Root Register
 *
 *  CCM_CACRR_OFFS
 *
 * Bus Clock Div Register
 *
 *  CCM_CBCDR_OFFS
 *
 * Bus Clock Mux Register
 *
 *  CCM_CBCMR_OFFS
 *
 * Control Div Register
 *
 *  CCM_CCDR_OFFS
 *
 * Clock Switcher Register
 *
 *  CCM_CCSR_OFFS
 *
 * D1 Clock Div Register
 *
 *  CCM_CDCDR_OFFS
 *
 * HSC Clock Div Register
 *
 *  CCM_CHSCCDR_OFFS
 *
 * SSI Clock Div Registers
 *
 *  CCM_CS1CDR_OFFS
 *  CCM_CS2CDR_OFFS
 *
 * Serial Clock Div Registers
 *
 *  CCM_CSCDR1_OFFS
 *  CCM_CSCDR2_OFFS
 *  CCM_CSCDR3_OFFS
 *
 * Serial Clock Mux Registers
 *
 *  CCM_CSCMR1_OFFS
 *  CCM_CSCMR2_OFFS
 *
 ******************************************************************************
 * 
 * podf -> 3 bit divider = 1 / (podf + 1)
 * clk_sel -> 2 bit input clock selector
 *
 * ipu<N>_hsp_clk
 *
 * CSCDR.ipu<N>_hsp_clk_sel:
 * 0 .. mmdc_ch0_clk
 * 1 .. 396M_PFD_CLK
 * 2 .. 120M_CLK
 * 3 .. 540M_PFD_CLK
 * 
 * usdhc<N>_clk
 * 
 * CSCMR1.usdhc<N>_clk_sel (1 bit):
 * 0 .. 396M_PFD_CLK
 * 1 .. 352M_PFD_CLK
 *
 * CSCDR1.usdhc<N>_podf (3 bit)
 * 
 * CCGR6.usdhc<N>_clk_en (2 bit)
 *
 * PODFs:
 * periph_clk2_podf (3 bit) CBCDR
 * mmdc_ch0_axi_podf (3 bit) CBCDR
 * axi_podf (3 bit) CBCDR
 * ahb_podf (3 bit) CBCDR
 * ipg_podf (2 bit) CBCDR
 *
 * Register offset ranges from 0x00 to 0x88 (0..136) thus fits into a single byte.
 * As all registers are 32-bit wide, the lower two bits in the offset are 0 and
 * thus the offset might be reduced to a bit-field with 6 bits.
 *
 * Peripheral clocks seam to have at most the following properties:
 *
 *  - a mux
 *  - a divider
 *  - a gate
 *
 * where each of the properties may be placed in a different register and the
 * related bit-fields may have different lsb and width. Thus a configuration
 * must contain:
 *
 *  3 x offset (3 x 6..8 bit)
 *
 *  3 x lsb    (3 x 5 bit)
 *
 *  1 x mux width  (1, 2 or 3 bit) (2 bit)
 *  1 x podf width (2, 3 or 6 bit) (2 or 3 bit)
 *  1 x gate width (2 bit)         (0 or 2 bit)
 *
 *  1 x sel val    (3 bit)
 *  1 x podf val   (6 bit)
 *  1 x gate val   (2 bit)
 *
 *  1 x offs  (8 bit)
 *  1 x lsb   (5 bit)
 *  1 x width (5 bit)
 *  1 x value (6 bit)
 */


/*
 * Structure defining the parameters of a single clock property configuration.
 * This can be one of the following properties:
 *  - SEL
 *  - PFD
 *  - GATE
 */
typedef struct T_CCM_CLK_CFG
{
  uint8 regOffs;
  uint8 bfLsb;
  uint8 bfWidth;
  uint8 cfgVal;
}T_CCM_CLK_CFG;


/* Macros for the definition of a CCM clock configuration
 *
 * Use it like that:
 *
 * T_CCM_CLK_CFG module_clkCfg[] =
 * {
 *   {CLK_CFG_DEF(<ccm-reg-offs>, <lsb>, <width>, <value>)},
 *   {CLK_CFG_DEF(<ccm-reg-offs>, <lsb>, <width>, <value>)},
 *    ...
 *   {0, 0, 0, 0}
 * };
 */

#define CLK_CFG_DEF(offs, lsb, width, cfg) (offs), (lsb), (width), (cfg)
#define CLK_CNF_DEF(offs, bf, cfg) \
  {                                \
    .regOffs = (offs),             \
    .bfLsb = BW_LSB(bf),           \
    .bfWidth = BW_WIDTH(bf),       \
    .cfgVal = (cfg),               \
  }

#define CLK_CNF_END() \
  {                   \
    .regOffs = 0,     \
    .bfLsb = 0,       \
    .bfWidth = 0,     \
    .cfgVal = 0,      \
  }

void ccm_setupSingleClkProp(const T_CCM_CLK_CFG* clkCfg);
void ccm_setupMultipleClkProps(const T_CCM_CLK_CFG* clkCfg);

#endif /* CCM_H */

