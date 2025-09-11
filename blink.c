typedef unsigned char uint8_t;

#define _MMIO_BYTE(mem_addr) (*(volatile uint8_t *)(mem_addr))

#define SPL     _MMIO_BYTE(0x3D)
#define SPH     _MMIO_BYTE(0x3E)
#define WDTCSR  _MMIO_BYTE(0x60)
#define DDRB    _MMIO_BYTE(0x24)  // 0x44
#define PORTB   _MMIO_BYTE(0x25)  // 0x45

extern uint8_t __bss_start, __bss_end;
extern uint8_t __data_start, __data_end, __data_load_start;

__attribute__((naked, section(".vectors")))
void vectors(void) {
    asm volatile ( "jmp main" );
}

static void init_stack(void) {
    SPL = 0xFF;
    SPH = 0x21;   // 0x21FF = top of SRAM
}

void disable_wdt(void) {
    WDTCSR = (1 << 4) | (1 << 3); // WDCE | WDE
    WDTCSR = 0x00;
}

static void init_bss(void) {
    uint8_t *p = &__bss_start;
    while (p < &__bss_end) {
        *p++ = 0;
    }
}

static void init_data(void) {
    const uint8_t *src = &__data_load_start;
    uint8_t *dst = &__data_start;
    while (dst < &__data_end) {
        *dst++ = *src++;
    }
}

int main(void) {

    init_stack();
    disable_wdt();
    init_bss();
    init_data();

    DDRB |= (1 << 7); // PB7 output (LED)
    while (1) {
        PORTB ^= (1 << 7); // toggle LED
        for (volatile unsigned long i = 0; i < 160000UL; i++) {
            __asm__ __volatile__("nop");
        }
    }
}