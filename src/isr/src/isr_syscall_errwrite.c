#include "inttype.h"
#include "kernel.h"
#include "errlog.h"
#include "history.h"
#include "start.h"
#include "str_utils.h"

uint32_t isr_syscall_errwrite(char *arr, uint32_t len, uint32_t lvl)
{
    if (arr == NULL || len == 0 || lvl >= ERRLOG_LVL_LEN || lvl < 0)
    {
        return 1;
    }
    errlog_write(&errlog, lvl, arr);
        
    return 0;
}