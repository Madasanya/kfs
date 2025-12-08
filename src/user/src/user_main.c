#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "nanoshell.h"
#include "kernel.h"
#include "printk.h"


USER_TEXT
void user_main(void)
{
    uint32_t testis = g_super_important_global_var;
    md_printk("I dont want to be 42: %lu I am on address %p, my vlue is %lu", testis, &g_super_important_global_var, g_super_important_global_var);
    nanoshell_run();
}

