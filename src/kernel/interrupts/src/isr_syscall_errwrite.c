#include "inttype.h"
#include "kernel.h"
#include "errlog.h"
#include "history.h"
#include "start.h"
#include "str_utils.h"
#include "isr_syscall.h"
#include "printk.h"

#define USER_SEGMENT_BASE 0x200000

uint32_t isr_syscall_errwrite(const char *arr, uint32_t len, uint32_t lvl)
{
    // Translate user segment offset to kernel linear address
    const char* arr_linear = (const char*)((uint32_t)arr + USER_SEGMENT_BASE);
    
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
    else if (arr_linear < (char *)MIN_USER_ADDR || arr_linear >= (char *)MAX_USER_ADDR)
    {
        md_printk(KERN_CRIT "SYSCALL ErrWrite: Address %p (linear %p) outside of user data space.", arr, arr_linear);
        return 1;
    }

    errlog_write(&errlog, lvl, arr_linear);
        
    return 0;
}