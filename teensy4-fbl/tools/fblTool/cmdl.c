#ifndef CMDL_C
#define CMDL_C
#endif /* CMDL_C */


#include "bsp.h"
#include "cli.h"
#if (CMDL_USE_LIBC != 0)
#include "libc.h"
#elif (CMDL_USE_TRACE != 0)
#include "trace.h"
#else
#endif
#include "cmdio.h"
#include "pdu.h"
#include "bcp.h"
#include "cmdl.h"
#include "fbl_defs.h"
#include "fbl_cmd.h"
#include "upd_seq.h"

#include <string.h> /* for memset() */
#include <stdio.h>  /* for printf() */

#define CMDL_MAX_CMD_BUF_LEN 32
#define CMDL_PROMPT "FBLTOOL>"

typedef enum CMDL_STATE
{
  CMDL_STATE_eRESET = 0,
  CMDL_STATE_eIDLE,
  CMDL_STATE_eBUSY,
}T_CMDL_STATE;


typedef struct
{
  uint8 echoOff;
  T_CMDL_STATE state;
  char cmdBuffer[CMDL_MAX_CMD_BUF_LEN];
  int bufPos;
  uint32 devID;
}T_CMDL_DATA;


static T_CMDL_DATA cmdl_data;

static T_CMDL_DATA* cmdl_getCmdlData(void)
{
  return &cmdl_data;
}


/* Convinience functions */

/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_readChar(void)
{
  int result = -1;
  int c;

  /* Get character via UART */
  c = cmdio_getchar();
  if(-1 != c)
  {
    result = c;
  }
  else
  {
    /* Nothing received */
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static void cmdl_writeChar(char c)
{
  (void)cmdio_putchar(c);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void cmdl_writeStr(char* str)
{
  while(*str != 0)
  {
    cmdl_writeChar(*str);
    str++;
  }
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static void cmdl_pompt(void)
{
#if (CMDL_USE_LIBC != 0)
  libc_puts(CMDL_PROMPT);
#else
  cmdl_writeStr(CMDL_PROMPT);
#endif
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static void cmdl_echo(char c)
{
  T_CMDL_DATA* cmdData = &cmdl_data;

  /* Process echo */
  if(cmdData->echoOff != 0)
  {
    /* Echo is off */
  }
  else if(c == '\r')
  {
    /* Received CR */
#if (CMDL_USE_LIBC != 0)
    libc_putchar('\n');
#else
    cmdl_writeChar('\n');
#endif
  }
  else if(c == 0x7F)
  {
    /* Received DEL */
#if (CMDL_USE_LIBC != 0)
    libc_puts("\b \b");
#else
    cmdl_writeStr("\b \b");
#endif
  }
  else
  {
    /* Received other charachter */
#if (CMDL_USE_LIBC != 0)
    libc_putchar(c);
#else
    cmdl_writeChar(c);
#endif
  }
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static void cmdl_startCmd(void)
{
  T_CMDL_DATA* cmdData = cmdl_getCmdlData();

  cmdData->state = CMDL_STATE_eBUSY;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static void cmdl_finishCmd(uint32 errCode)
{
  T_CMDL_DATA* cmdData = cmdl_getCmdlData();

  fblctl_registerCallback(NULL);

  cmdData->state = CMDL_STATE_eIDLE;
  /* Start new command line */
  cmdl_pompt();
  
  /* Reset buffer position */
  cmdData->bufPos = 0;
}


/* Implemantation of the commands */

/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_helpCmd(const char* argStr)
{
  cmdl_writeStr("General commands\n");
  cmdl_writeStr("  ?          print this help\n");
  cmdl_writeStr("  help       print this help\n");
  cmdl_writeStr("  open       attach to the FBL during the first 10s after reset\n");
  cmdl_writeStr("  info       show some information about the SW programmed to the target\n");
  cmdl_writeStr("Command sequence\n");
  cmdl_writeStr("  update     run the whole update sequence\n");
  cmdl_writeStr("Single commands\n");
  cmdl_writeStr("  inv        invalidate current application SW\n");
  cmdl_writeStr("  erase      erase current application SW\n");
  cmdl_writeStr("  program    program a few dummy blocks\n");
  cmdl_writeStr("  activate   activate the new application SW\n");
  cmdl_writeStr("  restart    reset and restart the target\n");
  cmdl_writeStr("  reboot     reset and restart the target\n");
  return CLI_OK;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_echoCmd(const char* argStr)
{
  T_CMDL_DATA* cmdData = &cmdl_data;

  if(cmdData->echoOff != 0)
  {
#if (CMDL_USE_LIBC != 0)
    libc_puts("echo on\n");
#else
    cmdl_writeStr("echo on\n");
#endif
    cmdData->echoOff = 1;
  }
  else
  {
#if (CMDL_USE_LIBC != 0)
    libc_puts("echo off\n");
#else
    cmdl_writeStr("echo off\n");
#endif
    cmdData->echoOff = 0;
  }
  return CLI_OK;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_openCmd(const char* argStr)
{
  int result;

  fblctl_registerCallback(&cmdl_finishCmd);
  if(STATUS_eOK != fblctl_sendBootstrapReq())
  {
    result = CLI_OK;
  }
  else
  {
    result = CLI_PENDING;
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_infoCmd(const char* argStr)
{
  int result;

  fblctl_registerCallback(&cmdl_finishCmd);
  if(STATUS_eOK != fblctl_sendSwInfoReq())
  {
    result = CLI_OK;
  }
  else
  {
    result = CLI_PENDING;
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_invalidateCmd(const char* argStr)
{
  int result;

  fblctl_registerCallback(&cmdl_finishCmd);
  if(STATUS_eOK != fblctl_sendInvalidateReq(0x60010000, 0x800))
  {
    result = CLI_OK;
  }
  else
  {
    result = CLI_PENDING;
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_eraseCmd(const char* argStr)
{
  int result;

  fblctl_registerCallback(&cmdl_finishCmd);
  if(STATUS_eOK != fblctl_sendEraseReq(0x60010000, 0x800))
  {
    result = CLI_OK;
  }
  else
  {
    result = CLI_PENDING;
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static uint32 cmdl_readSrcDummy(uint8* msgBuf, uint32 bufSize, uint32 dstAddr)
{
  printf("CMDL: readSrcDummy(addr=%08x, size=%d)\n", dstAddr, bufSize);
  
  for(int i = 0; i < bufSize; i++)
  {
    msgBuf[i] = i;
  }
  return bufSize;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_programCmd(const char* argStr)
{
  int result;

  fblctl_registerCallback(&cmdl_finishCmd);
  if(STATUS_eOK != fblctl_sendProgramReq(0x60010000, &cmdl_readSrcDummy))
  {
    result = CLI_OK;
  }
  else
  {
    result = CLI_PENDING;
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_activateCmd(const char* argStr)
{
  int result;

  fblctl_registerCallback(&cmdl_finishCmd);
  if(STATUS_eOK != fblctl_sendActivateReq(0x60010000, 0x800))
  {
    result = CLI_OK;
  }
  else
  {
    result = CLI_PENDING;
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_restartCmd(const char* argStr)
{
  int result;

  fblctl_registerCallback(&cmdl_finishCmd);
  if(STATUS_eOK != fblctl_sendRestartReq())
  {
    result = CLI_OK;
  }
  else
  {
    result = CLI_PENDING;
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_updateCmd(const char* argStr)
{
  int result;

  updseq_registerCallback(&cmdl_finishCmd);
  if(STATUS_eOK != updseq_start())
  {
    result = CLI_OK;
  }
  else
  {
    result = CLI_PENDING;
  }
  return result;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static int cmdl_injectCmd(const char* argStr)
{
  fblctl_injectError(argStr[1] - '0');
  return CLI_OK;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

static T_CLI_ENTRY cmdl_cmdTable[] =
{
  {"?", cmdl_helpCmd},
  {"help", cmdl_helpCmd},
  {"echo", cmdl_echoCmd},
  {"open", cmdl_openCmd},
  {"info", cmdl_infoCmd},
  {"inv",  cmdl_invalidateCmd},
  {"erase", cmdl_eraseCmd},
  {"program", cmdl_programCmd},
  {"activate", cmdl_activateCmd},
  {"restart", cmdl_restartCmd},
  {"reboot", cmdl_restartCmd},
  {"update", cmdl_updateCmd},
  {"inj", cmdl_injectCmd},
  {NULL,   NULL},
};


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void cmdl_init(uint32 devID)
{
  T_CMDL_DATA* cmdlData = cmdl_getCmdlData();
  
  memset(cmdlData, 0, sizeof(T_CMDL_DATA));
  cmdlData->devID = devID;
  cmdlData->state = CMDL_STATE_eIDLE;

  cmdio_init();
#if (CMDL_USE_LIBC != 0)
  libc_registerGetcharCallback(&cmdio_getchar);
  libc_registerPutcharCallback(&cmdio_putchar);
#endif /* (CMDL_USE_LIBC != 0) */

  cmdl_pompt();
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

void cmdl_run(void)
{
  T_CMDL_DATA* cmdData = cmdl_getCmdlData();
  int c = -1;

  if(CMDL_STATE_eIDLE == cmdData->state)
  {
#if (CMDL_USE_LIBC != 0)
    /* Get character from stdin */
    c = libc_getchar();
#else
    c = cmdl_readChar();
#endif
  }
  
  if(c == -1)
  {
    /* Nothing received or still busy */
  }
  else
  {
    /* Received new character */

    /* Check for end of line */
    if( (c == '\r') || (c == '\n') )
    {
      const char* argStr;
      int tblPos;

      /* Send newline */
      cmdl_echo('\n');

      /* Terminate string */
      cmdData->cmdBuffer[cmdData->bufPos] = '\0';

      /* Parse received command */
      tblPos = cli_parseCmd(cmdl_cmdTable, cmdData->cmdBuffer, &argStr);

      cmdl_startCmd();
      /* Process command */
      if(CLI_PENDING != cli_execCmd(cmdl_cmdTable, tblPos, argStr))
      {
        cmdl_finishCmd(0);
      }
    }
    else if(c == 0x7F)
    {
      /* Received DEL */
      if(cmdData->bufPos > 0)
      {
        cmdData->bufPos--;
        cmdl_echo(c);
      }
    }
    else if(cmdData->bufPos < sizeof(cmdData->cmdBuffer) - 1)
    {
      /* Received other char */
      cmdData->cmdBuffer[cmdData->bufPos++] = c;
      cmdl_echo(c);
    }
  }
}


