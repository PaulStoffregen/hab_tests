#ifndef HAB_API_H
#define HAB_API_H


#define HAB_API_OK    0
#define HAB_API_WARN  1
#define HAB_API_FAIL -1

#define be32ToCpu(x)        \
  ( 0                       \
  | (((char*)&x)[0] << 24)  \
  | (((char*)&x)[1] << 16)  \
  | (((char*)&x)[2] <<  8)  \
  | (((char*)&x)[3] <<  0))


extern int hab_init(void);
extern int hab_deinit(void);
extern int hab_getStatus(void);
extern int hab_checkTargetMem(void* startAddr, uint32 length);
extern int hab_authImage(uint32 imageAddr, uint32 imageSize, uint32 ivtOffs);

extern boolean hab_isIvtValid(const void* ivtAddr);
extern const void* hab_getOwnIvtAddr(void);
extern const void* hab_getOwnDcdAddr(void);
extern uint32 hab_getOwnDcdSize(void);
extern const void* hab_getOwnCsfAddr(void);


extern const void* hab_getDcdAddrFromIvt(const void* ivtAddr);
extern const void* hab_getBootAddrFromIvt(const void* ivtAddr);
extern const void* hab_getEntryAddrFromIvt(const void* ivtAddr);
extern const void* hab_getPayloadAddrFromIvt(const void* ivtAddr);
extern uint32 hab_getPayloadSizeFromIvt(const void* ivtAddr);
extern uint32 hab_getImageSizeFromIvt(const void* ivtAddr);

extern void* hab_getDcdSizeFromIvt(void* dcdAddr);

#endif /* HAB_API_H */

