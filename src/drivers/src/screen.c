#include "history.h"
#include "screen.h"
#include "kernel.h"
#include "colors.h"
#include "str_utils.h"
#include "screen_settings.h"
#include "cursor.h"
#include "printk.h"

/**
 * @brief Cursor style used during normal input vs history viewing.
 *
 * SCREEN_CURSOR_SETTING_WRITE  → thin blinking underscore (active typing)
 * SCREEN_CURSOR_SETTING_READ   → full block (indicates viewing history)
 */
#define SCREEN_CURSOR_SETTING_WRITE CURSOR_SETTING_THIN
#define SCREEN_CURSOR_SETTING_READ  CURSOR_SETTING_FULL

/**
 * @brief Combines an ASCII character and color attribute into a single 16-bit VGA entry.
 *
 * Low byte  = character, high byte = color/attribute (as used by VGA text mode).
 *
 * @param uc    ASCII character to display
 * @param color Foreground/background color byte (e.g. COLOR_WHITE | BG_BLACK)
 * @return      16-bit value ready to be written to VGA memory
 */
static inline uint16_t screen_put_colored_char(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

/**
 * @brief Writes a colored character directly to a specific position in the screen buffer.
 *
 * Bypasses cursor logic — useful for headers, history rendering, and clearing.
 *
 * @param screen   Pointer to screen state structure
 * @param c        Character to write
 * @param color    Color attribute
 * @param position Linear index in VGA buffer (0 to SCREEN_WIDTH*SCREEN_HEIGHT-1)
 */
static inline void screen_put_char_at_position(screen_t *screen, char c, uint8_t color, uint16_t position)
{
	screen->screen_buffer[position] = screen_put_colored_char(c, color);
}

void screen_set_color(screen_t *screen, uint8_t color)
{
	md_printk(KERN_NOTICE "Color change screen with header: %s to color %u\n", screen->screen_header, color);
	screen->screen_color_current = color;
}

/**
 * @brief Advances the logical cursor by a number of columns, handling wrap and scroll.
 *
 * Automatically saves the current row to history when wrapping and scrolls screen if needed.
 *
 * @param screen Pointer to screen state
 * @param count  Number of columns to advance (usually 1)
 */
static void screen_advance_cursor(screen_t *screen, uint16_t count)
{
	screen->screen_column += count;
	while (screen->screen_column >= SCREEN_WIDTH)
	{
		screen->screen_column -= (SCREEN_WIDTH - screen->start_column);
		screen_save_row_to_history(screen, screen->screen_row);
		screen->screen_row++; 
		if (screen->screen_row >= SCREEN_HEIGHT)
		{
			screen->screen_row = SCREEN_HEIGHT - 1;
			screen_print_history(screen, SCREEN_HEIGHT - 1 - screen->start_row, 0);
		}
	}
	screen->cursor_column = screen->screen_column;
	screen->cursor_row = screen->screen_row;
	cursor_update(screen->cursor_column, screen->cursor_row);
}

/**
 * @brief Performs a newline: saves current row and moves to next line.
 *
 * Resets column to start_column and handles scrolling if at bottom.
 *
 * @param screen Pointer to screen state
 */
static void screen_newline(screen_t *screen)
{
	screen_save_row_to_history(screen, screen->screen_row);
	
	screen->screen_row++;
	screen->screen_column = screen->start_column;
	
	if (screen->screen_row >= SCREEN_HEIGHT)
	{
		screen->screen_row = SCREEN_HEIGHT - 1;
		screen_print_history(screen, SCREEN_HEIGHT- 1 - screen->start_row, 0);
	}
	screen->cursor_column = screen->screen_column;
	screen->cursor_row = screen->screen_row;
	cursor_update(screen->cursor_column, screen->cursor_row);
}

/**
 * @brief Resets state when starting to write new input.
 *
 * If user was viewing history (history_offset > 0), clears it and switches to write cursor.
 * Also removes incomplete last history entry from previous input.
 *
 * Called at the start of aby kinf of user print such as screen_put_char() and screen_put_str().
 */
static void cursor_at_put_reset(screen_t *screen)
{
if (screen->history_offset != 0u)
	{
		screen->history_offset = 0U;
		cursor_set_style(SCREEN_CURSOR_SETTING_WRITE);
		screen_print_history(screen, SCREEN_HEIGHT - screen->start_row, 0);
		history_last_entry_remove(screen->history_buffer);
	}
	if (screen->cursor_row != screen->screen_row)
	{
		cursor_set_style(SCREEN_CURSOR_SETTING_WRITE);
		screen->cursor_row = screen->screen_row;
		screen->cursor_column = screen->screen_column;
	}
}

void screen_put_char(screen_t *screen, char c)
{
	cursor_at_put_reset(screen);
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
	
	cursor_at_put_reset(screen);
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
	md_printk(KERN_INFO "Initializing screen with header: %s\n", header_str);
	screen->screen_buffer = (uint16_t*)VGA_MEMORY;
    screen->screen_row = 1;
	screen->screen_column = 0;
	screen->start_column = 0;
	screen->start_row = 1;
	screen->screen_color_default = default_color;
	screen->screen_color_current = screen->screen_color_default;
	screen->history_buffer = history_buffer;
	(void)md_strlencpy(screen->screen_header, header_str, SCREEN_WIDTH);
	screen->history_offset = 0;
	history_init(history_buffer);

	screen->cursor_column = screen->start_column;
	screen->cursor_row = screen->start_row;
}

/**
 * @brief Renders the centered header string at the top of the screen.
 *
 * Only the visible portion (up to SCREEN_WIDTH) is shown.
 */
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
	md_printk(KERN_NOTICE "Opening screen with header: %s\n", screen->screen_header);

	screen_clear(screen, 0, screen->screen_color_default);
	print_header(screen);
	screen_print_history(screen, SCREEN_HEIGHT - screen->start_row, screen->history_offset);
	if (screen->history_offset == 0u)
	{
		history_last_entry_remove(screen->history_buffer);
	}

	cursor_update(screen->cursor_column, screen->cursor_row);
	if (screen->history_offset == 0u && screen->cursor_row == screen->screen_row)
	{
		cursor_set_style(SCREEN_CURSOR_SETTING_WRITE);
	}
	else
	{
		cursor_set_style(SCREEN_CURSOR_SETTING_READ);
	}
	cursor_enable();
}

void screen_close(screen_t *screen)
{
	md_printk(KERN_NOTICE "Closing screen with header: %s\n", screen->screen_header);
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

	number_of_lines += screen->start_row; //Adding header offset
	number_of_lines--;
	if (number_of_lines >= SCREEN_HEIGHT)
	{
		number_of_lines = SCREEN_HEIGHT - 1;
	}
	
	for (row_cnt = number_of_lines; row_cnt >= screen->start_row; row_cnt--)
	{
		uint16_t *entry = history_get_entry(screen->history_buffer, pos_helper);
		for (uint16_t x = screen->start_column; x < SCREEN_WIDTH; x++)
		{
			screen->screen_buffer[row_cnt * SCREEN_WIDTH + x] = entry[x - screen->start_column];
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

/**
 * @brief Computes maximum allowed history offset (how far back user can scroll).
 *
 * @return Number of history entries beyond visible area (0 if all fit)
 */
static uint32_t screen_get_max_history_offset(screen_t *screen)
{
	uint32_t ret = 0u;
	uint32_t his_size = history_get_num_of_entrys(screen->history_buffer);
	uint16_t writable = SCREEN_HEIGHT - screen->start_row ;

	if (his_size > writable)
	{
		ret = his_size - (uint32_t)writable;
	}
	
	return ret;
}

void screen_scroll_up(screen_t *screen)
{
	uint32_t max_his_off = screen_get_max_history_offset(screen);

	if (screen->cursor_row > screen->start_row)
	{
		if (screen->cursor_row == screen->screen_row)
		{
			cursor_set_style(SCREEN_CURSOR_SETTING_READ);
		}
		screen->cursor_row--;
		cursor_update(screen->cursor_column, screen->cursor_row);
	}
	else if (max_his_off > screen->history_offset)
	{
		if (screen->history_offset == 0u)
		{
			screen_save_row_to_history(screen, screen->screen_row);
		}
		screen->history_offset++;
		screen_print_history(screen, SCREEN_HEIGHT - screen->start_row, screen->history_offset);
	}
	else
	{
		md_printk(KERN_DEBUG "SCREEN_SCROLL_UP: Reached top of history!\n");
	}
}

void screen_scroll_down(screen_t *screen)
{
	if (screen->cursor_row < screen->screen_row)
	{
		screen->cursor_row++;
		cursor_update(screen->cursor_column, screen->cursor_row);
		if ((screen->history_offset == 0u) && (screen->cursor_row == screen->screen_row))
		{
			cursor_set_style(SCREEN_CURSOR_SETTING_WRITE); 
		}
	}
	else if (screen->history_offset > 0)
	{
		screen->history_offset--;
		screen_print_history(screen, SCREEN_HEIGHT - screen->start_row, screen->history_offset);
		if (screen->history_offset == 0u)
		{
			history_last_entry_remove(screen->history_buffer);
			cursor_set_style(SCREEN_CURSOR_SETTING_WRITE); 
		}
	}
	else
	{
		md_printk(KERN_DEBUG "SCREEN_SCROLL_DOWN: Reached bottom of history!\n");
	}
}
