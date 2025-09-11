/* blink.ld - minimal AVR linker script for ATmega2560 */

MEMORY {
  text   (rx) : ORIGIN = 0,       LENGTH = 256K
  data   (rw) : ORIGIN = 0x800200, LENGTH = 8K
}

SECTIONS {
  /* Code in flash */
  .text : {
    KEEP(*(.vectors))     /* keep the reset/vector table */
    *(.init0) *(.init1) *(.init2) *(.init3) *(.init4) *(.init5)
    *(.init6) *(.init7) *(.init8) *(.init9)
    *(.text*)             /* actual code */
    *(.rodata*)           /* const data */
    . = ALIGN(2);
    __data_load_start = .;   /* flash load addr for .data */
  } > text

  /* Initialized data in SRAM */
  .data : AT(__data_load_start) {
    __data_start = .;
    *(.data*)
    __data_end = .;
  } > data

  /* Uninitialized data (zeroed at startup) */
  .bss (NOLOAD) : {
    __bss_start = .;
    *(.bss*)
    *(COMMON)
    __bss_end = .;
  } > data
}
