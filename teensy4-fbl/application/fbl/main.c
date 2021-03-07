#ifndef MAIN_C
#define MAIN_C
#endif /* MAIN_C */

#include "bsp.h"
#include "config.h"
#include "irqc.h"
#include "iomux.h"
#include "uart.h"
#include "cpu_irq.h"
#include "cmdl.h"
#include "trace_pub.h"
#include "pdu.h"
#include "bmgr.h"
#include "bcp.h"
#include "fbl.h"
#include "misc.h"
#include "arm_nvic.h"
#include "arm_sys_timer.h"


void cpu_init(void)
{
  cpu_enableIRQs();
  return;
}


const char startMsg[] = "Hello FBL\n";
void print_startMsg(void)
{
  const char* str = startMsg;
  while(*str != '\0')
  {
    uart_sendByte(STD_UART, (uint8)*str++);
  }
}


/* Pad configuration value for the UART pads */
#define GPIO_PAD_CONF (0                            \
  | BF_SET(PADCONF_eHYS_SCHMITT, PADCONF_HYS_BF)    \
  | BF_SET(PADCONF_eODE_CMOS, PADCONF_ODE_BF)       \
  | BF_SET(PADCONF_ePKE_ENA, PADCONF_PKE_BF)        \
  | BF_SET(PADCONF_ePUE_PULL, PADCONF_PUE_BF)       \
  | BF_SET(PADCONF_ePUS_PU_100K, PADCONF_PUS_BF)    \
  | BF_SET(PADCONF_eSPEED_100MHZ, PADCONF_SPEED_BF) \
  | BF_SET(PADCONF_eDSE_R0_BY_6, PADCONF_DSE_BF)       \
  | BF_SET(PADCONF_eSRE_SLOW, PADCONF_SRE_BF)       \
  )


/* Pin configuration */
T_IOMUX_DESC gpio_pinCfg[] =
{
  /* UART1_RXD */
  [0] =
  {
    .padConf = PAD_CNF_DEF(PADCONF_GPIO1_IO11__GPIO_AD_B0_11_ALT5_OFFS, GPIO_PAD_CONF),
    .muxConf = MUX_CNF_DEF(MUXCONF_GPIO1_IO11__GPIO_AD_B0_11_ALT5_OFFS, 5, 0),
    .inpConf = INP_CNF_DEF(0, 0),
  },
  { 0 },
};


int main(void)
{
  int cnt = 0;

  /* Setup NVIC */
  nvic_setPriorityGrouping(3);

  /* Initialize interrupt controller */  
  irqc_init();

  /* Initialize debug uart */
  uart_initDev(STD_UART);
  uart_configCtl(STD_UART, &uart_ctlDevCfgTbl[0]);

  trace_init();
  
  /* Initialize communication uart */
  uart_initDev(COM_UART);
  uart_configCtl(COM_UART, &uart_ctlDevCfgTbl[1]);

  /* Initialize system timer for 1ms interval */
  arm_initSysTimer(0, 396000000/1000);

  /* Initialize CPU and print start message */
  cpu_init();
  print_startMsg();

  /* Initialize commandline */
  cmdl_init();

  /* Initialize boot control protocol */
  bcp_init();

  /* Initialize boot manager */
  bmgr_init();

  /* Initialize flash bootloader */
  fbl_init();

  while(1)
  {
    /* Check for timer expiration */
    if(0 != arm_pollSysTimer())
    {
      bmgr_run();
      fbl_run();

      if(cnt < 1000)
      {
        cnt++;
      }
      else
      {
        cnt = 0;
      }
    }

    bcp_run();
    cmdl_run();
  }
}

int armv7m_main(void) __attribute__((alias("main")));

