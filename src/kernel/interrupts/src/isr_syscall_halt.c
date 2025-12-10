#include "inttype.h"

uint32_t isr_syscall_halt(void)
{
    asm volatile ("hlt");
    return (0);
}