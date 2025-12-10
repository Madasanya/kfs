#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"
#include "user_syscall.h"

USER_TEXT
void nanoshell_wait(void)
{
    user_syshalt();
}