#ifndef RBUF_H
#define RBUF_H


#define RBUF_OK    0
#define RBUF_ERROR_EMPTY 1
#define RBUF_ERROR_FULL  2

typedef struct T_RBUF
{
  uint16 size;   /* size of buffer */
  uint16 rdPtr; /* index of next byte to be read */
  uint16 wrPtr; /* index of next byte to be written */
  uint8* buffer; /* pointer to buffer array */
  void (*rdCbk)(uint8*); /* read callback */
  void (*wrCbk)(uint8);  /* write callback */
}T_RBUF;

void  rbuf_init(T_RBUF* bufInfo, uint8* buffer, uint16 size, void (*)(uint8*), void (*)(uint8));
T_STATUS rbuf_wrByte(T_RBUF* bufInfo, uint8 byte);
T_STATUS rbuf_rdByte(T_RBUF* bufInfo, uint8* byte);
void   rbuf_flush(T_RBUF* bufInfo);
uint16 rbuf_getFree(T_RBUF* bufInfo);
uint16 rbuf_getFill(T_RBUF* bufInfo);

#define rbuf_makeRbuf(name, size)     \
  uint8 name[size + sizeof(T_RBUF)] = \
  {                                   \
    .buffer = &name[sizeof(T_RBUF)],  \
    .size = size,                     \
  }

#endif /* RBUF_H */

