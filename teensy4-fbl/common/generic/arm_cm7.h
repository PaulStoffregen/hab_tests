#ifndef ARM_CM7_H
#define ARM_CM7_H

#include "armv7m_scb.h"


#define CM7_VTOR        (ARMV7_M_SCB_BASE + SCB_VTOR_OFFS) /* Vector Table Offset Register address */

#define CM7_NUM_EXCS     16

#if !defined(__ASSEMBLER__)

#define CPU_DMB(opt)        \
  __asm__ __volatile__ (    \
    " DMB "    "\r\n"       \
    : /* outputs */         \
    : /* inputs */          \
  )

#define CPU_DSB(opt)        \
  __asm__ __volatile__ (    \
    " DSB "    "\r\n"       \
    : /* outputs */         \
    : /* inputs */          \
  )

#define CPU_ISB(opt)        \
  __asm__ __volatile__ (    \
    " ISB "    "\r\n"       \
    : /* outputs */         \
    : /* inputs */          \
  )

#define CPU_WFE()        \
  __asm__ __volatile__ (    \
    " WFE "    "\r\n"       \
    : /* outputs */         \
    : /* inputs */          \
  )

#define CPU_SEV(opt)        \
  __asm__ __volatile__ (    \
    " SEV "    "\r\n"       \
    : /* outputs */         \
    : /* inputs */          \
  )

#else /* defined(__ASSEMBLER__) */

#define CPU_DMB(opt)        \
  DMB

#define CPU_DSB(opt)        \
  DSB

#define CPU_ISB(opt)        \
  ISB

#define CPU_WFE()           \
  WFE

#define CPU_SEV()           \
  SEV

#endif /* defined(__ASSEMBLER__) */

#include "armv7m_nvic.h"

#endif /* ARM_CM7_H */

