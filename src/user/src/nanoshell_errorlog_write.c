#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"
#include "user_syscall.h"
#include "user_str_utils.h"
#include "errlog.h"

#define ERRLOG_LIMIT 60u

USER_TEXT
uint8_t nanoshell_errlog_write(char* str)
{
    uint16_t len = user_strlen(str);
    if ((len == 0) || (str == NULL))
    {
        return 1; // Nothing to write
    }  
    if (len > ERRLOG_LIMIT)
    {
        len = ERRLOG_LIMIT; // Truncate to limit
    }   
    user_syserrwrite(str, len, (uint32_t)ERRLOG_LVL_INFO);
    return 0; // Return success
}