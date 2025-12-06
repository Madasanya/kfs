#include "keyboard.h"
#include "str_utils.h"
#include "kernel.h"

#define MAX_READ 128

uint32_t isr_syscall_read(char* data_arr, uint8_t len)
{
    uint32_t ret = 0;
    if (len > MAX_READ)
    {
        len = MAX_READ;
    }

    ret = keyboard_chararr_get(&g_keyboard, data_arr, len);
    return (ret);
}