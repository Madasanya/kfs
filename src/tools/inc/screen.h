#ifndef _SCREEN_H
#define _SCREEN_H

#include "inttype.h"

#define SCREEN_WIDTH 80u
#define SCREEN_HEIGHT 25u
#define VGA_MEMORY  0xB8000 

typedef struct screen_s{
    uint16_t terminal_row;
    uint16_t terminal_column;
    uint16_t terminal_color;
    uint16_t *terminal_buffer;
} screen_t;

void md_screen_init(screen_t *screen);

#endif /* _SCREEN_H */