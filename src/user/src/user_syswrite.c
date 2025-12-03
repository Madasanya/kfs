#include "inttype.h"
#include "syscall.h"

extern uint32_t user_syscall(uint32_t num, ...);

uint32_t user_syswrite(char *arr, uint32_t len)
{
    user_syswrite((uint32_t)arr, 5);
}