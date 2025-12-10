#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"
#include "user_syscall.h"

USER_TEXT
uint8_t nanoshell_kernel_stack_print()
{
    return ((uint8_t)user_stackprint());
}