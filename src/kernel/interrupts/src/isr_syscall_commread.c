#include "keyboard.h"
#include "str_utils.h"
#include "kernel.h"
#include "isr_syscall.h"
#include "printk.h"

uint32_t isr_syscall_commread(uint8_t* data_arr, uint8_t len)
{
    uint32_t ret = 0;

    if (data_arr >= (uint8_t *)MIN_USER_ADDR && data_arr < (uint8_t *)MAX_USER_ADDR)
    {
        if (len > KEYBOARD_COMM_ARR_LEN)
        {
            len = KEYBOARD_COMM_ARR_LEN;
            md_printk(KERN_WARNING "SYSCALL CommRead: Input buffer truncated to length %u.", len);
        }
        ret = keyboard_commarr_get(&g_keyboard, data_arr, len);
    }
    else
    {
        md_printk(KERN_CRIT "SYSCALL CommRead: Address %p outside of user data space.", data_arr);
    }
    return (ret);
}