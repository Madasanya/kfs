#include "hexdump.h"

uint32_t isr_syscall_hexdump(const void* start_addr, uint32_t len)
{
    return (md_hexdump_print(start_addr, len));
}
 