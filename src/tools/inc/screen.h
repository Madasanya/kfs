#ifndef _SCREEN_H
#define _SCREEN_H

#include "inttype.h"
#include "str_utils.h"

#define VGA_MEMORY  0xB8000

/* Forward declaration */
typedef struct history_buffer_s history_buffer_t; 

typedef struct screen_s{
    uint16_t *screen_buffer;
    uint16_t screen_row;
    uint16_t screen_column;
    char *screen_header;
    uint16_t screen_color_default;
    uint16_t screen_color_current;
    history_buffer_t *history_buffer;

} screen_t;

void md_screen_init(screen_t *screen, history_buffer_t *history_buffer);
void md_screen_setcolor(screen_t *screen, uint8_t color) ;
void md_screen_putentryat(screen_t *screen,  char c, uint8_t color, uint16_t x, uint16_t y) ;
void md_screen_putchar(screen_t *screen, char c) ;
void md_screen_write(screen_t *screen, const char* data, uint16_t size) ;
void md_screen_writestring(screen_t *screen, const char* data) ;

#endif /* _SCREEN_H */