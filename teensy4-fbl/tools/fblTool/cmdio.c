#ifndef CMDIO_C
#define CMDIO_C
#endif /* CMDIO_C */

#include "bsp.h"

#include <stdio.h>

#if defined(__CYGWIN__)
#include <termios.h>
#endif /* defined(__CYGWIN__) */


void cmdio_init(void)
{ 
  /* Set STDIN to unbuffered mode */
  setvbuf(stdin, NULL, _IONBF, 0);
}

#if defined(__CYGWIN__)
/* Reads from keypress, doesn't echo */
int cmdio_getchar(void)
{
   struct termios oldAttr;
   struct termios newAttr;
   int newChar;

   /* Get current attributes */
   tcgetattr(STDIN_FILENO, &oldAttr);

   /* Copy to new attributes */
   newAttr = oldAttr;
   newAttr.c_lflag &= ~( 0
                       | ICANON /* Canonical mode off */
                       | ECHO   /* Echo off */
                       );

   /* Set new attributes */
   tcsetattr(STDIN_FILENO, TCSANOW, &newAttr);

   /* Read single character */
   newChar = getchar();

   /* Restore old attributes */
   tcsetattr(STDIN_FILENO, TCSANOW, &oldAttr);

   return newChar;
}
#elif defined(__MINGW32__)
int cmdio_getchar(void)
{
  return getchar();
}
#endif /* defined(__CYGWIN__) || defined(__MINGW32__) */


int cmdio_putchar(int byte)
{
  return putchar(byte);
}

