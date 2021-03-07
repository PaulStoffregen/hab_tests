/*
 ******************************************************************************
 * @file     bsp.h
 * @details
 *   This file includes the platform dependant header files.
 * @author   Frank Manig
 * @date     2007-12-11
 * @brief
 *   Include of platform dependant headers.
 *
 ******************************************************************************
 */

#ifndef BSP_H
#define BSP_H

/* Board definitions */
#define BSP_BRD_NONE             1
#define BSP_BRD_MIMXRT1060_EVK   2
#define BSP_BRD_TEENSY40         3

/* System-on-Module (SOM) definitions */
#define BSP_SOM_NONE      1

/* System-on-Chip (SOC) definitions */ 
#define BSP_SOC_NONE      1
#define BSP_SOC_GENERIC   2
#define BSP_SOC_IMXRT     3

/* Operating System (OS) definitions */
#define BSP_OS_NONE       1
#define BSP_OS_LINUX      2
#define BSP_OS_WIN32      3
#define BSP_OS_WIN64      4
#define BSP_OS_MACOS32    5
#define BSP_OS_MACOS64    6

/* CPU definitions */
#define BSP_CPU_NONE         1
#define BSP_CPU_X86          2
#define BSP_CPU_ARMv7        3
#define BSP_CPU_ARMv7A       4
#define BSP_CPU_ARMv7M       5
#define BSP_CPU_ARMv7ME      6
#define BSP_CPU_CORTEX_A8    7
#define BSP_CPU_CORTEX_A9    8
#define BSP_CPU_CORTEX_A15   9
#define BSP_CPU_CORTEX_M3   10
#define BSP_CPU_CORTEX_M4   11
#define BSP_CPU_CORTEX_M7   12


#if !defined (BSP_OS_TYPE)
  /* No OS defined */

  /* Check for Compiler */
  #if defined(__GNUC__)
    /* GNU Compiler */
    #if defined(__i386)
      /* WIN32 */
      #define BSP_OS_TYPE BSP_OS_WIN32
    #elif defined(__x86_64)
      /* WIN64 */
      #define BSP_OS_TYPE BSP_OS_WIN64
    #else
      /* Other OS */
      #define BSP_OS_TYPE BSP_OS_NONE
    #endif
  #elif defined(_WIN32)
    /* Windows Compiler */
    #define BSP_OS_TYPE BSP_OS_WIN32
  #else
    /* Other Compiler */
    #define BSP_OS_TYPE BSP_OS_NONE
  #endif
#endif /* !defined (BSP_OS_TYPE) */


#if !defined (BSP_BOARD_TYPE)
  /* No Board defined */
  #define BSP_BOARD_TYPE BSP_BRD_NONE
#elif (BSP_BOARD_TYPE == BSP_BRD_NONE)
  /* Board is none */
#elif (BSP_BOARD_TYPE == BSP_BRD_MIMXRT1060_EVK)
  /* Override SoM if already defined */
  #if defined(BSP_SOM_TYPE)
  #undef BSP_SOM_TYPE
  #endif /* defined(BSP_SOM_TYPE) */
  #define BSP_SOM_TYPE BSP_SOM_NONE
  
  /* Set SoC if not yet defined */
  #if !defined(BSP_SOC_TYPE)
  #define BSP_SOC_TYPE BSP_SOC_IMXRT
  #endif /* defined(BSP_SOC_TYPE) */
#elif (BSP_BOARD_TYPE == BSP_BRD_TEENSY40)
  /* Override SoM if already defined */
  #if defined(BSP_SOM_TYPE)
  #undef BSP_SOM_TYPE
  #endif /* defined(BSP_SOM_TYPE) */
  #define BSP_SOM_TYPE BSP_SOM_NONE
    
  /* Set SoC if not yet defined */
  #if !defined(BSP_SOC_TYPE)
  #define BSP_SOC_TYPE BSP_SOC_IMXRT
  #endif /* defined(BSP_SOC_TYPE) */
#else
  #error "Board unknown!"
#endif /* !defined (BSP_SOC_TYPE) */


#if !defined (BSP_SOC_TYPE)
  /* SoC is undefined */
  #error "No SoC defined!"
  #define BSP_SOC_TYPE BSP_SOC_NONE
#elif (BSP_SOC_TYPE == BSP_SOC_NONE)
  #error "SoC defined as none!"
#elif (BSP_SOC_TYPE == BSP_SOC_IMXRT)
          
  /* Set default CPU if not defined */
  #if !defined(BSP_CPU_TYPE)
  #define BSP_CPU_TYPE BSP_CPU_CORTEX_M7
  #endif /* defined(BSP_CPU_TYPE) */
          
  #if (BSP_CPU_TYPE == BSP_CPU_CORTEX_M7)
  #include "imxrt.h"
  #include "imxrt_m7_regmap.h"
  #include "imxrt_m7_irqs.h"
  #endif
      
#elif (BSP_SOC_TYPE == BSP_SOC_GENERIC)
  /* Nothing special to include */
#else
  #error "SoC unknown!"
#endif /* !defined (BSP_SOC_TYPE) */


#if !defined (BSP_CPU_TYPE)
  /* No CPU defined, so detect from preprocessor macros */
  #if !defined (__GNUC__)
    #error "No CPU defined!"
    #define BSP_CPU_TYPE BSP_CPU_NONE
  #elif defined(__ARM_ARCH_7A__)
    #define BSP_CPU_TYPE BSP_CPU_ARMv7A
  #elif defined(__ARM_ARCH_7ME__)
    #define BSP_CPU_TYPE BSP_CPU_ARMv7ME
  #elif defined(__ARM_ARCH_7M__)
    #define BSP_CPU_TYPE BSP_CPU_ARMv7M
  #else
    #error "No CPU defined!"
    #define BSP_CPU_TYPE BSP_CPU_NONE
  #endif
#elif (BSP_CPU_TYPE == BSP_CPU_CORTEX_M7)
#elif (BSP_CPU_TYPE == BSP_CPU_X86)
#else
  #error "CPU unknown!"
#endif /* !defined (BSP_CPU_TYPE) */


#if !defined(__ASSEMBLER__)
#include "typedefs.h"
#endif /* defined(__ASSEMBLER__) */

#endif /* BSP_H */

