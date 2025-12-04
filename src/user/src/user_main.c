#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "errlog.h"
#include "user_syscall.h"

int user_var USER_DATA = -20000;

USER_TEXT
void user_main(void)
{
    user_syserrprint(ERRLOG_LVL_DEBUG);
    while (1)
    {
        user_var++;
        if(user_var == 20000)
        {
            user_var = -20000;
        }
    }
}

