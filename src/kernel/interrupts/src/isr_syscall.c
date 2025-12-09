#include "inttype.h"
#include "isr_syscall.h"
#include "str_utils.h"
#include "screen.h"
#include "kernel.h"


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
            ret = isr_syscall_read((char*)arg0, (uint8_t)arg1);
            break;
        case SYS_COMMREAD:
            ret = isr_syscall_commread((uint8_t*)arg0, (uint8_t)arg1);
            break;
        case SYS_SCROLL:
            ret = isr_syscall_scroll((uint8_t)arg0, arg1);
            break;
        case SYS_SCREENSET:
            ret = isr_syscall_screenset((uint8_t)arg0, (uint8_t)arg1);
            break;
        case SYS_COLORSET:
            ret = isr_syscall_colorset((uint8_t)arg0, (uint8_t)arg1);
            break;
        case SYS_ERRWRITE:
            ret = isr_syscall_errwrite((char *)arg0, arg1, arg2);
            break;
        case SYS_ERRPRINT:
            ret = isr_syscall_errprint(arg0);
            break;
        case SYS_HEXDUMP:
            ret = isr_syscall_hexdump((void *)arg0, arg1);
    
        default:
            break;
    }
    return (ret);
}