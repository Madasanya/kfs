%define KERNEL_DATA_SEL  0x10

extern isr_syscall

global isr_stub80

isr_stub80:
    ; already on stack: ss, sp, flags, cs, ip.
    ; need to push ax, gs, fs, es, ds, -ENOSYS, bp, di, si, dx, cx, and bx
    cli
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
    sti
    call isr_syscall
    cli
    add esp, 28
;    pop word -ENOSYS
;    pop word ds
    xor edx, edx
    pop edx
    mov ds, dx
    mov es, dx
    mov fs, dx
    mov gs, dx
    sti
    iretd