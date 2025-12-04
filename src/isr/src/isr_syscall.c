#include "inttype.h"
#include "isr_syscall.h"

uint32_t isr_syscall(uint32_t num, uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4,  uint32_t arg5)
{
    arg2 = arg2;
    arg3 = arg3;
    arg4 = arg4;
    arg5 = arg5;
    uint32_t ret = 1;
    switch (num)
    {
        case SYS_WRITE:
            ret = isr_syscall_write((char *)arg0, arg1);
            break;
        case SYS_READ:
            
            break;
        case SYS_PRINTERR:
            ret = isr_syscall_printerr(arg0);
            break;

        case SYS_SCROLL:
            ret = isr_syscall_scroll(arg0, arg1);
            break;
    
        default:
            break;
    }
    return (ret);
}