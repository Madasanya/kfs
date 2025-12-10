#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "user_syscall.h"
#include "inttype.h"
#include "syscall.h"

extern uint32_t user_syscall(uint32_t num);

USER_TEXT
uint32_t user_stackprint(void)
{
    return(user_syscall(SYS_STACKPRINT));
}