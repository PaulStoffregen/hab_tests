#ifndef CRC32_C
#define CRC32_C
#endif /* CRC32_C */

#include "bsp.h"
#include "crc32.h"


/*!
 ******************************************************************************
 * @fn crc32_configCtx
 ******************************************************************************
 * @brief Initialize the given CRC context.
 *
 * @param ctx [IO] - The CRC32 context to be initialized
 * @param crcTbl [I] - The CRC lookup table
 *
 ******************************************************************************
 */

void crc32_configCtx(T_CRC32_DATA* ctx, const uint32* crcTbl)
{
  ctx->crc = 0;
  ctx->lut = crcTbl;
}


/*!
 ******************************************************************************
 * @fn crc32_preset
 ******************************************************************************
 * @brief Preset the CRC value with a dedicated initial value
 *
 * @param ctx [IO] - The CRC32 context to be used
 * @param preset [I] - The value to be taken as CRC value
 *
 ******************************************************************************
 */
 
void crc32_preset(T_CRC32_DATA* ctx, uint32 preset)
{
  ctx->crc = preset;
}


/*!
 ******************************************************************************
 * @fn crc32_finalize
 ******************************************************************************
 * @brief Finalize the CRC by XORing a dedicated value
 *
 * @param ctx [IO] - The CRC32 context to be used
 * @param preset [I] - The value to be XORed
 *
 ******************************************************************************
 */
 
void crc32_finalize(T_CRC32_DATA* ctx, uint32 finit)
{
  /* TODO: possibly reflect output */
  /* ... */

  /* XOR the finit value */
  ctx->crc ^= finit;
}


/*!
 ******************************************************************************
 * @fn crc32_read
 ******************************************************************************
 * @brief Read the current CRC value
 *
 * @param ctx [IO] - The CRC32 context to be used
 *
 * @return The CRC value
 *
 ******************************************************************************
 */
 
uint32 crc32_read(T_CRC32_DATA* ctx)
{
  return ctx->crc;
}


/*!
 ******************************************************************************
 * @fn crc32_writeLE
 ******************************************************************************
 * @brief Write the current CRC value to the given destination buffer in
 *   little-endian format
 *
 * @param ctx [I] - The CRC32 context to be used
 * @param dstBuf [O] - The buffer to which to write the CRC value
 *
 ******************************************************************************
 */

void crc32_writeLE(T_CRC32_DATA* ctx, uint8 dstBuf[4])
{
  dstBuf[0] = ((uint8*)&(ctx->crc))[3];
  dstBuf[1] = ((uint8*)&(ctx->crc))[2];
  dstBuf[2] = ((uint8*)&(ctx->crc))[1];
  dstBuf[4] = ((uint8*)&(ctx->crc))[0];
}


/*!
 ******************************************************************************
 * @fn crc32_writeBE
 ******************************************************************************
 * @brief Write the current CRC value to the given destination buffer in
 *   big-endian format
 *
 * @param ctx [I] - The CRC32 context to be used
 * @param dstBuf [O] - The buffer to which to write the CRC value
 *
 ******************************************************************************
 */

void crc32_writeBE(T_CRC32_DATA* ctx, uint8 crc[4])
{
  crc[0] = ((uint8*)&(ctx->crc))[0];
  crc[1] = ((uint8*)&(ctx->crc))[1];
  crc[2] = ((uint8*)&(ctx->crc))[2];
  crc[3] = ((uint8*)&(ctx->crc))[3];
}


/*!
 ******************************************************************************
 * @fn crc32_updateNTabFwd
 ******************************************************************************
 * @Brief: Calculate CRC over a given number of bytes
 *
 *
 * @param ctx [IO] - The CRC32 context to be used
 * @param srcBuf [I] - The buffer where to read the data to calculate the CRC
 *   from
 * @param numBytes [I] - The mumber of bytes within the buffer
 *
 ******************************************************************************
 */

void crc32_updateNTabFwd(T_CRC32_DATA* ctx, const uint8* srcBuf, uint16 numBytes)
{
  uint8  hiByte;   /* CRC's MSB */
  sint8  bitMask;
  uint8  srcByte;
  uint16 srcPos;
  uint32 mod;      /* Modulator value */
  uint32 crc;      /* Cached CRC value */

  /* Cache the current CRC value */
  crc = ctx->crc;
  /* Cache the polynomial value */
  mod = ctx->lut[1];

  for(srcPos = 0; srcPos < numBytes; srcPos++)
  {
    srcByte = srcBuf[srcPos];
    for(bitMask = 0x80; bitMask > 0; bitMask >>= 1)
    {
      hiByte = (crc >> 24); /* Get CRC's MSB */
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
 * @fn crc32_updateSTabFwd
 ******************************************************************************
 * @Brief: Calculate CRC over a given number of bytes
 *
 *
 * @param ctx [IO] - The CRC32 context to be used
 * @param srcBuf [I] - The buffer where to read the data to calculate the CRC
 *   from
 * @param numBytes [I] - The mumber of bytes within the buffer
 *
 ******************************************************************************
 */
 
void crc32_updateSTabFwd(T_CRC32_DATA* ctx, const uint8* srcBuf, uint16 numBytes)
{
  uint8  hiByte;   /* CRC's MSB */
  uint8  hiNibble; /* MSB's high nibble */
  uint8  loNibble; /* MSB's low nibble */
  uint16 srcPos;
  uint32 mod;      /* Modulator value */
  uint32 crc;      /* Cached CRC value */

  /* Cache the current CRC value */
  crc = ctx->crc;

  for(srcPos = 0; srcPos < numBytes; srcPos++)
  {
    hiByte = (crc >> 24); /* Get CRC's MSB */
    hiByte ^= srcBuf[srcPos]; /* XOR with input byte */
    hiNibble = (uint8)(hiByte >> 4); /* Get upper nibble */
    loNibble = (hiByte & 0x0F); /* Get lower nibble */
    mod = ctx->lut[hiNibble]; /* Get modulator for upper nibble */
    hiNibble = (mod >> 28); /* Extract modulator's upper nibble */
    loNibble ^= hiNibble; /* XOR with lower nibble */
    mod <<= 4; /* Shift out modulator's upper nibble */
    mod ^= ctx->lut[loNibble]; /* XOR with lower nibble's modulator */
    crc <<= 8; /* Shift out CRC's MSB */
    crc ^= mod; /* XOR CRC with modulator to form new CRC */
  }
  
  /* Write back the calculated CRC value to the context */
  ctx->crc = crc;
}


/*!
 ******************************************************************************
 * @fn crc32_updateSTabRev
 ******************************************************************************
 * @Brief: Calculate CRC over a given number of bytes with reflected algorithm
 *
 *
 * @param ctx [IO] - The CRC32 context to be used
 * @param srcBuf [I] - The buffer where to read the data to calculate the CRC
 *   from
 * @param numBytes [I] - The mumber of bytes within the buffer
 *
 ******************************************************************************
 */

void crc32_updateSTabRev(T_CRC32_DATA* ctx, const uint8* srcBuf, uint16 numBytes)
{
  uint8  loByte;   /* CRC's MSB */
  uint8  hiNibble; /* MSB's high nibble */
  uint8  loNibble; /* MSB's low nibble */
  uint16 srcPos;
  uint32 mod;      /* Modulator value */
  uint32 crc;      /* Cached CRC value */

  /* Cache the current CRC value */
  crc = ctx->crc;

  for(srcPos = 0; srcPos < numBytes; srcPos++)
  {
    loByte = (uint8)(crc); /* Get CRC's LSB */
    loByte ^= srcBuf[srcPos]; /* XOR with input byte */
    loNibble = (loByte & 0x0F); /* Get lower nibble */
    hiNibble = (uint8)(loByte >> 4); /* Get upper nibble */
    mod = ctx->lut[loNibble]; /* Get modulator for lower nibble */
    loNibble = (mod & 0x0F); /* Extract modulator's lower nibble */
    hiNibble ^= loNibble; /* XOR with upper nibble */
    mod >>= 4; /* Shift out modulator's lower nibble */
    mod ^= ctx->lut[hiNibble]; /* XOR with upper nibble's modulator */
    crc >>= 8; /* Shift out CRC's LSB */
    crc ^= mod; /* XOR CRC with modulator to form new CRC */
  }
  
  /* Write back the calculated CRC value to the context */
  ctx->crc = crc;
}


/*!
 ******************************************************************************
 * @fn crc32_updateFTabFwd
 ******************************************************************************
 * @Brief: Calculate CRC over a given number of bytes
 *
 *
 * @param ctx [IO] - The CRC32 context to be used
 * @param srcBuf [I] - The buffer where to read the data to calculate the CRC
 *   from
 * @param numBytes [I] - The mumber of bytes within the buffer
 *
 ******************************************************************************
 */
 
void crc32_updateFTabFwd(T_CRC32_DATA* ctx, const uint8* srcBuf, uint16 numBytes)
{
  uint8  hiByte;   /* CRC's MSB */
  uint16 srcPos;
  uint32 mod;      /* Modulator value */
  uint32 crc;      /* Cached CRC value */

  /* Cache the current CRC value */
  crc = ctx->crc;

  for(srcPos = 0; srcPos < numBytes; srcPos++)
  {
    hiByte = (crc >> 24); /* Get CRC's MSB */
    hiByte ^= srcBuf[srcPos]; /* XOR with input byte */
    mod = ctx->lut[hiByte]; /* Get modulator for MSB */
    crc <<= 8; /* Shift out CRC's MSB */
    crc ^= mod; /* XOR CRC with modulator to form new CRC */
  }
  
  /* Write back the calculated CRC value to the context */
  ctx->crc = crc;
}

