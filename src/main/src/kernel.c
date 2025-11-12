#include "str_utils.h"
#include "history.h"

void kernel(void)
{
    md_history_init();
    md_put_str("42");
}