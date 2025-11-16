#include "str_utils.h"
#include "history.h"
#include "screen.h"

void kernel(void)
{
    screen_t screen;
    history_buffer_t history_buffer;
    
    md_screen_init(&screen);
    md_history_init(&history_buffer);
    md_put_str("42");
}