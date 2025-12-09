#include "screen.h"
#include "str_utils.h"
#include "kernel.h"
#include "isr_syscall.h"
#include "printk.h"

#define MAX_SCROLL 32

uint32_t isr_syscall_scroll(uint8_t dir, uint32_t num)
{
    if (dir > 1)
    {
        md_printk(KERN_CRIT "SYSCALL Scroll: Bad input.");
        return (1);
    }
    else
    {
        if (num > MAX_SCROLL)
        {
            num = MAX_SCROLL;
            md_printk(KERN_WARNING "SYSCALL Scroll: Scroll decresed to %lu.", num);
        }
        if (dir == SYS_SCROLL_UP)
        {
            for (uint32_t i = 0; i < num; i++)
            {
                screen_scroll_up(g_active_screen);
            }
        }
        else
        {
            for (uint32_t i = 0; i < num; i++)
            {
                screen_scroll_down(g_active_screen);
            }
        }
    }

    return (0);
}