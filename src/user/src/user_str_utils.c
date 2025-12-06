#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"

#define STR_MAX_LEN 65535u

USER_TEXT
uint16_t user_strlen(const char* str)
{
    uint16_t ret = 0;
    while (*str != '\0')
    {
        if (ret == STR_MAX_LEN)
        {
            break;
        }
        ret++;
        str++;
    }
    return(ret);
}