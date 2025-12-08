#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"
#include "user_syscall.h"

USER_TEXT
uint8_t nanoshell_kernel_stack_print()
{
    uint32_t addr = 0x00400000; // Stack address
    uint32_t len = 999;          // Stack length    
    return (user_syshexdump((void *)addr, len));
}