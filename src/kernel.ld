ENTRY(_start)

SECTIONS
{
    /* User BSS at 1 MB */
    . = 1M;
    .multiboot1 BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot1)
    }
    .user_bss BLOCK(4K) : ALIGN(4K)
    {
        PROVIDE(_user_stack_start = .);
        *(.user_stack)
        . = ALIGN(4K);
        PROVIDE(_user_stack_end = .);
        PROVIDE(_user_data_start = .);
        *(.user_bss)
    }
    
    .user_data BLOCK(4K) : ALIGN(4K)
    {
        *(.user_data)
        . = ALIGN(4K);
        PROVIDE(_user_data_end = .);
    }

    .user_text BLOCK(4K) : ALIGN(4K)
    {
        PROVIDE(_user_text_start = .);
        *(.user_text)
        . = ALIGN(4K);
        PROVIDE(_user_text_end = .);
    }
    
    .bss BLOCK(4K) : ALIGN(4K)
    {
        PROVIDE(_kernel_stack_start = .);
        *(.stack)
        . = ALIGN(4K);
        PROVIDE(_kernel_stack_end = .);
        PROVIDE(_kernel_data_start = .);
        *(.bss)
        *(COMMON)
    }
    
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
        *(.tss)
        . = ALIGN(4K);
        PROVIDE(_kernel_data_end = .);
    }
    .text BLOCK(4K) : ALIGN(4K)
    {
        PROVIDE(_kernel_text_start = .);
        *(.kernel_info)
        *(.text)
        . = ALIGN(4K);
        PROVIDE(_kernel_text_end = .);
    }


}