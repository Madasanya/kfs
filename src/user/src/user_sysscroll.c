#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"
#include "syscall.h"

extern uint32_t user_syscall(uint32_t num, ...);

USER_TEXT
uint32_t user_sysscroll(uint8_t dir, uint32_t len)
{
    user_syscall(SYS_SCROLL, (uint32_t)dir, len);
}