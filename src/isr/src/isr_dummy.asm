; dummy_isr.asm — NASM syntax
section .text
align 16

global isr_dummy_nonblocking
global isr_dummy_blocking

; Non-blocking: just return immediately
isr_dummy_nonblocking:
    iret

; Blocking: disable interrupts and halt forever
isr_dummy_blocking:
    cli
    hlt
    jmp isr_dummy_blocking