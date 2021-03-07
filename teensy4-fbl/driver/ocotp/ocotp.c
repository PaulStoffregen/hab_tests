#ifndef OCOTP_C
#define OCOTP_C
#endif /* OCOTP_C */

#include "bsp.h"

#include "trace_pub.h"
#include "ocotp.h"


/*
 ******************************************************************************
 *
 ******************************************************************************
 *
 * Fuse address is the global memory space address of the dedicated fuse word
 * and expected to be between 0x401F4400 and 0x401F48f0.
 ******************************************************************************
 */

T_STATUS ocotp_writeFuse(uint32 fuseOffs, uint32 newValue)
{
  T_STATUS result = STATUS_eNOK;
  uint32 curValue = 0;
  uint32 ctlStatus;

  if(fuseOffs < OCOTP_FUSE_MAP_OFFS)
  {
    /* Invalid offset */
    TRACE_INFO("Illegal address: %08x\n", (uint32)fuseOffs);
  }
  else if(fuseOffs > OCOTP_FUSE_MAP_END)
  {
    /* Invalid address */
    TRACE_INFO("Illegal address: %08x\n", (uint32)fuseOffs);
  }
  else
  {
    /* Offset is valid, read current value. */
    REG32_RD_BASE_OFFS(curValue, OCOTP_BASE, fuseOffs);
    result = STATUS_eOK;
  }

  do
  {
    if(STATUS_eOK != result)
    {
      /* Previous error */
      break;
    }
    else if(curValue == (curValue | newValue))
    {
      /* No write necessary */
      TRACE_INFO("Skipped\n");
      break;
    }
    else
    {
      volatile uint32 delay;
      uint32 otpAddr = (fuseOffs - (uint32)OCOTP_FUSE_MAP_OFFS) >> 4;
      TRACE_INFO("Start, addr=%02x, data=%08x\n", otpAddr, newValue);
      //return;

      /* 1: Program the following fields with timing values to match the
       *    frequency of ipg_clk:
       *      HW_OCOTP_TIMING[STROBE_PROG]
       *      HW_OCOTP_TIMING[RELAX]
       */
       // TODO: for now assume default settings are ok...

       /* 2: Check that HW_OCOTP_CTRL[BUSY] and HW_OCOTP_CTRL[ERROR] are
        *    clear.  Overlapped accesses are not supported by the controller.
        */
       REG32_RD_BASE_OFFS(ctlStatus, OCOTP_BASE, OCOTP_CTRL_OFFS);
       if(0 != (BF_MASK(OCOTP_CTRL_ERROR_BF) & ctlStatus))
       {
         TRACE_INFO("Prior error\n");
         /* Clear error */
         REG32_WR_BASE_OFFS(ctlStatus, OCOTP_BASE, OCOTP_CTRL_SET_OFFS);
         result = STATUS_eNOK;
         break;
       }
       TRACE_INFO("wait1\n");
       while(0 != (BF_MASK(OCOTP_CTRL_BUSY_BF) & ctlStatus))
       {
         /* Wait */
         REG32_RD_BASE_OFFS(ctlStatus, OCOTP_BASE, OCOTP_CTRL_OFFS);
       }

       /* 3: Write the requested address to HW_OCOTP_CTRL[ADDR] and program
        *    the unlock code into HW_OCOTP_CTRL[WR_UNLOCK].
        */
       ctlStatus = ( 0
                   | otpAddr
                   | BF_SET(0x3E77, OCOTP_CTRL_WR_UNLOCK_BF)
                   );
       REG32_WR_BASE_OFFS(ctlStatus, OCOTP_BASE, OCOTP_CTRL_OFFS);

       /* 4: Write the data to the HW_OCOTP_DATA register. */
       REG32_WR_BASE_OFFS(newValue, OCOTP_BASE, OCOTP_DATA_OFFS);

       /* 5. Once complete, the controller will clear BUSY. */
       TRACE_INFO("wait2\n");
       do
       {
         REG32_RD_BASE_OFFS(ctlStatus, OCOTP_BASE, OCOTP_CTRL_OFFS);       
       }while(0 != (BF_MASK(OCOTP_CTRL_BUSY_BF) & ctlStatus));

       if(0 != (BF_MASK(OCOTP_CTRL_ERROR_BF) & ctlStatus))
       {
         TRACE_INFO("Error\n");
         /* Clear error */
         REG32_WR_BASE_OFFS(ctlStatus, OCOTP_BASE, OCOTP_CTRL_SET_OFFS);
         result = STATUS_eNOK;
         break;
       }

       /* Due to internal electrical characteristics of the OTP during writes,
        * all OTP operations following a write must be separated by 2 us
        */
       for(delay = 0; delay < 33; delay++){};

       /* Need to read it back and update shadow register */
       // page 2504 - read process
       REG32_WR_BASE_OFFS(otpAddr, OCOTP_BASE, OCOTP_CTRL_OFFS);
       ctlStatus = BF_SET(1, OCOTP_READ_CTRL_READ_FUSE_BF);
       REG32_WR_BASE_OFFS(ctlStatus, OCOTP_BASE, OCOTP_READ_CTRL_OFFS);

       TRACE_INFO("wait3\n");
       do
       {
         REG32_RD_BASE_OFFS(ctlStatus, OCOTP_BASE, OCOTP_CTRL_OFFS);       
       }while(0 != (BF_MASK(OCOTP_CTRL_BUSY_BF) & ctlStatus));

       REG32_RD_BASE_OFFS(curValue, OCOTP_BASE, OCOTP_READ_FUSE_DATA_OFFS);
       TRACE_INFO("fuse_write, read back as %08x\n", curValue);

       /* Update shadow register */
       REG32_WR_BASE_OFFS(curValue, OCOTP_BASE, fuseOffs);

       TRACE_INFO("fuse_write finish\n");
    }
  }while(0);
  return result;
}

