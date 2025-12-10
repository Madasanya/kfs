ENTRY(_start)
EXTERN(user_main)

SECTIONS
{
    /* User BSS at 1 MB */
    . = 1M;
    .multiboot1 BLOCK(4K) : ALIGN(4K)
    {
        kernel/*(.multiboot1)
        . = ALIGN(4K);
    }
    
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.boot_text)
        kernel/*(.kernel_info)
        kernel/*(.text)
        . = ALIGN(4K);
    }
    
    .data BLOCK(4K) : ALIGN(4K)
    {
        kernel/*(.tss)
        . = ALIGN(4K);
        kernel/*(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        kernel/*(.bss)
        kernel/*(COMMON)
        . = ALIGN(4K);
        *(.kernel_stack)
    }

    . = 2M;

    .user_text BLOCK(4K) : ALIGN(4K)
    {
        PROVIDE(_user_mem_start = .);
        user/*(.text)
        *(.user_text)
        . = ALIGN(4K);
    }

    .user_data BLOCK(4K) : ALIGN(4K)
    {
        *(.user_data)
        user/*(.data)
    }

        .user_bss BLOCK(4K) : ALIGN(4K)
    {
        *(.user_bss)
        user/*(.bss)
        user/*(COMMON)
        . = ALIGN(4K);
        *(.user_stack)
        . = ALIGN(4K);
        PROVIDE(_user_mem_end = .);
    }

    user_main_phys = user_main;

}