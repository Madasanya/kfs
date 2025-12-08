#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"
#include "user_syscall.h"
#include "user_str_utils.h"

USER_TEXT
uint8_t nanoshell_scroll(uint8_t dir)
{
    user_sysscroll(dir, 1);
    return 0;
}