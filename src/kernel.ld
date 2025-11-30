ENTRY(_start)

SECTIONS
{
    . = 1M;  /* Load kernel at 1MB physical address */

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot1)  /* For GRUB/Multiboot header if used */
        *(.kernel_info) /* kernel information (version etc.) */
        *(.text)
        *(.rodata)
    }

. = 3M;

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
        *(COMMON)
        *(.bss)
    }

    /* . = 3M; 

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    } */
}