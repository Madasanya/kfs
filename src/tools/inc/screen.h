#ifndef _SCREEN_H
#define _SCREEN_H

#include "inttype.h"

#define SCREEN_WIDTH 80u
#define SCREEN_HEIGHT 25u
#define VGA_MEMORY  0xB8000 

typedef struct screen_s{
    uint16_t screen_row;
    uint16_t screen_column;
    uint16_t screen_color;
    uint16_t *screen_buffer;
} screen_t;

void md_screen_init(screen_t *screen);
void md_screen_setcolor(screen_t *screen, uint8_t color) ;
void md_screen_putentryat(screen_t *screen,  char c, uint8_t color, uint16_t x, uint16_t y) ;
void md_screen_putchar(screen_t *screen, char c) ;
void md_screen_write(screen_t *screen, const char* data, uint16_t size) ;
void md_screen_writestring(screen_t *screen, const char* data) ;

#endif /* _SCREEN_H */