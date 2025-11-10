ENTRY(_start)

SECTIONS
{
    . = 1M;  /* Load kernel at 1MB physical address */

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot1)  /* For GRUB/Multiboot header if used */
        *(.kernel_info) /* kernel information (version etc.) */
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
}