#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"
#include "user_syscall.h"
#include "user_str_utils.h"

#define ECHO_LIMIT 64u

USER_TEXT
uint8_t nanoshell_echo(char* str)
{
    uint16_t len = user_strlen(str);
    if ((len == 0) || (str == NULL))
    {
        return 1; // Nothing to write
    }
    if (str[0] == '\n')
    {
        return 1; // Nothing to write
    }
    if (len >= ECHO_LIMIT)
    {
        len = ECHO_LIMIT + 1; // Truncate to limit
        str[len - 1] = '\n';
    }   
    user_syswrite(str, len);
    return 0;
}