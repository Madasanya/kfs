#include "history.h"
#include "screen.h"
#include "kernel.h"
#include "colors.h"
#include "str_utils.h"

static inline uint16_t screen_put_colored_char(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

static inline void screen_put_char_at_position(screen_t *screen, char c, uint8_t color, uint16_t position)
{
	screen->screen_buffer[position] = screen_put_colored_char(c, color);
}

void screen_set_color(screen_t *screen, uint8_t color)
{
	screen->screen_color_current = color;
}

void screen_put_char(screen_t *screen, char c)
{
	uint16_t position = screen->screen_row * SCREEN_WIDTH + screen->screen_column;
	screen_put_char_at_position(screen, c, screen->screen_color_current, position);
	
	position++;
	if (position >= SCREEN_WIDTH * SCREEN_HEIGHT)
		position = 0;
	
	screen->screen_row = position / SCREEN_WIDTH;
	screen->screen_column = position % SCREEN_WIDTH;
}

void screen_put_str(screen_t *screen, const char *str)
{
	uint16_t size = md_strlen(str);
	uint16_t position = screen->screen_row * SCREEN_WIDTH + screen->screen_column;
	uint8_t color = screen->screen_color_current;
	
	for (uint16_t i = 0; i < size; i++) {
		screen_put_char_at_position(screen, str[i], color, position);
		position++;
		
		if (position >= SCREEN_WIDTH * SCREEN_HEIGHT)
			position = 0;
	}
	
	screen->screen_row = position / SCREEN_WIDTH;
	screen->screen_column = position % SCREEN_WIDTH;
}

void screen_init(screen_t *screen, history_buffer_t *history_buffer, uint8_t default_color)
{ 
	screen->screen_buffer = (uint16_t*)VGA_MEMORY;
    screen->screen_row = 0;
	screen->screen_column = 0;
	screen->screen_header = "42";
	screen->screen_color_default = default_color;
	screen->screen_color_current = screen->screen_color_default;
	screen->history_buffer = history_buffer;
	history_init(history_buffer);
	
	for (uint16_t y = 0; y < SCREEN_HEIGHT; y++) {
		for (uint16_t x = 0; x < SCREEN_WIDTH; x++) {
			const uint16_t index = y * SCREEN_WIDTH + x;
			screen->screen_buffer[index] = screen_put_colored_char(' ', screen->screen_color_default);
		}
	}
}

void screen_print_history(screen_t *screen)
{
    int16_t pos_helper = history_get_last_command_index(screen->history_buffer) - (SCREEN_HEIGHT - 1);

    if (pos_helper < 0)
    {
        for (uint32_t i = HISTORY_HEIGHT + pos_helper; i <= HISTORY_HEIGHT - 1; i++)
        {
            screen_put_str(screen, history_get_entry(screen->history_buffer, i));
        }
        for (uint32_t i = 0; i <= history_get_last_command_index(screen->history_buffer); i++)
        {
            screen_put_str(screen, history_get_entry(screen->history_buffer, i));
        }
    }
    else
    {
        for (uint32_t i = pos_helper; i <= history_get_last_command_index(screen->history_buffer); i++)
        {
            screen_put_str(screen, history_get_entry(screen->history_buffer, i));
        }
    }
}