#ifndef MAIN_C
#define MAIN_C
#endif /* MAIN_C */

#include "bsp.h"
#include <string.h>
#include <stdio.h>

#include "pdu.h"
#include "rbuf.h"
#include "dlcf.h"
#include "dlcf_dev.h"


/* Define control characters
 * and their escaped characters.
 * Note:
 *   For the purpose of testing we choose characters that
 *   will be included in the simple message.
 */
                 /* SOF  ESC  EOF */
uint8 ctlBytes[] = {'l', 'e', 'o'};
uint8 escBytes[] = {'l', 'e', 'o'};

/* Setup channel configuration */
T_DLCF_CFG dlcfCfg =
{
  .ctlBytes = ctlBytes,
  .escBytes = escBytes,
  .numCtlEscBytes = sizeof(ctlBytes),
};


int main(int argc, char* argv[])
{
  T_DLCF_CTX svrCtx;
  T_DLCF_CTX cliCtx;

  boolean srvSend = FALSE;
  boolean cliSend = FALSE;

  /* Server's ring buffer */
  uint8  srvRbuf[256];
  /* Client's ring buffer */
  uint8  cliRbuf[256];
  /* Server's ring buffer control structure */
  T_RBUF srvQueue;
  /* Client's ring buffer control structure */
  T_RBUF cliQueue;

  /* Client's transmit message */  
  /* Should be transmitted as:
   * "lHedekeken frenm Cekiednto"
   */
  uint8 cliTxData[100] = "Hello from Client";
  /* Client's receive buffer */
  uint8 cliRxData[100];

  /* Client's transmit PDU */
  T_PDU cliTxPdu =
  {
    .data = cliTxData,               /* Transmit message */
    .size = sizeof(cliTxData),       /* Buffer size */
    .len = strlen((char*)cliTxData), /* Message length */
  };

  /* Client's receive PDU */
  T_PDU cliRxPdu =
  {
    .data = cliRxData,               /* Receive buffer */
    .size = sizeof(cliRxData),       /* Buffer size */
    .len = 0,
  };
  
  /* Server's transmit message */
  uint8 svrTxData[100] = "Hello from Server";
  /* Client's receive buffer */
  uint8 svrRxData[100];

  /* Server's transmit PDU */
  T_PDU svrTxPdu =
  {
    .data = svrTxData,               /* Transmit message */
    .size = sizeof(svrTxData),       /* Buffer size */
    .len = strlen((char*)svrTxData), /* Message length */
  };

  /* Server's receive PDU */
  T_PDU svrRxPdu =
  {
    .data = svrRxData,               /* Receive buffer */
    .size = sizeof(svrRxData),       /* Buffer size */
    .len = 0,
  };

  /* Server's device data */
  T_DLCF_DEV_DATA srvDevData =
  {
    .txFifo = &cliQueue,             /* Server sends to client's queue */
    .rxFifo = &srvQueue,             /* Server receives from server's queue */
  };

  /* Client's device data */
  T_DLCF_DEV_DATA cliDevData =
  {
    .txFifo = &srvQueue,             /* Client sends to server's queue */
    .rxFifo = &cliQueue,             /* Client receives from client's queue */
  };

  /* Server's device info */
  T_DLCF_DEV_INFO srvDevInfo =
  {    
    .wrByte = &dlcf_dev_sendByte,    /* Appropriate send function for RBUF */
    .rdByte = &dlcf_dev_recvByte,    /* Appropriate receive function for RBUF */
    .devData = &srvDevData,          /* Server's device data appropriate for RBUF */
  };

  /* Client's device info */
  T_DLCF_DEV_INFO cliDevInfo =
  {    
    .wrByte = &dlcf_dev_sendByte,
    .rdByte = &dlcf_dev_recvByte,
    .devData = &cliDevData,
  };

  /* Initialize server's queue */
  rbuf_init(&srvQueue, srvRbuf, 256, NULL, NULL);
  /* Initialize client's queue */
  rbuf_init(&cliQueue, cliRbuf, 256, NULL, NULL);

  /* Configure client's channel context */
  printf("CLI: Config...");
  /* Configure character set */
  dlcf_configCtx(&cliCtx, &dlcfCfg);
  /* Setup device info */
  if(DLCF_OK != dlcf_setDevInfo(&cliCtx, &cliDevInfo))
  {
    printf("failed\n");
  }
  else    
  {
    printf("ok\n");
  }
  
#if 0
    dlcf_registerRdByteCallback(&cliCtx, &dlcf_recvByteRbuf);
    dlcf_registerWrByteCallback(&cliCtx, &dlcf_sendByteRbuf);
    dlcf_setDevData(&cliCtx, &srvDevData);
#endif
  
  /* Configure server's channel context */
  printf("SVR: Config...");
  dlcf_configCtx(&svrCtx, &dlcfCfg);
  if(DLCF_OK != dlcf_setDevInfo(&svrCtx, &srvDevInfo))
  {
    printf("failed\n");
  }
  else    
  {
    printf("ok\n");
  }

#if 0
  dlcf_registerRdByteOp(&svrCtx, &dlcf_recvByteRbuf);
  dlcf_registerWrByteOp(&svrCtx, &dlcf_sendByteRbuf);
  dlcf_setDevData(&svrCtx, &srvDevData);
#endif

  printf("CLI: Prepare receiver...");
  if(DLCF_OK != dlcf_recvPdu(&cliCtx, &cliRxPdu))
  {
    printf("failed\n");
  }
  else
  {
    printf("ok\n");
  }

  printf("SVR: Prepare receiver...");
  if(DLCF_OK != dlcf_recvPdu(&svrCtx, &svrRxPdu))
  {
    printf("failed\n");
  }
  else
  {
    printf("ok\n");
  }

  
  printf("CLI: Transmit message...");
  if(DLCF_OK != dlcf_sendPdu(&cliCtx, &cliTxPdu))
  {
    printf("failed\n");
  }
  else
  {
    printf("ok\n");
    cliSend = !FALSE;
  }


  int cnt = 0;
  while(1)
  {
    printf("Main: Cycle (cnt=%d)\n", cnt++);

    /* Run DLCF on server context */
    printf("SVR: Run\n");
    dlcf_run(&svrCtx);
    printf("\n");

    /* Run DLCF on client context */
    printf("CLI: Run\n");
    dlcf_run(&cliCtx);
    printf("\n");

    printf("CLI: Check TX status...");
    if(DLCF_FRAME_PENDING != dlcf_getTxStatus(&cliCtx))
    {
      /* Transmission finished */
      printf("finished\n");

      cliSend = FALSE;
    }
    else
    {
      printf("pending\n");
    }

    printf("CLI: Check RX status...");
    if(DLCF_FRAME_PENDING != dlcf_getRxStatus(&cliCtx))
    {
      /* Reception finished */
      printf("finished\n");
      
      printf("\nCLI: Received message\n");
      printf("  %s\n", cliRxData);

      /* Prepare for reception of new message */
      printf("CLI: Setup receiver...");
      if(DLCF_OK != dlcf_recvPdu(&cliCtx, &cliRxPdu))
      {
        printf("failed\n");
      }
      else
      {
        printf("ok\n");
      }

      /* Transmit new message */
      printf("CLI: Transmit message...");
      if(DLCF_OK != dlcf_sendPdu(&cliCtx, &cliTxPdu))
      {
        printf("failed\n");
      }
      else
      {
        printf("ok\n");

        cliSend = !FALSE;
      }      
    }
    else
    {
      printf("pending\n");
    }


    printf("SVR: Check TX status...");
    if(DLCF_FRAME_PENDING != dlcf_getTxStatus(&svrCtx))
    {
      /* Transmission finished */
      printf("finished\n");

      srvSend = FALSE;
    }
    else
    {
      printf("pending\n");
    }

    
    printf("SVR: Check RX status...");
    if(DLCF_FRAME_PENDING != dlcf_getRxStatus(&svrCtx))
    {
      /* Reception finished */
      printf("finished\n");
      printf("\nSVR: Received message\n");
      printf("  %s\n", svrRxData);

      printf("SVR: Setup receiver...");
      if(DLCF_OK != dlcf_recvPdu(&svrCtx, &svrRxPdu))
      {
        printf("failed\n");
      }
      else
      {
        printf("ok\n");
      }

      printf("SVR: Transmit message...");
      if(DLCF_OK != dlcf_sendPdu(&svrCtx, &svrTxPdu))
      {
        printf("failed\n");
      }
      else
      {
        printf("ok\n");

        srvSend = !FALSE;
      }      
    }
    else
    {
      printf("pending\n");
    }
  }
  return 0;
}

