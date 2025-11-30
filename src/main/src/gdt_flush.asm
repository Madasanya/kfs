; void gdt_flush(uint32_t gdtr_ptr);
; Called from init_gdt() after filling the table

global gdt_flush
%define KERNEL_CODE_SEL  0x08
%define KERNEL_DATA_SEL  0x18
%define KERNEL_BSS_SEL   0x28
%define KERNEL_TSS_SEL   0x38

section .text
align 16
gdt_flush:
    lgdt [esp + 4]              ; load GDTR

    ; Reload all segment registers for kernel
    mov   ax, KERNEL_DATA_SEL
    mov   ds, ax
    mov   es, ax
    mov   fs, ax
    mov   gs, ax

    mov   ax, KERNEL_BSS_SEL
    mov   ss, ax

    ; Far jump to reload CS
    jmp   KERNEL_CODE_SEL:.flush_cs
.flush_cs:
    ret

; C declaration: void flush_tss(void);
global tss_flush
tss_flush:
	mov ax, (5 * 8) | 0 ; fifth 8-byte selector, symbolically OR-ed with 0 to set the RPL (requested privilege level).
	ltr ax
	ret