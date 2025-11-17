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

static void screen_advance_cursor(screen_t *screen, uint16_t count)
{
	screen->screen_column += count;
	while (screen->screen_column >= SCREEN_WIDTH)
	{
		screen->screen_column -= SCREEN_WIDTH;
		screen->screen_row++;
		if (screen->screen_row >= SCREEN_HEIGHT)
		{
			screen_save_row_to_history(screen, SCREEN_HEIGHT - 1);
			screen->screen_row = SCREEN_HEIGHT - 1;
			screen_print_history(screen, SCREEN_HEIGHT - 1);
			screen_clear(screen, screen->screen_row * SCREEN_WIDTH, screen->screen_color_default);
		}
	}
}

static void screen_newline(screen_t *screen)
{
	screen_save_row_to_history(screen, screen->screen_row);
	
	screen->screen_row++;
	screen->screen_column = 0;
	
	if (screen->screen_row >= SCREEN_HEIGHT)
	{
		screen->screen_row = SCREEN_HEIGHT - 1;
		screen_print_history(screen, SCREEN_HEIGHT - 1);
		screen_clear(screen, screen->screen_row * SCREEN_WIDTH, screen->screen_color_default);
	}
}

void screen_put_char(screen_t *screen, char c)
{
	if (c == '\n')
	{
		screen_newline(screen);
		return;
	}
	
	uint16_t position = screen->screen_row * SCREEN_WIDTH + screen->screen_column;
	screen_put_char_at_position(screen, c, screen->screen_color_current, position);
	screen_advance_cursor(screen, 1);
}

void screen_put_str(screen_t *screen, const char *str)
{
	uint16_t size = md_strlen(str);
	uint8_t color = screen->screen_color_current;
	uint16_t chars_written = 0;
	
	for (uint16_t i = 0; i < size; i++)
	{
		if (str[i] == '\n')
		{
			screen_newline(screen);
			screen_clear(screen, screen->screen_row * SCREEN_WIDTH + screen->screen_column, screen->screen_color_default);
			chars_written = 0;
			continue;
		}
		
		uint16_t position = screen->screen_row * SCREEN_WIDTH + screen->screen_column + chars_written;
		
		if (screen->screen_column + chars_written >= SCREEN_WIDTH)
		{
			screen_newline(screen);
			screen_clear(screen, screen->screen_row * SCREEN_WIDTH + screen->screen_column, screen->screen_color_default);
			chars_written = 0;
			position = screen->screen_row * SCREEN_WIDTH + screen->screen_column;
		}
		
		screen_put_char_at_position(screen, str[i], color, position);
		chars_written++;
	}
	
	screen_advance_cursor(screen, chars_written);
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
	
	screen_clear(screen, 0, screen->screen_color_default);
}

void screen_clear(screen_t *screen, uint16_t start_index, uint8_t color)
{
	uint16_t colored_space = screen_put_colored_char(' ', color);
	uint16_t total_size = SCREEN_WIDTH * SCREEN_HEIGHT;
	
	for (uint16_t i = start_index; i < total_size; i++)
	{
		screen->screen_buffer[i] = colored_space;
	}
}

void screen_print_history(screen_t *screen, uint16_t number_of_lines)
{
    int16_t pos_helper = history_get_last_command_index(screen->history_buffer) - (number_of_lines - 1);
    uint16_t row = 0;

    if (pos_helper < 0)
    {
        for (uint32_t i = HISTORY_HEIGHT + pos_helper; i <= HISTORY_HEIGHT - 1; i++)
        {
            uint16_t *entry = history_get_entry(screen->history_buffer, i);
            for (uint16_t x = 0; x < SCREEN_WIDTH; x++)
            {
                screen->screen_buffer[row * SCREEN_WIDTH + x] = entry[x];
            }
            row++;
        }
        for (uint32_t i = 0; i <= history_get_last_command_index(screen->history_buffer); i++)
        {
            uint16_t *entry = history_get_entry(screen->history_buffer, i);
            for (uint16_t x = 0; x < SCREEN_WIDTH; x++)
            {
                screen->screen_buffer[row * SCREEN_WIDTH + x] = entry[x];
            }
            row++;
        }
    }
    else
    {
        for (uint32_t i = pos_helper; i <= history_get_last_command_index(screen->history_buffer); i++)
        {
            uint16_t *entry = history_get_entry(screen->history_buffer, i);
            for (uint16_t x = 0; x < SCREEN_WIDTH; x++)
            {
                screen->screen_buffer[row * SCREEN_WIDTH + x] = entry[x];
            }
            row++;
        }
    }
}

void screen_save_row_to_history(screen_t *screen, uint16_t row)
{
    if (row >= SCREEN_HEIGHT)
        return;
    
    uint16_t *row_buffer = &screen->screen_buffer[row * SCREEN_WIDTH];
    history_add_entry(screen->history_buffer, row_buffer);
}