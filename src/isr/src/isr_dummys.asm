; dummy_isr.asm — NASM syntax
section .text
align 16

global isr_dummy_nonblocking
global isr_dummy_blocking

; Non-blocking: just return immediately
dummy_isr_nonblocking:
    iret

; Blocking: disable interrupts and halt forever
dummy_isr_blocking:
    cli
    hlt
    jmp dummy_isr_blocking