#ifndef IOMUX_H
#define IOMUX_H
#endif /* IOMUX_H */

/*
 * Structure defining the parameters of a single pin / pad configuration.
 */

typedef struct
{
  uint32 padConf; 
  uint16 muxConf;
  uint16 inpConf;
}T_IOMUX_DESC;

/* Bit-field definitions for the 'padConf' member:
 *
 *  |31      30|29  20|19     0|
 *  +----------+------+--------+
 *  | reserved | offs | config |
 *  +----------+------+--------+
 *      (2)      (10)    (20)
 */

#define PAD_CNF_OFFS_BF         20, 10
#define PAD_CNF_CONF_BF          0, 20


/* Bit-field definitions for the 'muxConf' member:
 *
 *  |    15    |14    10|9    0|
 *  +----------+--------+------+
 *  | reserved | config | offs |
 *  +----------+--------+------+
 *      (1)       (5)     (10)
 *             |         \
 *             |          \
 *             |           \
 *             |            \
 *             |             |
 *
 *  |    15    |  14  |13  10|9    0|
 *  +----------+------+------+------+
 *  | reserved | sion | mode | offs |
 *  +----------+------+------+------+
 *      (2)      (1)    (4)    (10)
 */

#define MUX_CNF_CONF_BF         10,  5
#define MUX_CNF_SION_BF         14,  1
#define MUX_CNF_MODE_BF         10,  4
#define MUX_CNF_OFFS_BF          0, 10


/* Bit-field definitions for the 'inpConf' member:
 *
 *  |15      13|12    10|9    0|
 *  +----------+--------+------+
 *  | reserved | config | offs |
 *  +----------+--------+------+
 *      (3)       (3)     (10)
 */
 
#define INP_CNF_SEL_BF          10,  3
#define INP_CNF_OFFS_BF          0, 10


/* Macros for the definition of a IOMUX configuration
 *
 * Use it like that:
 *
 * T_IOMUX_DESC module_muxConf[] =
 * {
 *   { PAD_CNF_DEF(<padconf-offs>, <pad-conf-value>), MUX_CNF_DEF(<muxconf_offs>, <mux-mode>, <sion>), INP_CNF_DEF(<inpconf-offs>, <inp-select>) },
 *   { PAD_CNF_DEF(<padconf-offs>, <pad-conf-value>), MUX_CNF_DEF(<muxconf_offs>, <mux-mode>, <sion>), INP_CNF_DEF(<inpconf-offs>, <inp-select>) },
 *     ... 
 *   { 0, 0, 0}
 * };
 *
 * T_IOMUX_DESC module_muxConf[] =
 * {
 *   {
 *     .padConf = PAD_CNF_DEF(<padconf-offs>, <pad-conf-value>),
 *     .muxConf = MUX_CNF_DEF(<muxconf_offs>, <mux-mode>, <sion>),
 *     .inpConf = INP_CNF_DEF(<inpconf-offs>, <inp-select>),
 *   },
 *   {
 *     .padConf = PAD_CNF_DEF(<padconf-offs>, <pad-conf-value>),
 *     .muxConf = MUX_CNF_DEF(<muxconf_offs>, <mux-mode>, <sion>),
 *     .inpConf = INP_CNF_DEF(<inpconf-offs>, <inp-select>),
 *   },
 *     ... 
 *   { 0, 0, 0}
 * };
 */

#define PAD_CNF_DEF(offs, conf) (BF_SET(((offs)>>2), PAD_CNF_OFFS_BF) | BF_SET((conf), PAD_CNF_CONF_BF))
#define MUX_CNF_DEF(offs, mode, sion) (BF_SET(((offs)>>2), MUX_CNF_OFFS_BF) | BF_SET((mode), MUX_CNF_MODE_BF) | BF_SET((sion), MUX_CNF_SION_BF))
#define INP_CNF_DEF(offs, sel) (BF_SET(((offs)>>2), INP_CNF_OFFS_BF) | BF_SET((sel), INP_CNF_SEL_BF))


void iomux_setupSinglePad(const T_IOMUX_DESC* desc);
void iomux_setupMultiplePads(const T_IOMUX_DESC* descArray);

