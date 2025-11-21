#include "history.h"
#include "screen.h"
#include "kernel.h"
#include "colors.h"
#include "str_utils.h"
#include "screen_settings.h"
#include "cursor.h"

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
		screen->screen_column -= (SCREEN_WIDTH - screen->start_screen_column);
		screen_save_row_to_history(screen, screen->screen_row);
		screen->screen_row++; 
		if (screen->screen_row >= SCREEN_HEIGHT)
		{
			screen->screen_row = SCREEN_HEIGHT - 1;
			screen_print_history(screen, SCREEN_HEIGHT - 1 - screen->start_screen_row, 0);
		}
	}
	cursor_update(screen->screen_column, screen->screen_row);
}

static void screen_newline(screen_t *screen)
{
	screen_save_row_to_history(screen, screen->screen_row);
	
	screen->screen_row++;
	screen->screen_column = screen->start_screen_column;
	
	if (screen->screen_row >= SCREEN_HEIGHT)
	{
		screen->screen_row = SCREEN_HEIGHT - 1;
		screen_print_history(screen, SCREEN_HEIGHT- 1 - screen->start_screen_row, 0);
	}
	cursor_update(screen->screen_column, screen->screen_row);

}

void screen_put_char(screen_t *screen, char c)
{
	if (screen->history_offset != 0u)
	{
		return;
	}
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
	
	if (screen->history_offset != 0u)
	{
		return;
	}
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
			screen_advance_cursor(screen, chars_written);
			chars_written = 0;
			position = screen->screen_row * SCREEN_WIDTH + screen->screen_column;
		}
		
		screen_put_char_at_position(screen, str[i], color, position);
		chars_written++;
	}
	
	screen_advance_cursor(screen, chars_written);
}

void screen_init(screen_t *screen, history_buffer_t *history_buffer, uint8_t default_color, char *header_str)
{ 
	screen->screen_buffer = (uint16_t*)VGA_MEMORY;
    screen->screen_row = 1;
	screen->screen_column = 0;
	screen->start_screen_column = 0;
	screen->start_screen_row = 1;
	screen->screen_color_default = default_color;
	screen->screen_color_current = screen->screen_color_default;
	screen->history_buffer = history_buffer;
	(void)md_strlencpy(screen->screen_header, header_str, SCREEN_WIDTH);
	screen->history_offset = 0;
	history_init(history_buffer);
}

static void print_header(screen_t *screen)
{
	uint16_t len = md_strlen(screen->screen_header);
	uint16_t padding = (SCREEN_WIDTH - len) / 2;

	for (uint16_t x = 0; x < len; x++)
	{
		screen_put_char_at_position(screen, screen->screen_header[x], screen->screen_color_default, padding + x);
	}
}

void screen_open(screen_t *screen)
{
	screen_clear(screen, 0, screen->screen_color_default);
	print_header(screen);
	screen_print_history(&screen, SCREEN_HEIGHT - screen->start_screen_row, screen->history_offset);
	cursor_update(screen->screen_column, screen->screen_row);
	if (screen->history_offset == 0u)
	{
		history_last_entry_remove(screen->history_buffer);
		cursor_enable(14, 15);
	}
}

void screen_close(screen_t *screen)
{
	if (screen->history_offset == 0u)
	{
		screen_save_row_to_history(screen, screen->screen_row);
	}
	cursor_disable();
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

void screen_print_history(screen_t *screen, uint16_t number_of_lines, uint32_t history_offset)
{
    uint32_t pos_helper = history_get_last_entry_index(screen->history_buffer);
	const uint32_t pos_end = history_get_first_entry_index(screen->history_buffer);
	uint16_t row_cnt;
	uint32_t his_size = history_get_num_of_entrys(screen->history_buffer);

	if (number_of_lines > his_size)
	{
		number_of_lines = his_size;
		history_offset = 0;
	}
	else if (((uint32_t)number_of_lines + history_offset) > his_size)
	{
		history_offset = his_size - (uint32_t)number_of_lines;
	}

	if (history_offset > pos_helper)
	{
		pos_helper = HISTORY_HEIGHT - (history_offset - pos_helper);
	}
	else
	{
		pos_helper -= history_offset;
	}


	number_of_lines += screen->start_screen_row; //Adding header offset
	number_of_lines--;
	if (number_of_lines >= SCREEN_HEIGHT)
	{
		number_of_lines = SCREEN_HEIGHT - 1;
	}
	
	for (row_cnt = number_of_lines; row_cnt >= screen->start_screen_row; row_cnt--)
	{
		uint16_t *entry = history_get_entry(screen->history_buffer, pos_helper);
		for (uint16_t x = screen->start_screen_column; x < SCREEN_WIDTH; x++)
		{
			screen->screen_buffer[row_cnt * SCREEN_WIDTH + x] = entry[x - screen->start_screen_column];
		}
		if (pos_helper == pos_end)
		{
			break;
		}
		if (pos_helper == 0)
		{
			pos_helper = HISTORY_HEIGHT;
		}
		pos_helper--;
	}
	if ((number_of_lines + 1u) < SCREEN_HEIGHT)
	{
		screen_clear(screen, (number_of_lines + 1) * SCREEN_WIDTH, screen->screen_color_default);
	}
}

void screen_save_row_to_history(screen_t *screen, uint16_t row)
{
    if (row >= SCREEN_HEIGHT)
        return;
    
    uint16_t *row_buffer = &screen->screen_buffer[row * SCREEN_WIDTH];
    history_add_entry(screen->history_buffer, row_buffer);
}

static uint32_t screen_max_histoty_offset_get(screen_t *screen)
{
	uint32_t ret = 0u;
	uint32_t his_size = history_get_num_of_entrys(screen->history_buffer);
	uint16_t writtable = SCREEN_HEIGHT - screen->start_screen_row ;

	if (his_size > writtable)
	{
		ret = his_size - (uint32_t)writtable;
	}
	
	return ret;
}

void screen_scroll_up(screen_t *screen)
{
	uint32_t max_his_off = screen_max_histoty_offset_get(screen);

	if (max_his_off > screen->history_offset)
	{
		if (screen->history_offset == 0u)
		{
			screen_save_row_to_history(screen, screen->screen_row);
			cursor_disable(); 
		} 
		screen->history_offset++;
		screen_print_history(screen, SCREEN_HEIGHT - screen->start_screen_row, screen->history_offset);
	}
}

void screen_scroll_down(screen_t *screen)
{
	if (screen->history_offset > 0)
	{
		screen->history_offset--;
		screen_print_history(screen, SCREEN_HEIGHT - screen->start_screen_row, screen->history_offset);
		if (screen->history_offset == 0u)
		{
			history_last_entry_remove(screen->history_buffer);
			cursor_enable(14, 15); 
		}
		
	}
}