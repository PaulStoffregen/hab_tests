#ifndef LIBC_C
#define LIBC_C
#endif /* LIBC_C */

#include "bsp.h"
#include "libc.h"

#define PRINT_BUF_SIZE         256

#define LINE_FEED              0xA
#define CARRIAGE_RETURN        0xD
#define PRINTABLE_START        0x20
#define PRINTABLE_END          0x7E

int libc_vsprintf(char *buf, const char *fmt, va_list args);

/**
 * @brief 
 *
 * @param fmt
 * @param ...
 *
 * @return 
 */
static char libc_printBuffer[PRINT_BUF_SIZE];

#define PAD_LEFT      0x0
#define PAD_RIGHT     0x1
#define PAD_ZERO      0x2
#define PAD_POS_PLUS  0x4
#define PAD_POS_SPACE 0x8

/* Definition and initialization of putchar() callback function */
static T_PUTCHAR_FUNC libc_putcharCbk = NULL;

/* Definition and initialization of getchar() callback function */
static T_GETCHAR_FUNC libc_getcharCbk = NULL;


void libc_registerPutcharCallback(T_PUTCHAR_FUNC cbk)
{
  libc_putcharCbk = cbk;
}


void libc_registerGetcharCallback(T_GETCHAR_FUNC cbk)
{
  libc_getcharCbk = cbk;
}

/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Print a character into a buffer
 *
 * @param buf - buffer to print to
 * @param size - size of the buffer
 * @param c - char to be printed
 ******************************************************************************
 */

static int _libc_snprintc(char** buf, int* size, char c)
{
  if(*size > sizeof(char))
  {
    **buf = c;
    (*buf)++;
    (*size)--;
  }
  return sizeof(char);
}


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Print a character into a buffer
 *
 * @param buf - buffer to print to
 * @param size - size of the buffer
 * @param str - string to be printed
 * @param width - width for the string to be used
 * @param pad - type of padding to be used
 ******************************************************************************
 */

static int _libc_snprints(char **buf, int* size, const char* str, int width, int pad)
{
  register int pc = 0;
  register int padchar = ' ';

  if(width > 0)
  {
    register int len = 0;
    len = libc_strlen(str);
    
    if(len >= width)
    {
      /* String is longer then width, so do not pad */
      width = 0;
    }
    else
    {
      /* String is shorter then width, so pad difference */
      width -= len;
    }
    
    if(pad & PAD_ZERO)
    {
      /* Padding shall be done with zeros */
      padchar = '0';
    }
  }

  if( !(pad & PAD_RIGHT) )
  {
    /* Left padding */
    for(pc = 0; width > 0; width--)
    {
      pc += _libc_snprintc(buf, size, padchar);
    }
  }

  for(; *str != '\0'; str++)
  {
    pc += _libc_snprintc(buf, size, *str);
  }

  /* Right padding */
  for(; width > 0; width--)
  {
    pc += _libc_snprintc(buf, size, ' ');
  }
  return pc;
}


/* The following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @brief Print a character into a buffer
 *
 * @param buf - buffer to print to
 * @param size - size of the buffer
 * @param num - number to be printed
 * @param base - base of the number to be printed
 * @param sg - 
 * @param width - width for the number to be used
 * @param pad - type of padding to be used
 * @param letbase - 
 ******************************************************************************
 */

static int _libc_snprinti(char **buf, int* size, int num, int base, int sg, int width, int pad, int letbase)
{
  char print_buf[PRINT_BUF_LEN];
  register char* s;
  register int t;
  register int neg = 0;
  register int pc = 0;
  register unsigned int u = num;

  if(sg && (base == 10) && (num < 0) )
  {
    /* Number is signed and less then 0, base is 10 */ 
    neg = 1;
    u = -num;
  }

  /* 0-terminate the buffer */ 
  s = print_buf + PRINT_BUF_LEN - 1;
  *s = '\0';

  do
  {
    t = u % base;
//    t = _libc_udivmodhi4(u, base, 1);
    if(t >= 10)
    {
      /* Base is 16 */
      t += letbase - '0' - 10;
    }
    *--s = t + '0';
    u = u / base;
//    u = _libc_udivmodhi4(u, base, 0);
  }while(u);

  if(neg)
  {
    if( (width > 0) && (pad & PAD_ZERO) )
    {
      pc += _libc_snprintc(buf, size, '-');
      width--;
    }
    else
    {
      *--s = '-';
    }
  }
  else if(pad & PAD_POS_PLUS)
  {
    if( (width > 0) && (pad & PAD_ZERO) )
    {
      pc += _libc_snprintc(buf, size, '+');
      width--;
    }
    else
    {
      *--s = '+';
    }
  }
  else if(pad & PAD_POS_SPACE)
  {
    if( (width > 0) && (pad & PAD_ZERO) )
    {
      pc += _libc_snprintc(buf, size, ' ');
      width--;
    }
    else
    {
      *--s = ' ';
    }
  }
  return pc += _libc_snprints(buf, size, s, width, pad);
}


int libc_vsnprintf(char* buf, int size, const char* fmt, va_list args)
{
  register int width;
  register int pad;
  register int len;
  register int pc;
  char scr[2];

  for(pc = 0; (*fmt != 0); fmt++)
  {
    if(*fmt == '%')
    {
      fmt++;
      len = 0;
      width = 0;
      pad = PAD_LEFT;

      while(*fmt != '\0')
      {
        if(*fmt == '-')
        {
          pad |= PAD_RIGHT;
        }
        else if(*fmt == '+')
        {
          pad |= PAD_POS_PLUS;
        }
        else if(*fmt == ' ')
        {
          pad |= PAD_POS_SPACE;
        }
        else if(*fmt == '0')
        {
          pad |= PAD_ZERO;
        }
        else
        {
          break;
        }
        fmt++;
      }

      while( *fmt != '\0' )
      {
        if( *fmt < '0' )
        {
           /* */
           break;
        }
        else if( *fmt > '9' )
        {
          /* */
          break;
        }
        width *= 10;
        width += *fmt - '0';
        fmt++;
      }
      
      while( *fmt != '\0')
      {
        if(*fmt == 'h')
        {
          if(len == 0)
          {
            len = 2;
          }
          else if(len == 2)
          {
            len = 1;
          }
        }
        else if( (*fmt == 'l') && ( len < 8) )
        {
          len += 4;
        }
        else
        {
          break;
        }
        fmt++;
      }

      if(*fmt == '\0')
      {
        break;
      }


      if(*fmt == 's')
      {
        register char *str = (char*)va_arg(args, int);
        if(str != 0)
        {
          pc += _libc_snprints(&buf, &size, str, width, pad);
        }
        else
        {
          pc += _libc_snprints(&buf, &size, "(null)", width, pad);
        }
        continue;
      }

      if(*fmt == 'd')
      {
        int arg;
        if(len == 1)
        {
          arg = (char)va_arg(args, int);
        }
        else if(len == 2)
        {
          arg = (short)va_arg(args, int);
        }
        else if(len & (1 << 3))
        {
          arg = va_arg(args, long long);
        }
        else
        {
          arg = va_arg(args, int);
        }
        pc += _libc_snprinti(&buf, &size, arg, 10, 1, width, pad, 'a');
        continue;
      }
      
      if(*fmt == 'i')
      {
        pc += _libc_snprinti(&buf, &size, va_arg(args, int), 10, 1, width, pad, 'a');
        continue;
      }
      
      if(*fmt == 'u')
      {
        pc += _libc_snprinti(&buf, &size, va_arg(args, int), 10, 0, width, pad, 'a');
        continue;
      }

      if(*fmt == 'x')
      {
        int arg;
        if(len == 1)
        {
          arg = (char)va_arg(args, int);
        }
        else if(len == 2)
        {
          arg = (short)va_arg(args, int);
        }
        else if(len & (1 << 3))
        {
          arg = va_arg(args, long long);
        }
        else
        {
          arg = va_arg(args, int);
        }
        pad &= ~(PAD_POS_PLUS | PAD_POS_SPACE);
        pc += _libc_snprinti(&buf, &size, arg, 16, 0, width, pad, 'a');
        continue;
      }

      if(*fmt == 'X')
      {
        pc += _libc_snprinti(&buf, &size, va_arg(args, int), 16, 0, width, pad, 'A');
        continue;
      }

      if(*fmt == 'c')
      {
        /* char are converted to int then pushed on the stack */
        scr[0] = (char)va_arg(args, int);
        scr[1] = '\0';
        pc += _libc_snprints(&buf, &size, scr, width, pad);
        continue;
      }

      if(*fmt == '%')
      {
        pc += _libc_snprintc(&buf, &size, *fmt);
      }
    }
    else 
    {
      pc += _libc_snprintc(&buf, &size, *fmt);
    }
  }

  if(buf)
  {
    *buf = '\0';
  }
  return pc;
}


int libc_snprintf(char* buf, int size, const char *fmt, ...)
{
  va_list args;
  int i;
  va_start(args, fmt);

  i = libc_vsnprintf(buf, size, fmt, args);
  va_end(args);

  return i;
}


int libc_printf(const char *fmt, ...)
{
  va_list args;
  int i;
  va_start(args, fmt);

  i = libc_vsnprintf(libc_printBuffer, PRINT_BUF_SIZE, fmt, args);
  va_end(args);

  /* Print the string */
  libc_puts(libc_printBuffer);
  return i;
}

/**
 * @brief 
 *  strlen - Find the length of a string
 *
 * @param str
 *
 * @return 
 */
int libc_strlen(const char* str)
{
  const char *sc;

  for(sc = str; *sc != '\0'; sc++)
  {
    /* do nothing */
  }
  return sc - str;
}


int libc_putchar(int c)
{
  int result = -1; /* EOF */

  if(libc_putcharCbk != NULL)
  {
    result = libc_putcharCbk(c);
  }
  return result;
}


int libc_getchar(void)
{
  int result = -1; /* EOF */

  if(libc_getcharCbk != NULL)
  {
    result = libc_getcharCbk();
  }
  return result;
}


int libc_puts(const char* str)
{
  int i = 0;
  while(*str != '\0')
  {
    if((int)*str != libc_putchar((int)*str))
    {
      i = -1;
      break;
    }
    else
    {
      str++;
      i++;
    }
  }
  return i;
}


void* libc_memset(void *dst, int pattern, int len)
{
  char* ptr = dst;
  dst = (void*)(ptr + len);
  
  while(ptr < (char*)dst)
  {
    *ptr = pattern;
    ptr++;
  }
  return dst;
}

void* _libc_memcpyRev(void* dst, const void* src, int len)
{
  char* s = (char*)src + len;
  char* d = (char*)dst + len;
  char* start = (char*)src;
  while(s > start)
  {
    d--;
    s--;
    *d = *s;
  }
  return (void*)d;
}


void* libc_memcpy(void* dst, const void* src, int len)
{
  char* s = (char*)src;
  char* d = (char*)dst;
  char* end = s + len;
  while(s < end)
  {
    *d = *s;
    d++;
    s++;
  }
  return (void*)d;
}


void* libc_memmove(void* dst, const void* src, int len)
{
  const char *s = (const char*)src;
  char* d = (char*)dst;
  if(s < d)
  {
    _libc_memcpyRev(dst, src, len);
  }
  else
  {
    libc_memcpy(dst, src, len);
  }
  return dst;
}


int libc_memcmp(const void* src, const void* ref, int len)
{
  int diff = 0;
  char* s = (char*)src;
  char* r = (char*)ref;
  char* end = s + len;
  while( (s < end) && (diff == 0) )
  {
    diff = *s - *r;
    r++;
    s++;
  }
  return diff;
}

