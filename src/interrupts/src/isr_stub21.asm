; isr_irq1.asm - Keyboard IRQ handler
section .text
align 16

%define KERNEL_DATA_SEL  0x10

extern irq1_keyboard_handler

global isr_stub21

; IRQ1 (Keyboard) interrupt handler
isr_stub21:
    ; already on stack: ss, sp, flags, cs, ip.
    ; need to push ax, gs, fs, es, ds, -ENOSYS, bp, di, si, dx, cx, and bx
    push ds
;    push dword -ENOSYS
    push ebp
    push edi
    push esi
    push edx
    push ecx
    push ebx
    push eax
    mov ax, KERNEL_DATA_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call irq1_keyboard_handler
    add esp, 28
;    pop word -ENOSYS
;    pop word ds
    xor edx, edx
    pop edx
    mov ds, dx
    mov es, dx
    mov fs, dx
    mov gs, dx
    iretd
