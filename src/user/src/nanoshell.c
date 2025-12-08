#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "nanoshell_builtins.h"
#include "user_syscall.h"
#include "user_str_utils.h"

#define SHELL_NAME "nanoshell> "

USER_TEXT
void nanoshell_run()
{
    char line_buff[81] = {0};
    uint8_t comm = 0;

    while (1)
    {
        if (nanoshell_gnl(line_buff, 81) != 0u)
        {
            user_syswrite(line_buff, user_strlen(line_buff));
        }

        if (nanoshell_gnc(&comm))
        {
            if (comm == 0x01u)
            {
                user_syswrite("scroll up", user_strlen("scroll up"));
            }
            else if (comm == 0x02u)
            {
                user_syswrite("scroll down", user_strlen("scroll down"));
            }
            else
            {
                user_syswrite("yeah right", user_strlen("yeah right"));
            }
        }
    }
}
