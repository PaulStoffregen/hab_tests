#ifndef LIBC_H
#define LIBC_H

#if defined (_MSC_VER)
  /* On MSVC use standard libs per default */
  #include "stdlib.h"
  #include "stdarg.h"
  #define libc_memcpy memcpy
  #define libc_memset memset
#elif defined (__MINGW32__)
  /* On MINGW use standard libs per default */
  #include "stdlib.h"
  #include "stdarg.h"
  #define libc_memcpy memcpy
  #define libc_memset memset
#elif (__linux__ == 1)
  /* On LINUX use standard libs per default */
  #include "stdlib.h"
  #include "stdarg.h"
  #define libc_memcpy memcpy
  #define libc_memset memset
#elif defined (__TMS320C6X__)
  /* On TMS320C6X use standard args per default */
  #include "stdarg.h"
#else /* All other platforms */
  #include "stdlib.h"
  #include "stdarg.h"

extern int   libc_memcmp(const void* src, const void* ref, int len);
extern void* libc_memcpy(void* dst, const void* src, int len);
extern void* libc_memmove(void* dst, const void* src, int len);
extern void* libc_memset(void* dst, int pattern, int len);

extern void exit(int);

#endif /* platforms */

#define libc_isdigit(c) (c >= '0' && c <= '9')
#define libc_ishex(c) ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'F' && c <= 'F'))
#define libc_isspace(c) (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')

/* Type definitions of the following callbacks */
typedef int (*T_PUTCHAR_FUNC)(int);
typedef int (*T_GETCHAR_FUNC)(void);


extern void  libc_registerGetcharCallback(T_GETCHAR_FUNC cbk);
extern void  libc_registerPutcharCallback(T_PUTCHAR_FUNC cbk);

extern int   libc_putchar(int c);
extern int   libc_getchar(void);
extern int   libc_puts(const char* str);
extern int   libc_sprintf(char* buf, const char* fmt, ...);
extern int   libc_snprintf(char* buf, int size, const char* fmt, ...);
extern int   libc_printf(const char* fmt, ...);
extern int   libc_strlen(const char* str);

extern int   libc_vsprintf(char* buf, const char* fmt, va_list args);
extern int   libc_vsnprintf(char* buf, int size, const char* fmt, va_list args);

#endif /* LIBC_H */

