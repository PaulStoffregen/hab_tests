/*
 ******************************************************************************
 * @file     typedefs.h
 * @details
 *   This file defines all commonly used data types.
 * @author   Frank Manig
 * @date     2007-12-11
 * @brief
 *   Common data types.
 *
 ******************************************************************************
 */

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef unsigned char       U8;
typedef unsigned short     U16;
typedef unsigned int       U32;
typedef unsigned long long U64;

typedef signed char         S8;
typedef signed short       S16;
typedef signed int         S32;
typedef signed long long   S64;


typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;

typedef signed char        sint8;
typedef signed short       sint16;
typedef signed int         sint32;
typedef signed long long   sint64;

typedef uint8              boolean;


/* Fast types */

typedef uint_fast8_t       U8F;
typedef uint_fast16_t     U16F;
typedef uint_fast32_t     U32F;
typedef uint_fast64_t     U64F;

typedef int_fast8_t        S8F;
typedef int_fast16_t      S16F;
typedef int_fast32_t      S32F;
typedef int_fast64_t      S64F;


typedef uint_fast8_t       uint8_least;
typedef uint_fast16_t     uint16_least;
typedef uint_fast32_t     uint32_least;
typedef uint_fast64_t     uint64_least;

typedef int_fast8_t        sint8_least;
typedef int_fast16_t      sint16_least;
typedef int_fast32_t      sint32_least;
typedef int_fast64_t      sint64_least;


typedef enum STATUS
{
  STATUS_eOK = 0,
  STATUS_eNOK = 1,
}T_STATUS;


typedef void* T_HANDLE;

#if !defined(_WIN32)
#if defined NULL
#undef NULL
#endif /* defined NULL */

#define NULL ((void*)0)
#else /* defined(_WIN32) */
#if !defined NULL
#define NULL ((void*)0)
#endif /* !defined NULL */
#endif /* !defined(_WIN32) */


#define STD_OFF 0
#define STD_ON  1

#ifndef FALSE
#define FALSE 0
#elif (FALSE != 0)
#undef FALSE
#define FALSE 0    
#endif

#if defined TRUE
#undef TRUE
#endif

#ifdef __cplusplus
}
#endif

#endif /* TYPEDEFS_H */

