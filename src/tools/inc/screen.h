#ifndef _SCREEN_H
#define _SCREEN_H

#include "inttype.h"

#define VGA_MEMORY  0xB8000

/* Forward declaration */
typedef struct history_buffer_s history_buffer_t; 

typedef struct screen_s{
    uint16_t *screen_buffer;
    uint16_t screen_row;
    uint16_t screen_column;
    char *screen_header;
    uint16_t screen_color_default;
    uint16_t screen_color_current;
    history_buffer_t *history_buffer;

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
 * @brief   Puts a single character on the screen.
 *
 * @details Writes a character at the current cursor position with the current color.
 *          Automatically advances the cursor position and wraps around if needed.
 *
 * @param[in,out] screen Pointer to the screen structure.
 * @param[in]     c      The character to display.
 */
void screen_put_char(screen_t *screen, char c);

/**
 * @brief   Puts a string on the screen.
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
void screen_init(screen_t *screen, history_buffer_t *history_buffer, uint8_t default_color);

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
void screen_print_history(screen_t *screen);

#endif /* _SCREEN_H */