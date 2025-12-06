#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"

USER_TEXT
uint8_t gnl(char * buffer)
{
    if (buffer == NULL)
    {
        return 0;
    }

    uint8_t idx = 0;
    char c = 0;

    while (1)
    {
        c = user_sysread();
        if (c == '\n' || idx >= 80)
        {
            break;
        }
        buffer[idx++] = c;
    }
    buffer[idx] = '\0';

    return idx;
}