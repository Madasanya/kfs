#include "inttype.h"
#include "isr_syscall.h"
#include "str_utils.h"
#include "screen.h"
#include "kernel.h"
#include "user_syscall.h"
#include "syscall.h"

#define MAX_WRITE 20

uint32_t dummy_write(const char* data_arr, uint32_t len)
{
    if (len > MAX_WRITE)
    {
        return(1);
    }
    char data_str[MAX_WRITE + 1] =   {0};
    md_strlencpy(data_str, data_arr, len + 1);
    screen_put_str(g_screen, data_str);
    return 0;
}

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
            ret = dummy_write((char *)arg0, arg1);
            break;
        case SYS_READ:
            
            break;
        case SYS_ERRPRINT:
            ret = isr_syscall_errprint(arg0);
            break;
    
        default:
            break;
    }
    return (ret);
}