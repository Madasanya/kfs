#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "user_syscall.h"
#include "inttype.h"
#include "syscall.h"

extern uint32_t user_syscall(uint32_t num);

USER_TEXT
void user_syshalt(void)
{
    (void)user_syscall(SYS_HALT);
}