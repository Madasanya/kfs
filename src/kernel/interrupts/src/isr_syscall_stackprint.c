#include "inttype.h"
#include "hexdump.h"

extern uint32_t kernel_stack_bottom[];
extern uint32_t kernel_stack_top[];

uint32_t isr_syscall_stackprint(void)
{
    return (md_hexdump_print(kernel_stack_bottom, (uint32_t)kernel_stack_top -  (uint32_t)kernel_stack_bottom)); 
}