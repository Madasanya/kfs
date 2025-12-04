global user_syscall

section .text
user_syscall:
    push ebp
    push edi
    push esi
    push ebx

    mov eax, [esp + 20]
    cmp eax, 0  ;syswrite
    je syscall_2arg
    cmp eax, 3 ;syserrwrite
    je syscall_3arg
    cmp eax, 4 ;syserrprint
    je syscall_1arg
    jmp syscall_0arg
syscall_6arg:
    mov ebp, [esp + 44]
syscall_5arg:
    mov edi, [esp + 40]
syscall_4arg:
    mov esi, [esp + 36]
syscall_3arg:
    mov edx, [esp + 32]
syscall_2arg:
    mov ecx, [esp + 28]
syscall_1arg:
    mov ebx, [esp + 24]
syscall_0arg:
    int 0x80

    pop ebx
    pop esi
    pop edi
    pop ebp

    ret