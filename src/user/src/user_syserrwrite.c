#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "user_syscall.h"
#include "inttype.h"
#include "syscall.h"

extern uint32_t user_syscall(uint32_t num, char *arg0, uint32_t arg1, uint32_t arg2);

USER_TEXT
uint32_t user_syserrwrite(char *arr, uint32_t len, uint32_t lvl)
{
    return (user_syscall(SYS_ERRWRITE, arr, len, lvl));
}