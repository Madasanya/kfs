#include "inttype.h"
#include "kernel.h"
#include "errlog.h"
#include "history.h"
#include "start.h"
#include "str_utils.h"
#include "isr_syscall.h"
#include "printk.h"

uint32_t isr_syscall_errwrite(const char *arr, uint32_t len, uint32_t lvl)
{
    if (len == 0)
    {
        md_printk(KERN_INFO "SYSCALL ErrWrite: Length 0, Nothing to write.");
        return 1;
    }
    else if (lvl >= ERRLOG_LVL_LEN)
    {
        md_printk(KERN_CRIT "SYSCALL ErrWrite: Unknown lvl %lu.", lvl);
        return 1;
    }
    else if (arr < (char *)MIN_USER_ADDR || arr >= (char *)MAX_USER_ADDR)
    {
        md_printk(KERN_CRIT "SYSCALL ErrWrite: Address %p outside of user data space.", arr);
        return 1;
    }

    errlog_write(&errlog, lvl, arr);
        
    return 0;
}