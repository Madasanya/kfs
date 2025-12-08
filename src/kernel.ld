ENTRY(_start)

SECTIONS
{
    /* User BSS at 1 MB */
    . = 1M;
    .user_bss BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot1)
        *(.user_bss)
    }

    /* User DATA at 2 MB */
    . = 2M;
    .user_data BLOCK(4K) : ALIGN(4K)
    {
        *(.user_data)
    }

    /* User CODE at 3 MB */
    . = 3M;
    .user_text BLOCK(4K) : ALIGN(4K)
    {
        *(.user_text)
    }
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
        *(.kernel_info)
        *(.text)
    }
}