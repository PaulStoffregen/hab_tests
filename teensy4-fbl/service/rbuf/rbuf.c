#ifndef RBUF_C
#define RBUF_C
#endif /* RBUF_C */

#include "bsp.h"
#include "rbuf.h"


/*
 ******************************************************************************
 * rbuf_init
 ******************************************************************************
 * Description:
 *   The function initializes the ring buffer structure given by the argument
 *   buf_info.
 *
 ******************************************************************************
 */

void rbuf_init(T_RBUF* bufInfo, uint8* buffer, uint16 size, void (*rdCbk)(uint8*), void (*wrCbk)(uint8))
{
  bufInfo->buffer = buffer;
  bufInfo->size = size;
  bufInfo->rdPtr = 0;
  bufInfo->wrPtr = 0;
  bufInfo->rdCbk = rdCbk;
  bufInfo->wrCbk = wrCbk;
}


/*
 ******************************************************************************
 * rbuf_write
 ******************************************************************************
 * Description:
 *   The function checks whether there is space left in the ring buffer given
 *   by the parameter buf_info and if yes, it writes one byte given by the
 *   parameter data to the ring buffer. If not it returns with an error code.
 *
 ******************************************************************************
 */

T_STATUS rbuf_wrByte(T_RBUF* bufInfo, uint8 byte)
{
  uint8* buffer;
  uint16 wr = bufInfo->wrPtr;
  uint16 w1 = wr + 1;

  if(w1 >= bufInfo->size)
  {
    w1 = 0;
  }
  if(w1 != bufInfo->rdPtr)
  {
    buffer = bufInfo->buffer;
    buffer[wr] = byte;
    bufInfo->wrPtr = w1;

    return RBUF_OK;
  }
  else
  {
    /* buffer is already full */
    return RBUF_ERROR_FULL;
  }
}


/*
 ******************************************************************************
 * rbuf_read
 ******************************************************************************
 * Description:
 *   The function checks whether there is any data to be retrieved from the
 *   ring buffer given by the parameter buf_info and if yes, it reads and
 *   returns it in the parameter data. If not, it returns an error code.
 *
 ******************************************************************************
 */

T_STATUS rbuf_rdByte(T_RBUF* bufInfo, uint8* byte)
{
  uint8* buffer;
  uint16 rd = bufInfo->rdPtr;

  if(bufInfo->wrPtr != rd)
  {
    buffer = bufInfo->buffer;
    *byte = buffer[rd];
    rd++;
    if(rd >= bufInfo->size)
    {
      rd = 0;
    }
    bufInfo->rdPtr = rd;
    return RBUF_OK;
  }
  else
  {
    /* buffer is already empty */
    return RBUF_ERROR_EMPTY;
  }
}


/*
 ******************************************************************************
 * rbuf_flush
 ******************************************************************************
 * Description:
 *   This function resets the ring buffer given by the parameter buf_info, so
 *   as if it had been just initialized and empty.
 *
 ******************************************************************************
 */

void rbuf_flush(T_RBUF* bufInfo)
{
  bufInfo->rdPtr = 0;
  bufInfo->wrPtr = 0;
}


/*
 ******************************************************************************
 * rbuf_getFree
 ******************************************************************************
 * Description:
 *   This function checks how many space is left in the ring buffer given by
 *   the parameter buf_info and returns that value.
 *
 ******************************************************************************
 */

uint16 rbuf_getFree(T_RBUF* bufInfo)
{
  sint32 free = bufInfo->rdPtr - bufInfo->wrPtr - 1;
  if(free < 0)
  {
    free += bufInfo->size;
  }
  return free;
}


/*
 ******************************************************************************
 * rbuf_getFill
 ******************************************************************************
 * Description:
 *   This function checks how many space is ocupied by data in the ring buffer
 *   given by the parameter buf_info.
 *
 ******************************************************************************
 */

uint16 rbuf_getFill(T_RBUF* bufInfo)
{
  sint32 fill = bufInfo->wrPtr - bufInfo->rdPtr;
  if(fill < 0)
  {
    fill += bufInfo->size;
  }
  return fill;
}

