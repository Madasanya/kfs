#include "str_utils.h"

/** @brief Global cursor position for text output. */
uint8_t cursor = 0;

uint16_t md_strlen(const char* str)
{
    uint16_t ret = 0;
    while (*str != '\0')
    {
        if (ret == STR_MAX_LEN)
        {
            break;
        }
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

int16_t md_put_str(const char *str)
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

uint16_t md_strlencpy(char *dest, const char *src, uint16_t len)
{
    uint16_t ret = 0;
    while (*src != '\0')
    {
        ret++;
        if (ret == len)
        {
            break;
        }
        *dest = *src;
        src++;
        dest++;
    }
    *dest = '\0';
    return (ret);
}

void uitoa_base(char *output, uint32_t num, const char* base)
{
    char *iter = output;
    uint16_t remainder, divisor = md_strlen(base);
    char tmp;

    do
    {
        remainder = num % divisor;
        *iter = base[remainder];
        iter++;
        num /= divisor;
    }
    while (num != 0u);

    *iter = '\0';
    iter--;
    
    // Reverse string
    while (output < iter)
    {
        tmp = *output;
        *output = *iter;
        *iter = tmp;
        output++;
        iter--;
    }
}

void itoa_base(char *output, int32_t num, const char* base)
{
    if (num < 0)
    {
        output[0] = '-';
        output++;
        num *= -1;
        uitoa_base(output, (uint32_t)num, base);
    }
    else
    {
        uitoa_base(output, (uint32_t)num, base);
    }
}

void ulltoa_base(char *output, uint64_t num, const char* base)
{
    char *iter = output;
    uint16_t remainder, divisor = md_strlen(base);
    char tmp;

    do
    {
        remainder = num % divisor;
        *iter = base[remainder];
        iter++;
        num /= divisor;
    }
    while (num != 0u);

    *iter = '\0';
    iter--;
    
    // Reverse string
    while (output < iter)
    {
        tmp = *output;
        *output = *iter;
        *iter = tmp;
        output++;
        iter--;
    }
}

void lltoa_base(char *output, int64_t num, const char* base)
{
    if (num < 0)
    {
        output[0] = '-';
        output++;
        num *= -1;
        ulltoa_base(output, (uint64_t)num, base);
    }
    else
    {
        ulltoa_base(output, (uint64_t)num, base);
    }
}
