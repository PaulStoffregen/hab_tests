MEMORY
{
	ITCM     (rwx): ORIGIN = 0x00000000, LENGTH = 512K
	DTCM     (rwx): ORIGIN = 0x20000000, LENGTH = 512K
	OCRAM2   (rwx): ORIGIN = 0x20200000, LENGTH = 512K
	HAB_RAM  (rwx): ORIGIN = 0x20200000, LENGTH =   7K
	RAM      (rwx): ORIGIN = 0x20202000, LENGTH = 504K
	FLEX_RAM (rwx): ORIGIN = 0x20280000, LENGTH = 512K
	FLASH    (rwx): ORIGIN = 0x60000000, LENGTH = 1984K
}

ENTRY(ImageVectorTable)

MAX_HAB_CSF_SIZE = 0x2000;

SECTIONS
{
	.text.flashconfig : {
		__start_image = .;
		_simage = .;
		_sfls_cfg = .;
		KEEP(*(.flashconfig))
		_efls_cfg = .;
		FILL(0xCF)
		. = ORIGIN(FLASH) + 0x1000;
	} > FLASH

	.text.progmem : {
		KEEP(*(.ivt))
		KEEP(*(.bootdata))
		KEEP(*(.dcd))
		. = ALIGN(0x1000);
		KEEP(*(.vectors))
		KEEP(*(.startup))
		*(.progmem*)
                . = ALIGN(4);
                KEEP(*(.init))
                __preinit_array_start = .;
                KEEP (*(.preinit_array))
                __preinit_array_end = .;
                __init_array_start = .;
                KEEP (*(.init_array))
                __init_array_end = .;
		. = ALIGN(16);
		*(.text*)
		. = ALIGN(16);
	} > FLASH

    .itcm.trap (NOLOAD): {
		. = . + 32; /* MPU to trap NULL pointer deref */
	}
	
	.text.itcm : {
		*(.fastrun)
		. = ALIGN(16);
	} > ITCM  AT> FLASH

	.data : {
		*(.rodata*)
		*(.data*)
		. = ALIGN(16);
	} > DTCM AT> FLASH

	.pad : {
		_spad = .;
/*		KEEP(*(.pad)) */
		BYTE(0xAF);
		FILL(0x5A);
		. = ALIGN(0x1000);
		_epad = .;
	} > FLASH

    __image_size = SIZEOF(.text.progmem) + SIZEOF(.text.itcm) + SIZEOF(.data) + MAX_HAB_CSF_SIZE;
    __image_end  = __start_image + __image_size;
	_eimage = __image_end;

	.csf : {
		KEEP(*(.csf))
	} > FLASH

	.bss (NOLOAD): ALIGN(4) {
		*(.bss*)
		*(COMMON)
		. = ALIGN(32);
		. = . + 32; /* MPU to trap stack overflow */
	} > DTCM

	.bss.dma (NOLOAD) : {
		*(.dmabuffers)
		. = ALIGN(16);
	} > RAM

/*    _sfls_cfg = ADDR(.text.progmem); */

    _sflash = ADDR(.text.progmem);
    _eflash = ADDR(.text.progmem) + SIZEOF(.text.progmem);
	
	_stext = ADDR(.text.itcm);
	_etext = ADDR(.text.itcm) + SIZEOF(.text.itcm);
	_stextload = LOADADDR(.text.itcm);

	_sdata = ADDR(.data);
	_edata = ADDR(.data) + SIZEOF(.data);
	_sdataload = LOADADDR(.data);

    _scsf = _epad;
	_ecsf = _epad + MAX_HAB_CSF_SIZE;
	_sbss = ADDR(.bss);
	_ebss = ADDR(.bss) + SIZEOF(.bss);

	_heap_start = ADDR(.bss.dma) + SIZEOF(.bss.dma);
	_heap_end = ORIGIN(RAM) + LENGTH(RAM);

	_itcm_block_count = (SIZEOF(.text.itcm) + 0x7FFE) >> 15;
	_flexram_bank_config = 0xAAAAAAAA | ((1 << (_itcm_block_count * 2)) - 1);
	_estack = ORIGIN(DTCM) + ((16 - _itcm_block_count) << 15);

	_flashimagelen = SIZEOF(.text.progmem) + SIZEOF(.text.itcm) + SIZEOF(.data);
	_teensy_model_identifier = 0x24;

	.debug_info     0 : { *(.debug_info) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }

}
