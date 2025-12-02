global user_syscall

section .user_text
user_syscall:
mov eax [esp + 4]
mov ebx [esp + 8]
mov ecx [esp + 12]
int 0x80
ret