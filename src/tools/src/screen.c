#include "screen.h"

static inline uint8_t md_vga_entry_color(uint16_t fg, uint16_t bg) 
{
	return fg | bg << 4;
}

static inline uint16_t md_vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

void md_screen_init(screen_t *screen)
{
    screen->screen_row = 0;
	screen->screen_column = 0;
	screen->screen_color = 0x30;
    screen->screen_buffer = (uint16_t*)VGA_MEMORY;
	
	for (uint16_t y = 0; y < SCREEN_HEIGHT; y++) {
		for (uint16_t x = 0; x < SCREEN_WIDTH; x++) {
			const uint16_t index = y * SCREEN_WIDTH + x;
			screen->screen_buffer[index] = md_vga_entry(' ', screen->screen_color);
		}
	}
}

void md_screen_setcolor(screen_t *screen, uint8_t color) 
{
	screen->screen_color = color;
}

void md_screen_putentryat(screen_t *screen,  char c, uint8_t color, uint16_t x, uint16_t y) 
{
	const uint16_t index = y * SCREEN_WIDTH + x;
	screen->screen_buffer[index] = md_vga_entry(c, color);
}

void md_screen_putchar(screen_t *screen, char c) 
{
	md_screen_putentryat(screen, c, screen->screen_color, screen->screen_column, screen->screen_row);
	if (++screen->screen_column == SCREEN_WIDTH) {
		screen->screen_column = 0;
		if (++screen->screen_row == SCREEN_HEIGHT)
			screen->screen_row = 0;
	}
}

void md_screen_write(screen_t *screen, const char* data, uint16_t size) 
{
	for (uint16_t i = 0; i < size; i++)
		md_screen_putchar(screen, data[i]);
}

void md_screen_writestring(screen_t *screen, const char* data) 
{
	md_screen_write(screen, data, md_strlen(data));
}