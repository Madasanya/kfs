#include "multiboot.h"
#include "inttype.h"
#include "start.h"

kernel_ver_t version __attribute__((section(".kernel_info"))) = \
{
    .major_ver = MAJOR_VER,
    .minor_ver = MINOR_VER
};

errlog_t errlog;

void start_multiboot1(uint32_t magic)
{
    errlog_init(&errlog, ERRLOG_LVL_DEFAULT);
    if (magic == 0x2BADB002)
    {
        kernel();
    }
    else
    {
        return ;
    }
}