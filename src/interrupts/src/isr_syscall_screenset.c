#include "screen.h"
#include "str_utils.h"
#include "kernel.h"
#include "isr_syscall.h"

uint32_t isr_syscall_screenset(uint8_t mode, uint8_t num)
{
    int8_t tmp_idx;
    uint8_t old_screen_idx;
    uint32_t ret = 0xffffffffu;

    if ((mode <= 2) && (num <= NUM_SCREENS) && (g_active_screen != NULL))
    {
        if (mode == SYS_SCREENSET_INCREASE)
        {
            num = g_current_screen_index + num;
        }
        else if(mode == SYS_SCREENSET_DECREASE)
        {

            tmp_idx = (int8_t)g_current_screen_index - (int8_t)num;
            tmp_idx = (tmp_idx < 0) ? (tmp_idx * -1) : (tmp_idx);
            num = tmp_idx;
        }

        old_screen_idx = g_current_screen_index;
        g_current_screen_index = num % NUM_SCREENS;

        if (old_screen_idx != g_current_screen_index)
        {
            screen_close(g_active_screen);
            tmp_idx = (int8_t)old_screen_idx - (int8_t)g_current_screen_index;
            g_active_screen += tmp_idx;
            screen_open(g_active_screen);
        }

        ret = g_current_screen_index;
    }

    return (ret);
}