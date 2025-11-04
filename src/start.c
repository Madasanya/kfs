#include "multiboot.h"

#define MULTIBOOT1_MAGICNUM 0x1BADB002UL
#define MULTIBOOT1_FLAGS    0x0UL
#define MULTIBOOT1_CHKSUM (-(MULTIBOOT1_MAGICNUM + MULTIBOOT1_FLAGS))

// Multiboot header — placed in .multiboot section
const multiboot1_header_t mb1_header
    __attribute__((section(".multiboot1"), used)) = {
        .magic    = MULTIBOOT1_MAGICNUM,
        .flags    = MULTIBOOT1_FLAGS,
        .checksum = MULTIBOOT1_CHKSUM
};

void _start_multiboot1(uint32_t magic, uint32_t mb_info_addr)
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