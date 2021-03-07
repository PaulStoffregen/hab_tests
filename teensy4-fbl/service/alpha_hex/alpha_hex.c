#ifndef ITOHEX_C
#define ITOHEX_C
#endif /* ITOHEX_C */

#include "bsp.h"


/*!
 ******************************************************************************
 *
 ******************************************************************************
 * Converts a binary coded decimal value within the low nibble of the given
 * byte into an alphanumeric character
 *
 ******************************************************************************
 */

char bcdToChar(char bcd)
{
  if(bcd < 10)
  {
    bcd += '0';
  }
  else
  {
    bcd = 0;
  }
  return bcd;
}

char charToBcd(char c)
{
  return c;
}

/*
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Converts the low nibble into an alphanumeric character
 *
 ******************************************************************************
 */
 
uint8 nibble_to_alpha(uint8 buf)
{
  /* Mask off low nibble */
  buf = buf & 0xF;
  if(buf < 10)
  {
    buf += '0';
  }
  else
  {
    buf += 'A' - 10;
  }
  return buf;
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Converts an alphanumeric character and stores it as low nibble
 *
 ******************************************************************************
 */
 
uint8 alpha_to_nibble(uint8 buf)
{
  if(buf < '0')
  {
    /* Not hex */
    buf = 0xF0;
  }
  else if(buf <= '9')
  {
    /* '0' <= c <= '9' */
    buf -= '0';
  }
  else if(buf < 'A')
  {
    /* Not hex */
    buf = 0xF0;
  }
  else if(buf <= 'F')
  {
    /* 'A' <= c <= 'A' */
    buf -= 'A' - 10;
  }
  else if(buf < 'a')
  {
    /* Not hex */
    buf = 0xF0;
  }
  else if(buf <= 'f')
  {
    /* 'a' <= c <= 'f' */
    buf -= 'a' - 10;
  }
  else
  {
    /* Not hex */
    buf = 0xF0;
  }
  return buf;
}

/*
 * Converts the value of a given byte into its alphanumeric representation
 */
void byte_to_alpha(uint8* buf, uint8 byte)
{
  uint16 tmp1;
  uint8  tmp2;
  
  tmp1 = byte * 205;
  buf[1] = tmp1 >> 11;
  tmp2 = buf[1] * 10;
  buf[2] = byte - tmp2;
  
  tmp1 = buf[1] * 205;
  buf[0] = tmp1 >> 11;
  tmp2 = buf[0] * 10;
  buf[1] = buf[1] - tmp2;
  
  buf[0] += '0';
  buf[1] += '0';
  buf[2] += '0';
}


uint8 alpha_to_dec(char c)
{
  if(c < '0')
  {
    /* Not dec */
    c = 0xF0;
  }
  else if(c <= '9')
  {
    /* '0' <= c <= '9' */
    c -= '0';
  }
  else
  {
    /* Not dec */
    c = 0xF0;
  }
  return c;
}


