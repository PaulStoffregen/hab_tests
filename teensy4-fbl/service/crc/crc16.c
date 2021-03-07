#ifndef CRC16_C
#define CRC16_C
#endif /* CRC16_C */

#include "bsp.h"
#include "crc16.h"


/*!
 ******************************************************************************
 * @fn crc16_configCtx
 ******************************************************************************
 * @brief Initialize the given CRC context.
 *
 * @param ctx [IO] - The CRC16 context to be initialized
 * @param crcTbl [I] - The CRC lookup table
 *
 ******************************************************************************
 */

void crc16_configCtx(T_CRC16_DATA* ctx, const uint16* crcTbl)
{
  ctx->crc = 0;
  ctx->lut = crcTbl;
}


/*!
 ******************************************************************************
 * @fn crc16_preset
 ******************************************************************************
 * @brief Preset the CRC value with a dedicated initial value
 *
 * @param ctx [IO] - The CRC16 context to be used
 * @param preset [I] - The value to be taken as CRC value
 *
 ******************************************************************************
 */
 
void crc16_preset(T_CRC16_DATA* ctx, uint16 preset)
{
  ctx->crc = preset;
}


/*!
 ******************************************************************************
 * @fn crc16_finalize
 ******************************************************************************
 * @brief Finalize the CRC by XORing a dedicated value
 *
 * @param ctx [IO] - The CRC16 context to be used
 * @param preset [I] - The value to be XORed
 *
 ******************************************************************************
 */
 
void crc16_finalize(T_CRC16_DATA* ctx, uint16 finit)
{
  /* TODO: possibly reflect output */
  /* ... */

  /* XOR the finit value */
  ctx->crc ^= finit;
}


/*!
 ******************************************************************************
 * @fn crc16_read
 ******************************************************************************
 * @brief Read the current CRC value
 *
 * @param ctx [IO] - The CRC16 context to be used
 *
 * @return The CRC value
 *
 ******************************************************************************
 */
 
uint16 crc16_read(T_CRC16_DATA* ctx)
{
  return ctx->crc;
}


/*!
 ******************************************************************************
 * @fn crc16_writeLE
 ******************************************************************************
 * @brief Write the current CRC value to the given destination buffer in
 *   little-endian format
 *
 * @param ctx [I] - The CRC16 context to be used
 * @param dstBuf [O] - The buffer to which to write the CRC value
 *
 ******************************************************************************
 */

void crc16_writeLE(T_CRC16_DATA* ctx, uint8 dstBuf[2])
{
  dstBuf[0] = ((uint8*)&(ctx->crc))[1];
  dstBuf[1] = ((uint8*)&(ctx->crc))[0];
}


/*!
 ******************************************************************************
 * @fn crc16_writeBE
 ******************************************************************************
 * @brief Write the current CRC value to the given destination buffer in
 *   big-endian format
 *
 * @param ctx [I] - The CRC16 context to be used
 * @param dstBuf [O] - The buffer to which to write the CRC value
 *
 ******************************************************************************
 */

void crc16_writeBE(T_CRC16_DATA* ctx, uint8 crc[2])
{
  crc[0] = ((uint8*)&(ctx->crc))[0];
  crc[1] = ((uint8*)&(ctx->crc))[1];
}


/*!
 ******************************************************************************
 * @fn crc16_updateNTabFwd
 ******************************************************************************
 * @Brief: Calculate CRC over a given number of bytes
 *
 *
 * @param ctx [IO] - The CRC16 context to be used
 * @param srcBuf [I] - The buffer where to read the data to calculate the CRC
 *   from
 * @param numBytes [I] - The mumber of bytes within the buffer
 *
 ******************************************************************************
 */

void crc16_updateNTabFwd(T_CRC16_DATA* ctx, const uint8* srcBuf, uint16 numBytes)
{
  uint8  hiByte;   /* CRC's MSB */
  sint8  bitMask;
  uint8  srcByte;
  uint16 srcPos;
  uint16 mod;      /* Modulator value */
  uint16 crc;      /* Cached CRC value */

  /* Cache the current CRC value */
  crc = ctx->crc;
  /* Cache the polynomial value */
  mod = ctx->lut[1];

  for(srcPos = 0; srcPos < numBytes; srcPos++)
  {
    srcByte = srcBuf[srcPos];
    for(bitMask = 0x80; bitMask > 0; bitMask >>= 1)
    {
      hiByte = (crc >> 8); /* Get CRC's MSB */
      hiByte ^= srcByte; /* XOR with input */
      srcByte <<= 1; /* Shift out input byte's most significant bit */
      crc <<= 1; /* Shift out CRC's most significant bit */
      if(0 != (hiByte &= 0x80))
      {
        crc ^= mod; /* XOR CRC with modulator to form new CRC */
      }
      else
      {
        /* crc ^= 0; Keep CRC */
      }
    }
  }
  
  /* Write back the calculated CRC value to the context */
  ctx->crc = crc;
}


/*!
 ******************************************************************************
 * @fn crc16_updateSTabFwd
 ******************************************************************************
 * @Brief: Calculate CRC over a given number of bytes
 *
 *
 * @param ctx [IO] - The CRC16 context to be used
 * @param srcBuf [I] - The buffer where to read the data to calculate the CRC
 *   from
 * @param numBytes [I] - The mumber of bytes within the buffer
 *
 ******************************************************************************
 */
 
void crc16_updateSTabFwd(T_CRC16_DATA* ctx, const uint8* srcBuf, uint16 numBytes)
{
  uint8  hiByte;   /* CRC's MSB */
  uint8  hiNibble; /* MSB's high nibble */
  uint8  loNibble; /* MSB's low nibble */
  uint16 srcPos;
  uint16 mod;      /* Modulator value */
  uint16 crc;      /* Cached CRC value */

  /* Cache the current CRC value */
  crc = ctx->crc;

  for(srcPos = 0; srcPos < numBytes; srcPos++)
  {
    hiByte = (crc >> 8); /* Get CRC's MSB */
    hiByte ^= srcBuf[srcPos]; /* XOR with input byte */
    hiNibble = (uint8)(hiByte >> 4); /* Get upper nibble */
    loNibble = (hiByte & 0x0F); /* Get lower nibble */
    mod = ctx->lut[hiNibble]; /* Get modulator for upper nibble */
    hiNibble = (mod >> 12); /* Extract modulator's upper nibble */
    loNibble ^= hiNibble; /* XOR with lower nibble */
    mod <<= 4; /* Shift out modulator's upper nibble */
    mod ^= ctx->lut[hiNibble]; /* XOR with lower nibble's modulator */
    crc <<= 8; /* Shift out CRC's MSB */
    crc ^= mod; /* XOR CRC with modulator to form new CRC */
  }
  
  /* Write back the calculated CRC value to the context */
  ctx->crc = crc;
}


/*!
 ******************************************************************************
 * @fn crc16_updateFTabFwd
 ******************************************************************************
 * @Brief: Calculate CRC over a given number of bytes
 *
 *
 * @param ctx [IO] - The CRC16 context to be used
 * @param srcBuf [I] - The buffer where to read the data to calculate the CRC
 *   from
 * @param numBytes [I] - The mumber of bytes within the buffer
 *
 ******************************************************************************
 */
 
void crc16_updateFTabFwd(T_CRC16_DATA* ctx, const uint8* srcBuf, uint16 numBytes)
{
  uint8  hiByte;   /* CRC's MSB */
  uint16 srcPos;
  uint16 mod;      /* Modulator value */
  uint16 crc;      /* Cached CRC value */

  /* Cache the current CRC value */
  crc = ctx->crc;

  for(srcPos = 0; srcPos < numBytes; srcPos++)
  {
    hiByte = (crc >> 8); /* Get CRC's MSB */
    hiByte ^= srcBuf[srcPos]; /* XOR with input byte */
    mod = ctx->lut[hiByte]; /* Get modulator for MSB */
    crc <<= 8; /* Shift out CRC's MSB */
    crc ^= mod; /* XOR CRC with modulator to form new CRC */
  }
  
  /* Write back the calculated CRC value to the context */
  ctx->crc = crc;
}

