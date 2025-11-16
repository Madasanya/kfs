#include "str_utils.h"
#include "screen.h"
#include "history.h"
#include "colors.h"

void kernel(void)
{
    screen_t screen;
    history_buffer_t history_buffer;
    
    screen_init(&screen, &history_buffer, SCREEN_COLOR_PROFILES[4]);
    
    screen_put_char(&screen, '4');
    screen_put_char(&screen, '2');
    screen_set_color(&screen, SCREEN_COLOR_PROFILES[3]);
    screen_put_str(&screen, " Welcome to KFS!\n");
    
}