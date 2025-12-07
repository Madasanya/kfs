#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"
#include "syscall.h"

extern uint32_t user_syscall(uint32_t num, ...);

USER_TEXT
uint32_t user_sysscreenset(uint8_t mode, uint8_t num)
{
    return (user_syscall(SYS_SCREENSET, (uint32_t)mode, (uint32_t)num));
}