#include "str_utils.h"
#include "history.h"

void kernel(void)
{
    history_buffer_t history_buffer;
    
    md_history_init(&history_buffer);
    md_put_str("42");
}