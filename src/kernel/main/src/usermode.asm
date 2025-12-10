; asm/user_enter.asm
; First-time entry into a brand-new user task
; void user_enter(void *user_eip);
;   user_eip = linear address inside user code segment (e.g. 0x00300000 + offset)

global user_enter
%define USER_CODE_SEL 0x23
%define USER_DATA_SEL 0x2B
%define USER_BSS_SEL  0x33

extern user_stack_top
extern user_main_phys

section .text
align 16
user_enter:
    ; Save kernel state (we never return here)
    push ebp
    mov  ebp, esp

    ; Parameters
    mov  eax, [esp + 8]           ; user_eip (first argument)
    sub eax, 0x00200000        ; Convert to linear address (user code segment base)
    ; ------------------------------------------------------------------
    ; 1. Build the IRET frame on the KERNEL stack first
    ;    Order: SS, ESP, EFLAGS, CS, EIP
    ; ------------------------------------------------------------------
    push USER_BSS_SEL            ; User SS

    

    push user_stack_top - 0x200000             ; User ESP (top of BSS - 32 bytes)

    pushfd                        ; EFLAGS last d -makes sure that 32bit flags are pushed
    pop  ecx
    or   ecx, 0x200              ; Enable interrupts (IF = 1)
    push ecx

    push USER_CODE_SEL          ; User CS
    push eax                     ; User EIP (entry point)

    ; ------------------------------------------------------------------
    ; 2. Load user-mode DATA segment registers (not SS yet!)
    ; ------------------------------------------------------------------
    mov  cx, USER_DATA_SEL
    mov  ds, cx
    mov  es, cx
    mov  fs, cx
    mov  gs, cx

    ; ------------------------------------------------------------------
    ; 3. Final jump to ring-3 — IRET loads SS:ESP, CS:EIP, EFLAGS
    ; ------------------------------------------------------------------
    sti
    iret

    ; If user code ever returns here (should never happen), we hang safely
.hang:
    cli
    hlt
    jmp .hang