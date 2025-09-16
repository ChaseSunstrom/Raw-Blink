/* init.ld - minimal AVR linker script for ATmega2560 */

MEMORY {
  text (rx)  : ORIGIN = 0x0000, LENGTH = 256K
  data (rw!x): ORIGIN = 0x0200, LENGTH = 8K
}

SECTIONS {
  /* Interrupt vectors and program code in flash */
.text : {
    KEEP(*(.vectors))         /* keep vector table */
    KEEP(*(.text._reset))     /* keep reset handler */
    KEEP(*(.text._start))
    *(.init*)                 /* optional init sections */
    *(.text*)                 /* keep all code */
    *(.rodata*)               /* const data */
} > text

  /* Initialized variables (copied from flash to SRAM at startup) */
.data : AT(ADDR(.text) + SIZEOF(.text)) {
    __data_start = .;
    *(.data*)
    __data_end = .;
} > data

  __data_load_start = LOADADDR(.data);

  /* Uninitialized variables (zeroed at startup) */
  .bss (NOLOAD) : {
    __bss_start = .;
    *(.bss*)
    *(COMMON)
    __bss_end = .;
  } > data
}

