#include "screen.h"
#include "str_utils.h"
#include "kernel.h"
#include "isr_syscall.h"
#include "printk.h"

#define MAX_WRITE 256
#define USER_SEGMENT_BASE 0x200000

uint32_t isr_syscall_write(const char* data_arr, uint32_t len)
{
    uint32_t ret = 0;
    char data_str[MAX_WRITE + 1] = {0};
    
    // Translate user segment offset to kernel linear address
    const char* data_linear = (const char*)((uint32_t)data_arr + USER_SEGMENT_BASE);

    if (data_linear >= (char *)MIN_USER_ADDR && data_linear < (char *)MAX_USER_ADDR)
    {
        if (len > MAX_WRITE)
        {
            len = MAX_WRITE;
            md_printk(KERN_WARNING "SYSCALL Write: Input buffer truncated to length %lu.", len);
        }
        if ((len <= MAX_WRITE) && (g_active_screen != NULL))
        {
            ret = md_strlencpy(data_str, data_linear, len + 1);
            screen_put_str(g_active_screen, data_str);
        }
    }
    else
    {
        md_printk(KERN_CRIT "SYSCALL Write: Address %p (linear %p) outside of user data space.", data_arr, data_linear);
        screen_put_str(g_active_screen, "SYSCALL Write: Invalid memory access.\n");
        ret = 1;
    }

    return (ret);
}