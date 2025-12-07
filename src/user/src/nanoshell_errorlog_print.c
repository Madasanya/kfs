#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"
#include "user_syscall.h"
#include "user_str_utils.h"
#include "errlog.h"

USER_TEXT
uint8_t nanoshell_errlog_print(char *lvl_str)
{
    if (lvl_str == NULL)
    {
        return 1; // Invalid argument
    }

    uint32_t lvl = 0;
    if (user_strlen(lvl_str) == 0u)
    {
        return 1; // Invalid argument
    }

    // Convert string to integer level
    for (uint16_t i = 0; lvl_str[i] != '\0'; i++)
    {
        if ((lvl_str[i] < '0') || (lvl_str[i] > '9'))
        {
            return 1; // Invalid character
        }
        lvl = (lvl * 10u) + (uint32_t)(lvl_str[i] - '0');
    }

    if (lvl > (uint32_t)ERRLOG_LVL_DEBUG)
    {
        return 1; // Invalid level
    }
    user_syserrprint(lvl);
    return 0; // Return success
}