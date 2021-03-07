#ifndef PDU_H
#define PDU_H

typedef struct T_PDU_HDR
{
  U8  type;
  U8  flags;
  U16 size;
}T_PDU_HDR;

typedef struct T_PDU0
{
  T_PDU_HDR hdr;
  char data[0];
}T_PDU0;

typedef struct T_PDU1
{
  T_PDU_HDR hdr;
  char* data;
}T_PDU1;

typedef struct T_PDU2
{
  T_PDU_HDR hdr;
  char* data;
  U16 len;
}T_PDU2;

typedef struct T_PDU3
{
  T_PDU_HDR hdr;
  char* data;
  U16 len;
  U16 offs;
}T_PDU3;

typedef struct T_PDU
{
  U8*   data;
  U16   size;
  U16   len;
}T_PDU;


#endif /* PDU_H */

