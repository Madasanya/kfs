#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"
#include "syscall.h"

extern uint32_t user_syscall(uint32_t num, ...);

USER_TEXT
uint32_t user_sysread(char *arr, uint8_t len)
{
    return (user_syscall(SYS_READ, (uint32_t)arr, (uint32_t)len));
}