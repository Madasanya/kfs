#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"
#include "user_syscall.h"
#include "user_str_utils.h"

#define ECHO_LIMIT 255u

USER_TEXT
uint8_t nanoshell_echo(char* str)
{
    uint16_t len = user_strlen(str);
    if ((len == 0) || (str == NULL))
    {
        return 1; // Nothing to write
    }  
    if (len > ECHO_LIMIT)
    {
        len = ECHO_LIMIT; // Truncate to limit
    }   
    user_syswrite(str, len); // Skip "echo "
    return 0; // Return success
}