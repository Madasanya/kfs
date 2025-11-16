#include "screen.h"

static inline uint8_t vga_entry_color(uint16_t fg, uint16_t bg) 
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

void md_screen_init(screen_t *screen)
{
    screen->terminal_row = 0;
	screen->terminal_column = 0;
	screen->terminal_color = 0x30;
    screen->terminal_buffer = (uint16_t*)VGA_MEMORY;
	
	for (uint16_t y = 0; y < SCREEN_HEIGHT; y++) {
		for (uint16_t x = 0; x < SCREEN_WIDTH; x++) {
			const uint16_t index = y * SCREEN_WIDTH + x;
			screen->terminal_buffer[index] = vga_entry(' ', screen->terminal_color);
		}
	}
}
