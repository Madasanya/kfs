#include "str_utils.h"
#include "screen.h"
#include "history.h"

void kernel(void)
{
    screen_t screen;
    history_buffer_t history_buffer;
    
    md_screen_init(&screen, &history_buffer);
    md_screen_writestring(&screen, "Welcome to KFS!\n");
    md_put_str("42");
}