ENTRY(_start)
EXTERN(user_main)

SECTIONS
{
    /* User BSS at 1 MB */
    . = 1M;
    .multiboot1 BLOCK(4K) : ALIGN(4K)
    {
        kernel/*(.multiboot1)
    }
    
    .text BLOCK(4K) : ALIGN(4K)
    {
        PROVIDE(_kernel_text_start = .);
        kernel/*(.kernel_info)
        kernel/*(.text)
        . = ALIGN(4K);
        PROVIDE(_kernel_text_end = .);
    }
    
    .data BLOCK(4K) : ALIGN(4K)
    {
        kernel/*(.data)
        kernel/*(.tss)
        . = ALIGN(4K);
        PROVIDE(_kernel_data_end = .);
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        PROVIDE(_kernel_data_start = .);
        kernel/*(.bss)
        kernel/*(COMMON)
        PROVIDE(_kernel_stack_start = .);
        *(.kernel_stack)
        PROVIDE(_kernel_stack_end = .);
    }

    . = 2M;

    .user_text BLOCK(4K) : ALIGN(4K)
    {
        PROVIDE(_user_text_start = .);
        user/*(.text)
        *(.user_text)
        . = ALIGN(4K);
        PROVIDE(_user_text_end = .);
    }

    .user_data BLOCK(4K) : ALIGN(4K)
    {
        *(.user_data)
        user/*(.data)
        . = ALIGN(4K);
        PROVIDE(_user_data_end = .);
    }

        .user_bss BLOCK(4K) : ALIGN(4K)
    {
        PROVIDE(_user_data_start = .);
        PROVIDE(_user_stack_start = .);
        *(.user_bss)
        user/*(.bss)
        user/*(COMMON)
        PROVIDE(_user_stack_end = .);
        *(.user_stack)
    }

    
    user_main_phys = user_main;

}