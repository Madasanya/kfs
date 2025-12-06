#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "user_syscall.h"
#include "isr_syscall.h"
#include "inttype.h"
#include "syscall.h"

extern uint32_t user_syscall(uint32_t num, uint32_t arg0);

USER_TEXT
uint32_t user_syserrprint(uint32_t lvl)
{
    return (user_syscall(SYS_ERRPRINT, lvl));
}