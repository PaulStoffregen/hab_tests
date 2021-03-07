#ifndef ARM_CPU_H
#define ARM_CPU_H

/* CPSR/SPSR Bits
 * [4:0]    MODE Bits 0x10=USR, 0x11=FIQ, 0x12=IRQ, 0x13=SVC, 0x16=MON, 0x17=ABT, 0x1B=UND, 0x1F=SYS
 * [5]      T Bit 0=ARM state, 1=THUMB state
 * [6]      F Bit 0=FIQ enabled, 1=FIQ disabled
 * [7]      I Bit 0=IRQ enabled, 1=IRQ disabled
 * [8]      A Bit 
 * [9]      E Bit
 * [15:10]  IT Hi Bits
 * [19:16]  GE Bits
 * [24]     J Bit 
 * [26:25]  IT Lo Bits
 * [27]     Q Bit sticky overflow (>5TE)
 * [28]     V Bit oVerflow flag
 * [29]     C Bit Carry overflow
 * [30]     Z Bit Zero result
 * [31]     N Bit Negative result
 *
 * CPSR/SPSR Bit-Fields
 * [7:0]    c Field
 * [15:8]   x Field
 * [23:16]  s Field
 * [31:24]  f Field
 */

#define ARM_PSR_MODE_BF  0, 5
#define ARM_PSR_TBIT_BF  5, 1
#define ARM_PSR_FBIT_BF  6, 1
#define ARM_PSR_IBIT_BF  7, 1
#define ARM_PSR_ABIT_BF  8, 1
#define ARM_PSR_EBIT_BF  9, 1
#define ARM_PSR_QBIT_BF 27, 1
#define ARM_PSR_VBIT_BF 28, 1
#define ARM_PSR_CBIT_BF 29, 1
#define ARM_PSR_ZBIT_BF 30, 1
#define ARM_PSR_NBIT_BF 31, 1

/* Definition of processor modes in CPSR/SPSR */
#define SYSMODE_MASK 0x1F  /* Mask off all mode bits */
#define SYSMODE_USR  0x10  /* Normal User Mode                                        */
#define SYSMODE_FIQ  0x11  /* FIQ Mode processing Fast Interrupts                     */
#define SYSMODE_IRQ  0x12  /* IRQ Mode processing Standard Interrupts                 */
#define SYSMODE_SVC  0x13  /* Supervisor Mode processing Software Interrupts          */
#define SYSMODE_MON  0x16  /* Monitor Mode                                            */
#define SYSMODE_ABT  0x17  /* Abort Mode processing memory faults                     */
#define SYSMODE_UND  0x1B  /* Undefined Mode processing undefined instructions        */
#define SYSMODE_SYS  0x1F  /* System Mode running priviledged operating system tasks  */

/* Definition of interrupt flags in CPSR/SPSR */        
#define T_BIT        BF_MASK(ARM_PSR_TBIT_BF) /* Thumb-Mode Bit: when T bit is set, processor is in Thumb mode */
#define F_BIT        BF_MASK(ARM_PSR_FBIT_BF) /* Fast Interrupt Bit: when F bit is set, FIQ is disabled */
#define I_BIT        BF_MASK(ARM_PSR_IBIT_BF) /* Interrupt Bit: when I bit is set, IRQ is disabled */
#define A_BIT        BF_MASK(ARM_PSR_ABIT_BF) /*  */
#define E_BIT        BF_MASK(ARM_PSR_EBIT_BF) /*  */

/* Definition of condition flags in CPSR/SPSR */        
#define N_BIT        BF_MASK(ARM_PSR_NBIT_BF)
#define Z_BIT        BF_MASK(ARM_PSR_ZBIT_BF)
#define C_BIT        BF_MASK(ARM_PSR_CBIT_BF)
#define V_BIT        BF_MASK(ARM_PSR_VBIT_BF)
#define Q_BIT        BF_MASK(ARM_PSR_QBIT_BF)


/* Definition of security state */
#define SEC_ST_SEC  0
#define SEC_ST_NSEC 1

/* Exception vector offsets */
#define VECT_RESET_OFFS  0
#define VECT_UNDEF_OFFS  4
#define VECT_SWI_OFFS    8
#define VECT_PABT_OFFS  12
#define VECT_DABT_OFFS  16
#define VECT_RESERVED   24
#define VECT_IRQ_OFFS   24
#define VECT_FIQ_OFFS   28


#if !defined(__ASSEMBLER__)
#define CPU_SET_MODE(mode)    \
  __asm__ __volatile__ (      \
    " MSR  cpsr_c, %0" "\r\n" \
  : /* no outputs */          \
  : "I" (mode)                \
  )

#define CPU_GET_CPSR(cpsr)  \
  __asm__ __volatile__ (    \
    " MRS  %0, cpsr" "\r\n" \
  : "=r" (cpsr)             \
  : /* no inputs */         \
  )

#define CPU_SET_CPSR(val)   \
  __asm__ __volatile__ (    \
    " MSR  cpsr, %0" "\r\n" \
  : /* no outputs */        \
  : "r" (val)               \
  )

#define CPU_GET_SPSR(spsr)  \
  __asm__ __volatile__ (    \
    " MRS  %0, spsr" "\r\n" \
  : "=r" (spsr)             \
  : /* no inputs */         \
  )

#define CPU_SET_SPSR(val)   \
  __asm__ __volatile__ (    \
    " MSR  spsr, %0" "\r\n" \
  : /* no outputs */        \
  : "r" (val)               \
  )


#define CPU_GET_SPTR(sp)    \
  __asm__ __volatile__ (    \
    " MOV  %0, sp" "\r\n"   \
  : "=r" (sp)               \
  : /* no inputs */         \
  )

#define CPU_SET_SPTR(addr)  \
  __asm__ __volatile__ (    \
    " MOV  sp, %0" "\r\n"   \
  : /* no outputs */        \
  : "r" (addr)              \
  )

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

#define CPU_WFI()        \
  __asm__ __volatile__ (    \
    " WFI "    "\r\n"       \
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

#define CPU_SET_MODE(rn)    \
  MSR cpsr_c, rn

#define CPU_GET_CPSR(rd)    \
  MRS rd, cpsr
  
#define CPU_SET_CPSR(rn)    \
  MSR cpsr_fsxc, rn

#define CPU_GET_SPSR(rd)    \
  MRS rd, spsr
  
#define CPU_SET_SPSR(rn)    \
  MSR spsr_fsxc, rn


#define CPU_GET_SPTR(rd)    \
  MOV rd, sp

#define CPU_SET_SPTR(rn)    \
  MOV sp, rn

#define CPU_DMB(opt)        \
  DMB

#define CPU_DSB(opt)        \
  DSB

#define CPU_ISB(opt)        \
  ISB

#define CPU_WFI()           \
  WFI

#define CPU_WFE()           \
  WFE

#define CPU_SEV()           \
  SEV

#endif /* defined(__ASSEMBLER__) */

#endif /* ARM_CPU_H */

