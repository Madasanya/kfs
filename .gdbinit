# Manual GDB commands to connect and debug
file build/md_kernel.bin
set architecture i8086
target remote localhost:1234
disconnect
set architecture i386
target remote localhost:1234
break idt_init
break isr_dummy_nonblocking
break user_enter
break idt_flush
c
layout regs
