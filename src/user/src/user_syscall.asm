global user_syscall

section .user_text
user_syscall:
    push ebp
    push edi
    push esi
    push ebx

    mov eax, [esp + 20]
    cmp eax, 0  ;syswrite
    je syscall_2arg
    cmp eax, 10 ;sysscroll
    je syscall_2arg
    cmp eax, 11 ;sysscreenset
    je syscall_2arg
    cmp eax, 12 ;syscolorset
    je syscall_2arg
    cmp eax, 13 ;syshexdump
    je syscall_2arg
    cmp eax, 3 ;syserrwrite
    je syscall_3arg
    cmp eax, 4 ;syserrprint
    je syscall_1arg
    cmp eax, 1 ;sysread
    je syscall_2arg
    cmp eax, 2 ;syscommread
    je syscall_2arg
    cmp eax, 5 ;syshalt
    je syscall_0arg
    cmp eax, 14 ;sysstackprint
    je syscall_0arg
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