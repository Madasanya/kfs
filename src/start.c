#include "multiboot.h"

short cursor = 0;

unsigned int md_strlen(char* str)
{
    unsigned int ret = 0;
    while (*str != '\0')
    {
        ret++;
        str++;
    }
    return(ret);
}

short md_put_char(char c)
{
    volatile char *txt_out = (volatile char*)0xB8000;
    short ret = -1;

    if(cursor < 80)
    {
        txt_out[cursor] = c;
        txt_out[cursor + 1] = 0x20;
        cursor +=2;
        ret = 1;
    }
    return (ret);
}

int md_write(char *str)
{
    int ret = 0;
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

void itoa_hex(uint32_t num, char *output)
{
    if (num == 0)
    {
        output[0] = '0';
        output[1] = '\0';
        return;
    }

    int i = 0;
    char temp[8];

    while (num > 0)
    {
        int digit = num % 16;
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


void start_multiboot1(uint32_t magic, uint32_t mb_info_addr)
{
    char str[9];
    md_write("Magic: ");
    itoa_hex(magic, str);
    md_write(str);
    if (magic == 0x2BADB002)
    {
        kernel();
    }
    else
    {
        return ;
    }
}