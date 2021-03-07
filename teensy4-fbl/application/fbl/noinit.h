#ifndef NOINIT_H
#define NOINIT_H

typedef struct
{
   uint32 entryReqLo;
   uint32 entryReqHi;
   uint8  reserved[8];
}T_NOINIT_DATA;

T_NOINIT_DATA* noinit_getData(void);

#endif /* NOINIT_H */

