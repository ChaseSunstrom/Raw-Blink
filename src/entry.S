.section .vectors, "ax"
.global _reset
.org 0x0000
    jmp _reset

.text
.type _reset, @function
_reset:
    ldi r16, hi8(0x21FF)
    out SPH, r16
    ldi r16, lo8(0x21FF)
    out SPL, r16

    rjmp _start

