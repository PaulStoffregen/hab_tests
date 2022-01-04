#ifndef BOOT_LD_S 
#define BOOT_LD_S

OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_reset)
//ENTRY(_start)
//ENTRY(ivt)

#include "bsp.h"

#if !defined(MAX_HAB_CSF_DATA_SIZE)
MAX_HAB_CSF_DATA_SIZE = 0x2000;
#endif

#if (BSP_BOARD_TYPE == BSP_BRD_MIMXRT1060_EVK)
#include "imxrt_map.ld.h"
#elif (BSP_BOARD_TYPE == BSP_BRD_TEENSY40)
#include "imxrt_map.ld.h"
#else
#error "Unknown board type!"
#endif

_Min_Heap_Size = 0x800;      /* required amount of heap  */
_Min_Stack_Size = 0x400; /* required amount of stack */

/*
 * Definition of output sections
 */
SECTIONS
{
  /* Set location counter to address zero  */
  . = 0;
  /* Remember image start */
  __image_start = ORIGIN(EXT_FLASH);

  /*
   * IMXRT needs a flash configuration at the beginning of external flash
   */
  .flash_cfg :
  {
    KEEP(*(.flash_cfg))
    FILL(0xFF)
    . = ALIGN(0x200);
  } > EXT_FLASH
  __fls_cfg_start = (SIZEOF(.flash_cfg) > 0) ? (ADDR(.flash_cfg)) : 0;
  __fls_cfg_end = (SIZEOF(.flash_cfg) > 0) ? (ADDR(.flash_cfg) + SIZEOF(.flash_cfg) - 1) : 0;


  .text.sw_info :
  {
    __sw_info_start = .;
    KEEP(*(.sw_info))
    KEEP(*(.sw_rel_info))
    __sw_info_end = .;
  } > EXT_FLASH


  .text.boot :
  {
    . = ALIGN(4);
    __boot_start = .;      /* VMA of .text.boot section */

    /* IRQ / exception vectors and startup code goes into the first section.
     * It might, but doesn't necessarily has to go into a sepeate section,
     * so let it go into the .text section.
     * The input section name is defined in the entry / startup file via either
     * 
     *   ".section <section-name>" 
     *
     * in assembly files or
     *
     *   "__attribute__((section("<section-name>")))
     *
     * in C-files.
     * Make sure it is aligned to 2^4.
     */
    . = ALIGN(0x400);
    *(.armv7m_vect) /* Section defined for vectors in armv7m_entry.S */
    *(.m7start)     /* Section defined for start code in armv7m_entry.S */
    *(.armboot)
    *(.exit)
    *init*.o(.text)
    __boot_end = .;
    . = ALIGN(0x1000);
  } > EXT_FLASH
  
  PROVIDE(__boot_load = LOADADDR(.text.boot));

  /*
   * IMX processors need an Image Vector Table (IVT)
   */
  .text.ivt :
  {
    /* Collect .ivt section from source objects */
    __ivt_start = .;
    KEEP(*(.ivt))
    __ivt_end = .;

    /* Provide global symbol defining the start address of the
     * boot data. This is referenced by the ivt code.
     */
    __boot_data_start = .;
    /* Collect .boot_data section from source objects */
    KEEP(*(.boot_data))
    __boot_data_end = .;
  } > EXT_FLASH

  PROVIDE(__ivt_load = LOADADDR(.text.ivt));

  /*
   * If no Device Configuration Data is used, define addresses as 0
   */
  .text.dcd :
  {
    KEEP(*(.dcd))
  } > EXT_FLASH
  __dcd_start = (SIZEOF(.text.dcd) > 0) ? (ADDR(.text.dcd)) : 0;
  __dcd_end =   (SIZEOF(.text.dcd) > 0) ? (ADDR(.text.dcd) + SIZEOF(.text.dcd) - 1) : 0;


  /*
   * Main code section
   */
  .text.progmem :
  {
    . = ALIGN(4);
    __text_start = .;
    __text_exec = .;      /* VMA of .text section */

    /*
     * Program code goes into .text section
     */
    *(.text*)             /* all .text sections (code)  */

    *(.note.gnu.build-id) /* build-id used in later versions of GCC */
    *(.gnu.linkonce.t.*)  /* vague linking */
    *(.glue_7t)           /* glue thumb to arm code */
    *(.glue_7)            /* glue arm to thumb code */
    *(.vfp11_veneer)
    *(.eh_frame)

    KEEP (*(.init))      /* .init sections */
    KEEP (*(.fini))      /* .fini sections */
    *(.gcc_except_table)

    /* .preinit_array : */
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);

    /* .init_array : */
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);

    /* .fini_array : */
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);

    . = ALIGN(4);

    /* C++ constructors are assembled into input sections .ctors.
     * They might, but do not necessarily have to go into a seperate section,
     * so let them go into .text section.
     */
    PROVIDE(__ctors_start__ = .);
    KEEP(*(SORT(.ctors.*)))
    KEEP(*(.ctors))
    PROVIDE(__ctors_end__ = .);

    /* C++ destructors are assembled into input sections .dtors.
     * They might, but do not necessarily have to go into a seperate section,
     * so let them go into .text section.
     */
    PROVIDE(__dtors_start__ = .);
    KEEP(*(SORT(.dtors.*)))
    KEEP(*(.dtors))
    PROVIDE(__dtors_end__ = .);


    /* Constant data is assembled into input sections .rodata.
     * It might, but doesn't necessarily has to go into a sepeate section,
     * so let it go into the .text section.
     */
    . = ALIGN(4);
    *(.rodata)          /* all .rodata sections (constants, strings, etc.)  */
    *(.rodata*)         /* all .rodata* sections (constants, strings, etc.)  */

    *(.gnu.linkonce.r.*)

    . = ALIGN(4);       /* 4-Byte alignment of _text_end */
    *(.ARM.extab*)
    *(.gnu.linkonce.armextab.*)

    __exidx_start = .;
    *(.ARM.exidx*)
    . = ALIGN(4);

    __text_end = .;
  } > OCRAM AT > EXT_FLASH /* Put all the above into RAM */

  PROVIDE(__text_load = LOADADDR(.text.progmem)); /* LMA */


  /* Initialized data sections go into RAM, load LMA copy after code */
  .data : ALIGN(4)
  {
    . = ALIGN(4);
    /* Provide global symbol marking the start of the .data section */
    __data_start = .;
    __data_exec = .; /* VMA */
    /* Collect all initialized .data sections */
    *(.data)
    *(.data*)          /* .data* sections */

    *(.gnu.linkonce.d.*)
    SORT(CONSTRUCTORS)

    . = ALIGN(4);
    *(.fastrun .fastrun.*)
    *(.ramCode .ramCode.*)
    . = ALIGN(4);
    __data_end = .;
  } > OCRAM AT > EXT_FLASH = 0x5A

  PROVIDE(__data_load = LOADADDR(.data)); /* LMA */

  .text.pad :
  {
    __pad_start = .;
    BYTE(0xA5)
    FILL(0x5A)
    . = ALIGN(0x1000);
    __pad_end = .;
  } > EXT_FLASH


  __image_size = SIZEOF(.text.boot) + SIZEOF(.text.ivt) + SIZEOF(.text.progmem) + SIZEOF(.data) + SIZEOF(.text.pad) + MAX_HAB_CSF_DATA_SIZE;


  /* The .bss section comes after the hab data because it is not signed */
  .bss (NOLOAD) : ALIGN(0x100)
  {
    /* define a global symbol marking the start of the .bss section */
    __bss_start = .;
    /* Collect all uninitialized .bss sections */
    *(.bss)
    *(.bss*)

    *(.gnu.linkonce.b.*)
    *(COMMON)

    . = ALIGN(4);
    __bss_end = .;
  } > OCRAM

  .csf (NOLOAD) : ALIGN(0x400)
  {
    /* */
    *(.csf)
  } > EXT_FLASH
  __csf_start = ADDR(.csf);
  __csf_end =   ADDR(.csf) + MAX_HAB_CSF_DATA_SIZE;

  PROVIDE(__bss_exec = ADDR(.bss)); /* VMA */


  .heap (NOLOAD) :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    *(.heap)
    . += _Min_Heap_Size;
    . = ALIGN(8);
    __heap_end__ = .;
  } > OCRAM

  .stack (NOLOAD) :
  {
    . = ALIGN(8);
    __stack_start = .;
    __stack_start__ = .;
    *(.stack)
    . = ALIGN(8);
    . += _Min_Stack_Size;
    __stack_end = .;
    . = ALIGN(8);
  } > OCRAM

  .crash.stack (NOLOAD) :
  {
    __crash_stack_start = .;
    *(.crash.stack)
    __crash_stack_end = .;
  } > CRASH_STACK

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }
  .ARM.attributes 0 : { *(.ARM.attributes) }
}
//PROVIDE(_end = .); /* define a global symbol marking the end of application RAM */

#endif /* BOOT_LD_S */

