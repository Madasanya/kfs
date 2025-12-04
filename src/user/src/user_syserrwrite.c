#include "user_syscall.h"
#include "isr_syscall.h"
#include "inttype.h"
#include "syscall.h"

extern uint32_t user_syscall(uint32_t num, uint32_t arg0);

uint32_t user_syserrwrite(char *arr, uint32_t len, uint32_t lvl)
{
    return (user_syscall(SYS_ERRWRITE, lvl));
}