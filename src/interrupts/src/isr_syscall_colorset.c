#include "screen.h"
#include "str_utils.h"
#include "kernel.h"
#include "colors.h"
#include "isr_syscall.h"

uint32_t isr_syscall_colorset(uint8_t mode, uint8_t num)
{
    int8_t tmp_idx;
    uint32_t ret = 0xffffffffu;

    if ((mode <= 2) && (num <= NUM_SCREEN_COLOR_PROFILES) && (g_active_screen != NULL))
    {
        if (mode == SYS_COLORSET_MODE_INCREASE)
        {
            num = g_current_screen_index + num;
        }
        else if(mode == SYS_COLORSET_MODE_DECREASE)
        {
            tmp_idx = (int8_t)g_current_screen_index - (int8_t)num;
            tmp_idx = (tmp_idx < 0) ? (tmp_idx * -1) : (tmp_idx);
            num = tmp_idx;
        }
        num %= NUM_SCREEN_COLOR_PROFILES;
        g_current_color_index[g_current_screen_index] = num;
        screen_set_color(g_active_screen, SCREEN_COLOR_PROFILES[ g_current_color_index[g_current_screen_index]]);

        ret =  g_current_color_index[g_current_screen_index];
    }

    return (ret);
}