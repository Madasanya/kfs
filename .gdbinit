# Manual GDB commands to connect and debug
file build/md_kernel.bin
set architecture i8086
target remote localhost:1234
disconnect
set architecture i386
target remote localhost:1234
break user_main
break user_syscall
break user_syserrwrite
break user_syserrprint
break isr_stub80
c
layout regs
