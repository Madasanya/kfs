# Manual GDB commands to connect and debug
file build/md_kernel.bin
set architecture i8086
target remote localhost:1234
disconnect
set architecture i386
target remote localhost:1234
break nanoshell_screen_switch:13
break nanoshell_hexdump
break nanoshell_run
break nanoshell_run
b nanoshell_gnl.c:104
b nanoshell.c:18
c
layout regs
