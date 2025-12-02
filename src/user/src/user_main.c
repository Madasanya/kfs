#define DEFINE_USER_FUNCTIONS
#include "user.h"

int user_var USER_DATA = -20000;

USER_TEXT
void user_main(void)
{
    asm volatile("int $0x80");

    while (1)
    {
        user_var++;
        if(user_var == 20000)
        {
            user_var = -20000;
        }
    }
}

