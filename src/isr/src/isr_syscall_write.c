#include "screen.h"
#include "str_utils.h"
#include "kernel.h"

#define MAX_WRITE 256

uint32_t isr_syscall_write(const char* data_arr, uint32_t len)
{
    uint32_t ret = 0;
    char data_str[MAX_WRITE + 1] = {0};

    if ((len <= MAX_WRITE) && (g_active_screen != NULL))
    {
        ret = md_strlencpy(data_str, data_arr, len + 1);
        screen_put_str(g_active_screen, data_str);
    }

    return (ret);
}