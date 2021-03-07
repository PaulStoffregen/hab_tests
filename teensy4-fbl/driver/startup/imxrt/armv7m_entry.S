/*!
 ******************************************************************************
 *
 ******************************************************************************
 *
 *
 ******************************************************************************
 */
#ifndef ARMV7M_ENTRY_S
#define ARMV7M_ENTRY_S
#endif /* ARMV7M_ENTRY_S */

#include "bsp.h"
#include "config.h"
#include "reg.h"
#include "arm_cpu.h"
#include "arm_cm7.h"


.global armv7m_main_stack_end
.global armv7m_default_handler
.global cpu_memset4


/* GNU assembler controls  */
.section .armv7m_vect, "ax"
.thumb

.align 10 /* Align to 2^10 */

/*
 ****************************************************************************** 
 * armv7m_start
 ******************************************************************************
 * RESET vector 0x00000000 branches to here.
 * creates a stack for each ARM mode.
 * sets up a stack pointer for each ARM mode.
 * turns off interrupts in each mode
 * leaves CPU in SYS (System) mode.
 *
 * block copies the initializers to .data section
 * clears the .bss section to zero
 *
 * branches to main()
 ******************************************************************************
 */

.global _start
_start:
.global armv7m_start
armv7m_start:
.global armv7m_exc_vectors
armv7m_exc_vectors:
/* M7 vector table */
/*  0 */ .word armv7m_main_stack_end       /* Stack pointer at reset */
/*  1 */ .word armv7m_reset_handler        /* Reset vector - branch to m7_reset */
/*  2 */ .word armv7m_nmi_handler          /* Non-maskable Interrupt */
/*  3 */ .word armv7m_hard_fault_handler   /* Hard fault exception */
/*  4 */ .word armv7m_mpu_fault_handler    /* MPU fault exception */
/*  5 */ .word armv7m_bus_fault_handler    /* Bus fault exception */
/*  6 */ .word armv7m_use_fault_handler    /* Usage fault exception */
/*  7 */ .word 0                           /* Reserved */
/*  8 */ .word 0                           /* Reserved */
/*  9 */ .word 0                           /* Reserved */
/* 10 */ .word 0                           /* Reserved */
/* 11 */ .word armv7m_svc_handler          /* SVC */
/* 12 */ .word armv7m_debug_mon_handler    /* Debug Monitor */
/* 13 */ .word 0                           /* Reserved */
/* 14 */ .word armv7m_pend_sv_handler      /* PendSV */
/* 15 */ .word armv7m_sys_tick_handler     /* Sys Tick */

.global armv7m_irq_vectors
armv7m_irq_vectors:
/* 16 .. N: External interrupts */
#if 0
.rept IRQ_ID_MAX_IRQS
         .word armv7m_default_handler
.endr
#endif
armv7m_end_vectors:


.section ".armboot", "ax"
.thumb

/*
 ****************************************************************************** 
 * armv7m_reset_handler
 ******************************************************************************
 *
 *
 ******************************************************************************
 */
.global _reset
_reset:
.func armv7m_reset_handler
.thumb_func
armv7m_reset_handler:
      /* Write address of armv7m_exc_vectors to Vector Table Offset Register */
      ldr   r0, =armv7m_exc_vectors
      ldr   r1, =CM7_VTOR
      str   r0, [r1]
      /* Set current stack ponter */
      ldr   r0, =armv7m_main_stack_end
      mov   sp, r0
.endfunc


/*
 ****************************************************************************** 
 * armv7m_startCore
 ******************************************************************************
 *
 *
 ******************************************************************************
 */

.func armv7m_startCore
.thumb_func
armv7m_startCore:
      /* Copy the text section */
      ldr   r0, _text_start
      ldr   r2, _text_end
      ldr   r1, _text_load
      bl    cpu_memcpy4

      /* Copy the data section */
      ldr   r0, _data_start
      ldr   r2, _data_end
      ldr   r1, _data_load
      bl    cpu_memcpy4

      /* Zero the bss section */
      ldr   r0, _bss_start
      ldr   r2, _bss_end
      ldr   r1, =0
      bl    cpu_memset4

#if (ENABLE_STACK_CHECK == 1)
      /* Zero the stack RAM */
      ldr   r0, _stack_start
      ldr   r2, _stack_end
      ldr   r1, =0xAFFEDEAD
      bl    cpu_memset4
#endif /* (ENABLE_STACK_CHECK == 1) */

      /* Enter the C code init function */
      bl    armv7m_init
      /* Enter the C code main function */
      bl    armv7m_main

      /* If main returns we fall through to exit */
      bkpt
      b     .
.endfunc


/* The following symbols are defined in the linker script. */
.global __bss_start
.global __bss_end

.global __data_load
.global __data_start
.global __data_end

.global __text_load
.global __text_start
.global __text_end

/* The alignment is necessary for the assembler */
.align 2 /* Align to 2^2 */
_bss_start:
      .word __bss_start
_bss_end:
      .word __bss_end

_data_start:
      .word __data_start
_data_end:
      .word __data_end
_data_load:
      .word __data_load

_text_start:
      .word __text_start
_text_end:
      .word __text_end
_text_load:
      .word __text_load

#if (ENABLE_STACK_CHECK == 1)
.global __stack_start
.global __stack_end

.align 2 /* Align to 2^2 */
_stack_start:
      .word __stack_start

_stack_end:
      .word __stack_end
#endif /* (ENABLE_STACK_CHECK == 1) */


.section ".text", "ax"
.thumb


/* Create weak definitions for all exception handlers */
.weak armv7m_sys_tick_handler
.weak armv7m_nmi_handler
.weak armv7m_hard_fault_handler
.weak armv7m_mpu_fault_handler
.weak armv7m_bus_fault_handler
.weak armv7m_use_fault_handler
.weak armv7m_svc_handler
.weak armv7m_debug_mon_handler
.weak armv7m_pend_sv_handler

/* Redirect all weak exception handlers to the default handler */
.thumb_func
armv7m_sys_tick_handler:
.thumb_func
armv7m_nmi_handler:
.thumb_func
armv7m_hard_fault_handler:
.thumb_func
armv7m_mpu_fault_handler:
.thumb_func
armv7m_bus_fault_handler:
.thumb_func
armv7m_use_fault_handler:
.thumb_func
armv7m_svc_handler:
.thumb_func
armv7m_debug_mon_handler:
.thumb_func
armv7m_pend_sv_handler:
      b armv7m_default_handler


//.section ".text", "ax"
.thumb

/*
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Weak definition of the core's default exception handler.
 *
 ******************************************************************************
 */

.func   armv7m_default_handler
.weak   armv7m_default_handler
armv7m_default_handler:
      bkpt  #0
      b     .
.endfunc


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Weak definition of the core's init function.
 *
 ******************************************************************************
 */

.weak armv7m_init
.func armv7m_init
.thumb_func
armv7m_init:
      bx    lr
.endfunc


/*
 ******************************************************************************
 *
 ******************************************************************************
 * @par Description:
 *   Weak definition of the core's main function.
 *
 ******************************************************************************
 */

.weak armv7m_main
.func armv7m_main
.thumb_func
armv7m_main:
      bkpt  #0
      b     .
.endfunc


#if !defined(MAIN_STACK_SIZE)
#warning MAIN_STACK_SIZE undefined, using default of 1KiB
#define MAIN_STACK_SIZE 1024
#endif /* !defined(MAIN_STACK_SIZE) */

#define STACK_ALIGN 2

.section ".stack", "aw", %nobits
.align STACK_ALIGN
.armv7m_main_stack:
.space (MAIN_STACK_SIZE), 0
armv7m_main_stack_end:

