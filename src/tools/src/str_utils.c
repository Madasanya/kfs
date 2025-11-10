#include "str_utils.h"

/** @brief Global cursor position for text output. */
uint8_t cursor = 0;

uint16_t md_strlen(char* str)
{
    uint16_t ret = 0;
    while (*str != '\0')
    {
        ret++;
        str++;
    }
    return(ret);
}

int8_t md_put_char(char c)
{
    volatile char *txt_out = (volatile char*)0xB8000;
    int8_t ret = -1;

    if(cursor < 80)
    {
        txt_out[cursor] = c;
        txt_out[cursor + 1] = 0x20;
        cursor +=2;
        ret = 1;
    }
    return (ret);
}

int16_t md_put_str(char *str)
{
    int16_t ret = 0;
    while (*str != 0)
    {
        if (md_put_char(*str) == -1)
        {
            ret = -1;
            break;
        }
        ret++;
        str++;
    }
    return (ret);
}

void md_ptoa(uint32_t num, char *output)
{
    if (num == 0)
    {
        output[0] = '0';
        output[1] = '\0';
        return;
    }

    int16_t i = 0;
    char temp[8];

    while (num > 0)
    {
        int16_t digit = num % 16;
        temp[i] = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
        i++;
        num /= 16;
    }

    // Reverse the digits into output
    int j = 0;
    while (i > 0)
    {
        output[j++] = temp[--i];
    }
    output[j] = '\0';
    return;
}