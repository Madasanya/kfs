#include "history.h"
#include "screen.h"
#include "kernel.h"
#include "colors.h"

static inline uint8_t vga_entry_color(uint16_t fg, uint16_t bg) 
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

void md_screen_init(screen_t *screen, history_buffer_t *history_buffer)
{ 
	screen->screen_buffer = (uint16_t*)VGA_MEMORY;
    screen->screen_row = 0;
	screen->screen_column = 0;
	screen->screen_header = "42";
	screen->screen_color_default = 0x30;
	screen->screen_color_current = screen->screen_color_default;
	screen->history_buffer = history_buffer;
	md_history_init(history_buffer);
	
	for (uint16_t y = 0; y < SCREEN_HEIGHT; y++) {
		for (uint16_t x = 0; x < SCREEN_WIDTH; x++) {
			const uint16_t index = y * SCREEN_WIDTH + x;
			screen->screen_buffer[index] = vga_entry(' ', screen->screen_color_default);
		}
	}
}

void md_screen_setcolor(screen_t *screen, uint8_t color) 
{
	screen->screen_color_current = color;
}

void md_screen_putentryat(screen_t *screen,  char c, uint8_t color, uint16_t x, uint16_t y) 
{
	const uint16_t index = y * SCREEN_WIDTH + x;
	screen->screen_buffer[index] = vga_entry(c, color);
}

void md_screen_putchar(screen_t *screen, char c) 
{
	md_screen_putentryat(screen, c, screen->screen_color_current, screen->screen_column, screen->screen_row);
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