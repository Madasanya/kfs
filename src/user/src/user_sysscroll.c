#include "inttype.h"
#include "syscall.h"

extern uint32_t user_syscall(uint32_t num, ...);

uint32_t user_sysscroll(uint8_t dir, uint32_t len)
{
    user_syscall(SYS_SCROLL, (uint32_t)dir, len);
}