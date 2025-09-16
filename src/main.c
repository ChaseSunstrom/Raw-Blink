#include "main.h"
#include "types.h"

#define _MMIO_BYTE(_address) (*(volatile uint8_t *)(_address))

#define WDTCSR _MMIO_BYTE(0x60)
#define DDRB _MMIO_BYTE(0x24)
#define PORTB _MMIO_BYTE(0x25)

__attribute__((used)) static void _disable_wdt(void) {
  WDTCSR = (1 << 4) | (1 << 3);
  WDTCSR = 0x00;
}
__attribute__((used)) static void _init_data(void) {
  for (uint8_t *src = &__data_load_start, *dst = &__data_start;
       src < &__data_end; src++, dst++) {
    *dst = *src;
  }
}

__attribute__((used)) static void _init_bss(void) {
  for (uint8_t *p = &__bss_start; p < &__bss_end; p++) {
    *p = 0x00;
  }
}

__attribute__((used)) static void _start(void) {
  _init_data();
  _init_bss();
  _disable_wdt();

  DDRB |= (1 << 7);
  while (1) {
    PORTB ^= (1 << 7);

    for (int32_t i = 0; i < 16000UL * 500; i++) {
      asm volatile("nop");
    }
  }
}
