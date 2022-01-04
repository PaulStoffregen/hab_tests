
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp.h"
#include "getopt.h"
#include "defopt.h"

#include "trace_pub.h"
#include "cli.h"
#include "uart.h"
#include "cmdl.h"
#include "pdu.h"
#include "bcp.h"
#include "fbl_defs.h"
#include "fbl_cmd.h"
#include "upd_seq.h"


typedef struct
{
  uint16 comPortNum;
  char   fileName[256];
  FILE*  imgFile;
  uint32 baudRate;
//  uint8* fileBuffer;
}T_APP_CTRL;


static T_APP_CTRL appCtrl =
{
  .comPortNum = 1,
  .baudRate = 115200,
  .imgFile = NULL,
  .fileName = {0},
};


extern void fbl_command (char * filename);


static void print_usage(const char *prog);
static int  parse_opts(int argc, char *argv[]);

enum
{
  LOPT_0 = 256,
  LOPT_HELP,
};


#define OPT_HELP          'h'
#define OPT_HELP_ARGS      0
#define OPT_HELP_DESC      \
"  -h               prints this help information\n"

#define OPT_COMPORT       'c'
#define OPT_COMPORT_ARGS   1
#define OPT_COMPORT_DESC   \
"  -c<num>          use comport\n"

#define OPT_BAUDRATE      'b'
#define OPT_BAUDRATE_ARGS  1
#define OPT_BAUDRATE_DESC  \
"  -b<baudrate>     set baudrate\n"

#define OPT_IMGFILE       'f'
#define OPT_IMGFILE_ARGS   1
#define OPT_IMGFILE_DESC   \
"  -f <image-file>  set file to be programmed\n"

/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static const char optionTbl[] =
{
  MKOPT(HELP),
  MKOPT(COMPORT),
  MKOPT(BAUDRATE),
  MKOPT(IMGFILE),
  LASTOPT(),
};


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static const struct option lopts[] =
{
  LASTLOPT(),
};


/*
 ******************************************************************************
 * @fn print_usage
 ******************************************************************************
 * @par Description:
 *   ...
 *
 * @param
 *
 * @return
 *
 * @retval
 *
 ******************************************************************************
 */

static void print_usage(const char *prog)
{
  printf("Usage: %s [opts]\n", prog);
  puts(
    MKDESC(HELP)
    MKDESC(COMPORT)
    MKDESC(BAUDRATE)
    MKDESC(IMGFILE)
  );
  exit(1);
}


/*
 ******************************************************************************
 * @fn parse_opts
 ******************************************************************************
 * @par Description:
 *   ...
 *
 * @param
 *
 * @return
 *
 * @retval
 *
 ******************************************************************************
 */

static int parse_opts(int argc, char *argv[])
{
  int result = -1;
//  char* fileName;
  int opt;
//  int devID;
//  uint32 ord;
//  uint32 len;

  if(argc == 1)
  {
    /* Print usage and quit */
    print_usage(argv[0]);
  }

  while(1)
  {
    opt = getopt_long(argc, argv, optionTbl, lopts, NULL);

    if(-1 == opt)
    {
      break;
    }
    result = 0;
    
//    printf("Opt: %c\n", opt);
    switch(opt)
    {
    case OPT_HELP:
    //case LOPT_HELP:
      /* Print usage and quit */
      print_usage(argv[0]);
      result = 1;
      break;

    case OPT_COMPORT:
      if(optarg != NULL)
      {
        appCtrl.comPortNum = strtoul(optarg, NULL, 10);
      }
      else
      {
        /* No baudrate given, assume default */
      }
      break;

    case OPT_BAUDRATE:
      if(optarg != NULL)
      {
        appCtrl.baudRate = strtoul(optarg, NULL, 10);
      }
      else
      {
        /* No baudrate given, assume default */
      }
      break;

    case OPT_IMGFILE:
      if(optarg != NULL)
      {
        strncpy(appCtrl.fileName, optarg, sizeof(appCtrl.fileName));
      }
      else
      {
        /* No file given, assume none */
      }
      break;

    default:
      print_usage(argv[0]);
      result = -1;
      break;
    }
  }
  return result;
}



int main(int argc, char *argv[])
{
  int retVal = EXIT_SUCCESS;
  int status = 0;
  
  /* Parse command line options */
  status = parse_opts(argc, argv);

  if(status < 0)
  {
    /* Problem during option parsing */
  }
  else if(status > 0)
  {
    /* Help */
    status = 0;
  }
  else
  {
    T_UART_CTL_CFG  devCfg;
    printf("Open COM-Port COM%d with baudrate=%d\n", appCtrl.comPortNum, appCtrl.baudRate);

    /* Initialize device */
    uart_initDev(appCtrl.comPortNum);
#if 0
    devProps.rxBuffer = uart_rxBuf;
    devProps.rxBufSize = sizeof(uart_rxBuf);
    devProps.txBuffer = uart_txBuf;
    devProps.txBufSize = sizeof(uart_txBuf);
#endif

    /* Setup device config */
    devCfg.baudrate = appCtrl.baudRate;
    devCfg.dataBits = 8;
    devCfg.stopBits = 1;
    devCfg.parity = UART_PARITY_NONE;
    
    if(UART_OK != uart_configCtl(appCtrl.comPortNum, &devCfg))
    {
      /* Couldn't configure UART */
    }
    else
    {
      trace_init();
      bcp_init();
      fblctl_init();
      updseq_init(appCtrl.fileName);
      cmdl_init(appCtrl.comPortNum);
      
      while(1)
      {
        cmdl_run();
        bcp_run();
        fblctl_run();
        updseq_run();
      }
    }
  }
  
  return retVal;
}

