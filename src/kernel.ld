ENTRY(_start_multiboot1)

SECTIONS
{
    . = 1M;  /* Kernel loaded at 1 MB */

    .multiboot1 : {
        . = ALIGN(8);
        *(.multiboot1)
    }

    .text : { 
        . = ALIGN(8);
        *(.text) }
    .rodata : { *(.rodata) }
    .data   : { *(.data) }
    .bss    : { *(COMMON) *(.bss) }
}