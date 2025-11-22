#ifndef _SCREEN_H
#define _SCREEN_H

#include "inttype.h"
#include "kernel.h"
#include "screen_settings.h"

#define VGA_MEMORY  0xB8000

/* Forward declaration */
typedef struct history_buffer_s history_buffer_t; 

typedef struct screen_s{
    uint16_t *screen_buffer;
    uint16_t start_row;
    uint16_t start_column;
    uint16_t screen_row;
    uint16_t screen_column;
    char    screen_header[SCREEN_WIDTH];
    uint16_t screen_color_default;
    uint16_t screen_color_current;
    history_buffer_t *history_buffer;
    uint32_t history_offset;
    uint16_t cursor_row;
    uint16_t cursor_column;
} screen_t;

/**
 * @brief   Sets the current color for the screen.
 *
 * @details Updates the screen's current color attribute used for subsequent 
 *          character output operations.
 *
 * @param[in,out] screen Pointer to the screen structure.
 * @param[in]     color  The color attribute to set.
 */
void screen_set_color(screen_t *screen, uint8_t color);

/**
 * @brief   Writes a single character to the screen.
 *
 * @details Writes a character at the current cursor position with the current color.
 *          Automatically advances the cursor position and wraps around if needed.
 *
 * @param[in,out] screen Pointer to the screen structure.
 * @param[in]     c      The character to display.
 */
void screen_put_char(screen_t *screen, char c);

/**
 * @brief   Writes a string to the screen.
 *
 * @details Writes a null-terminated string at the current cursor position with 
 *          the current color. Advances cursor position and wraps around if needed.
 *
 * @param[in,out] screen Pointer to the screen structure.
 * @param[in]     str    Pointer to the null-terminated string to display.
 */
void screen_put_str(screen_t *screen, const char *str);

/**
 * @brief   Initializes the screen structure.
 *
 * @details Initializes the screen with VGA memory buffer, resets cursor position,
 *          sets default color, associates history buffer, and clears the screen
 *          by filling it with spaces.
 *
 * @param[in,out] screen         Pointer to the screen structure to initialize.
 * @param[in]     history_buffer Pointer to the history buffer to associate.
 * @param[in]     default_color  The default color attribute for the screen.
 */
void screen_init(screen_t *screen, history_buffer_t *history_buffer, uint8_t default_color, char *header_str);

/**
 * @brief   Clears the screen from a starting position to the end.
 *
 * @details Fills the screen buffer from the specified starting position to the end
 *          of the screen with spaces using the specified color attribute.
 *
 * @param[in,out] screen      Pointer to the screen structure.
 * @param[in]     start_index The starting buffer position (0 to SCREEN_WIDTH * SCREEN_HEIGHT - 1).
 * @param[in]     color       The color attribute to use for cleared characters.
 */
void screen_clear(screen_t *screen, uint16_t start_index, uint8_t color);

/**
 * @brief   Prints the history buffer to the screen.
 *
 * @details Prints the history buffer to screen, displaying the most recent commands.
 *          Handles circular buffer wrap-around to show commands in chronological order.
 *          Calculates starting position for printing based on screen height. If 
 *          wrap-around is needed, prints from the end of the buffer first, then 
 *          from the beginning up to the last command.
 *
 * @param[in,out] screen Pointer to the screen structure.
 */
void screen_print_history(screen_t *screen, uint16_t number_of_lines, uint32_t history_offset);

/**
 * @brief   Saves the current screen row to the history buffer.
 *
 * @details Copies the current row from the screen buffer to the history buffer,
 *          preserving all colored character data for later restoration.
 *
 * @param[in,out] screen Pointer to the screen structure.
 * @param[in]     row    The row number to save (0-based).
 */
void screen_save_row_to_history(screen_t *screen, uint16_t row);

/**
 * @brief   Opens and refreshes the screen display.
 *
 * @details Clears the entire screen, prints the header, and displays the history
 *          buffer. If not in history browsing mode (history_offset == 0), removes
 *          the last history entry to prepare for new input.
 *
 * @param[in,out] screen Pointer to the screen structure.
 */
void screen_open(screen_t *screen);

/**
 * @brief   Closes the screen and saves current state.
 *
 * @details Saves the current screen row to the history buffer if not currently
 *          browsing history (history_offset == 0). Should be called before
 *          switching away from the screen.
 *
 * @param[in,out] screen Pointer to the screen structure.
 */
void screen_close(screen_t *screen);

/**
 * @brief   Scrolls the screen view up through history.
 *
 * @details Increases the history offset to show older entries, effectively scrolling
 *          up through command history. Saves the current row to history on first
 *          scroll up. Does nothing if already at the maximum history offset.
 *
 * @param[in,out] screen Pointer to the screen structure.
 */
void screen_scroll_up(screen_t *screen);

/**
 * @brief   Scrolls the screen view down through history.
 *
 * @details Decreases the history offset to show more recent entries, effectively
 *          scrolling down through command history. Removes the last history entry
 *          when returning to normal mode (history_offset == 0).
 *
 * @param[in,out] screen Pointer to the screen structure.
 */
void screen_scroll_down(screen_t *screen);

#endif /* _SCREEN_H */