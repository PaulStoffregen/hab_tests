#ifndef CPU_IRQ_H
#define CPU_IRQ_H

extern void cpu_enableIRQs(void);
extern void cpu_disableIRQs(void);

#if !defined(__ASSEMBLER__)

#define CPU_ENA_IRQS()   \
  __asm__ __volatile__ ( \
    " CPSIE i" "\r\n"    \
  : /* no outputs */     \
  : /* no inputs */      \
  : "memory"             \
  )

#define CPU_DIS_IRQS()   \
  __asm__ __volatile__ ( \
    " CPSID i" "\r\n"    \
  : /* no outputs */     \
  : /* no inputs */      \
  : "memory"             \
  )

#else /* defined(__ASSEMBLER__) */
   
#define CPU_ENA_IRQS()   \
     CPSIE i

#define CPU_DIS_IRQS()   \
     CPSID i

#endif /* defined(__ASSEMBLER__) */

#endif /* CPU_IRQ_H */

