#include "inttype.h"
#include "printk.h"


uint32_t syscall_printerr(uint32_t lvl)
{
    md_printk(lvl);
    return 0;
}