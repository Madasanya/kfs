ENTRY(_start)

SECTIONS
{
    /* Kernel BSS at 4 MB */
    . = 4M;
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }

    /* Kernel DATA at 5 MB */
    . = 5M;
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    /* Kernel CODE at 6 MB */
    . = 6M;
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot1)
        *(.kernel_info)
        *(.text)
        *(.rodata)
    }
}