#include "keyboard.h"
#include "str_utils.h"
#include "kernel.h"
#include "isr_syscall.h"
#include "printk.h"

#define USER_SEGMENT_BASE 0x200000

uint32_t isr_syscall_read(char* data_arr, uint8_t len)
{
    uint32_t ret = 0;
    
    // Translate user segment offset to kernel linear address
    char* data_linear = (char*)((uint32_t)data_arr + USER_SEGMENT_BASE);

    if (data_linear >= (char *)MIN_USER_ADDR && data_linear < (char *)MAX_USER_ADDR)
    {
        if (len > KEYBOARD_CHAR_ARR_LEN)
        {
            len = KEYBOARD_CHAR_ARR_LEN;
            md_printk(KERN_WARNING "SYSCALL Read: Input buffer truncated to length %u.", len);
        }
        ret = keyboard_chararr_get(&g_keyboard, data_linear, len);
        }
    else
    {
        md_printk(KERN_CRIT "SYSCALL Read: Address %p (linear %p) outside of user data space.", data_arr, data_linear);
    }   
    return (ret);
}