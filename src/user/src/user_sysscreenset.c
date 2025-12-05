#include "inttype.h"
#include "syscall.h"

extern uint32_t user_syscall(uint32_t num, ...);

uint32_t user_sysscreenset(uint8_t mode, uint8_t num)
{
    user_syscall(SYS_SCREENSET, (uint32_t)mode, (uint32_t)num);
}