; asm/user_enter.asm
; First-time entry into a brand-new user task
; void user_enter(void *user_eip);
;   user_eip = linear address inside user code segment (e.g. 0x00300000 + offset)

global user_enter
%define USER_CODE_SEL 0x13
%define USER_DATA_SEL 0x23
%define USER_BSS_SEL  0x33

section .text
align 16

user_enter:
    ; Save kernel state (we never return here)
    push ebp
    mov  ebp, esp

    ; Parameters
    mov  eax, [ebp + 8]          ; user_eip (first argument)

    ; ------------------------------------------------------------------
    ; 1. Load ALL user-mode segment registers
    ; ------------------------------------------------------------------
    mov  cx, USER_DATA_SEL
    mov  ds, cx
    mov  es, cx
    mov  fs, cx
    mov  gs, cx

    mov  cx, USER_BSS_SEL
    mov  ss, cx

    ; ------------------------------------------------------------------
    ; 2. Set up a SAFE user stack
    ;    Top of your 1 MB user BSS segment: 0x00600000 + 0x100000 = 0x00700000
    ;    We subtract 32 bytes just to be safe
    ; ------------------------------------------------------------------
    mov  esp, 0x00700000 - 32

    ; ------------------------------------------------------------------
    ; 3. Build the IRET frame on the user stack
    ;    Order: SS, ESP, EFLAGS, CS, EIP
    ; ------------------------------------------------------------------
    push USER_BSS_SEL            ; User SS
    push 0x00700000 - 32         ; User ESP (after this frame)

    pushf                        ; EFLAGS
    pop  ecx
    or   ecx, 0x200              ; Enable interrupts (IF = 1)
    push ecx

    push USER_CODE_SEL           ; User CS (0x13)
    push eax                     ; User EIP (entry point)

    ; ------------------------------------------------------------------
    ; 4. Final jump to ring-3 — we never come back!
    ; ------------------------------------------------------------------
    iret

    ; If user code ever returns here (should never happen), we hang safely
.hang:
    cli
    hlt
    jmp .hang