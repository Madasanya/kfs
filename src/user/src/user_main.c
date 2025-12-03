#define DEFINE_USER_FUNCTIONS
#include "user.h"

extern unsigned int user_syscall(unsigned int num, unsigned int arg0, unsigned int arg1);


int user_var USER_DATA = -20000;

USER_TEXT
void user_main(void)
{
    user_syswrite("Crazy", 5);


    while (1)
    {
        user_var++;
        if(user_var == 20000)
        {
            user_var = -20000;
        }
    }
}

