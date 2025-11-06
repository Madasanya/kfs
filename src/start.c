#include "multiboot.h"

void start_multiboot1(uint32_t magic, uint32_t mb_info_addr)
{
    if (magic == 0x2BADB002)
    {
        kernel();
    }
    else
    {
        return ;
    }
}