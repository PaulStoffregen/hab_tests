#ifndef SWINFO_H
#define SWINFO_H

#define SWINFO_DEFAULT_CRC  0x00000000
#define SWINFO_TAG          0x12345688

typedef struct
{
   uint32 imgAddr;
   uint32 imgSize;
   uint32 crc;
   uint32 tag;
   uint32 versionAddr;
   uint16 versionLen;
   uint8  reserved[10];
}T_SWINFO;

extern const T_SWINFO* swinfo_getOwnSwInfo(void);

#endif /* SWINFO_H */

