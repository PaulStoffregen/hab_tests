#ifndef CPU_ASM_S
#define CPU_ASM_S
#endif /* CPU_ASM_S */

#include "bsp.h"
#include "arm_cpu.h"

.section ".armboot", "ax"
.thumb

/*
 ******************************************************************************
 * cpu_memcpy4(uint32* dstStart, uint32* srcStart, uint32* dstEnd)
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

.global cpu_memcpy4
.func cpu_memcpy4
#ifdef __thumb__
.thumb_func
#endif
cpu_memcpy4:
      /* Check for equal address (dst = src) */
      cmp   r0, r1
      beq   memcpy4_end
memcpy4_loop:
      /* Compare start with end */
      cmp   r0, r2
      /* Out if same or higher */
      bhs   memcpy4_end
      /* Otherwise load data from source address (R1) into R3 and
       * increment address
       */
      ldmia r1!, {r3}
      /* Store R3 to sestination address (R0) and increment address */
      stmia r0!, {r3}
      /* Branch back */
      b     memcpy4_loop
memcpy4_end:
      /* Return to caller */
      bx    lr      
.endfunc


/*
 ******************************************************************************
 * cpu_memcmp4(uint32* memStart, uint32* refStart, uint32* memEnd)
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

.global cpu_memcmp4
.func cpu_memcmp4
#ifdef __thumb__
.thumb_func
#endif
cpu_memcmp4:
      mov   r4, r0
      mov   r0, #0
      /* Check for equal address (mem = ref) */
      cmp   r4, r1
      beq   memcmp4_end
memcmp4_loop:
      /* Compare start with end */
      cmp   r4, r2
      /* Out if same or higher */
      bhs   memcmp4_end
      /* Otherwise load data from mem address (R4) into R0 and
       * increment address
       */
      ldmia r4!, {r0}
      /* Load data from ref address (R1) into R3 and
       * increment address
       */
      ldmia r1!, {r3}
      cmp   r0, r3
      /* Branch back */
      beq   memcpy4_loop
      sub   r0, r3
memcmp4_end:
      /* Return to caller */
      bx    lr      
.endfunc


/*
 ******************************************************************************
 * cpu_memset4(uint32* dstStart, uint32 pattern, uint32* dstEnd)
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

.global cpu_memset4
.func cpu_memset4
#ifdef __thumb__
.thumb_func
#endif
cpu_memset4:
memset4_loop:
      /* Compare start with end */
      cmp   r0, r2
      /* Out if higer or same */
      bhs   memset4_end
      /* Otherwise store R1 to address in R0 and increment R0 */
      stmia r0!, {r1}
      /* Branch back */
      b     memset4_loop
memset4_end:
      /* Return to caller */
      bx    lr
.endfunc


/*
 ******************************************************************************
 * cpu_memchk4(uint32* memStart, uint32 pattern, uint32* memEnd)
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

.global cpu_memchk4
.func cpu_memchk4
#ifdef __thumb__
.thumb_func
#endif
cpu_memchk4:
memchk4_loop:
      /* Compare start with end */
      cmp   r0, r2
      /* Out if higher or same */
      bhs   memchk4_end
      /* Otherwise load data from memory address (R0) into R3 and increment address */
      ldmia r0!, {r3}
      /* Compare with pattern given in R1 */
      cmp   r1, r3
      /* Branch back if equal */
      beq   memchk4_loop
      /* Decrement R0 in order to return address where mismatch was detected */
      sub   r0, #4
memchk4_end:
      /* Return to caller */
      bx    lr
.endfunc

