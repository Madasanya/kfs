#include "user_syscall.h"
#include "isr_syscall.h"
#include "inttype.h"

extern uint32_t user_syscall(uint32_t num, ...);

uint32_t user_sysprinterr(uint32_t lvl)
{
    return (user_syscall(SYS_PRINTERR, lvl));
}