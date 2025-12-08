#include "screen.h"
#include "str_utils.h"
#include "kernel.h"
#include "isr_syscall.h"
#include "printk.h"

#define MAX_WRITE 256

uint32_t isr_syscall_write(const char* data_arr, uint32_t len)
{
    uint32_t ret = 0;
    char data_str[MAX_WRITE + 1] = {0};

    if (data_arr >= (char *)MIN_USER_ADDR && data_arr < (char *)MAX_USER_ADDR)
    {
        if (len > MAX_WRITE)
        {
            len = MAX_WRITE;
            md_printk(KERN_WARNING "SYSCALL Write: Input buffer truncated to length %lu.", len);
        }
        if ((len <= MAX_WRITE) && (g_active_screen != NULL))
        {
            ret = md_strlencpy(data_str, data_arr, len + 1);
            screen_put_str(g_active_screen, data_str);
        }
    }
    else
    {
        md_printk(KERN_CRIT "SYSCALL Write: Address %p outside of user data space.", data_arr);
        ret = 1;
    }

    return (ret);
}