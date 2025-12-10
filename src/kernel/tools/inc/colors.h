#ifndef _COLORS_H_
#define _COLORS_H_

#include "inttype.h"

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

/**
 * @brief Predefined VGA color profiles to choose from.
 */
enum vga_color_profile {
	VGA_MODE_GREEN_ON_BLACK = 0x02,
	VGA_MODE_WHITE_ON_BLACK = 0x0F,
	VGA_MODE_BLACK_ON_WHITE = 0xF0,
	VGA_MODE_BLACK_ON_RED = 0x40,
	VGA_MODE_BLUE_ON_CYAN = 0x31
};

#define NUM_SCREEN_COLOR_PROFILES 5

/**
 * @brief Default color profiles for multiple screens.
 */
extern const uint8_t SCREEN_COLOR_PROFILES[NUM_SCREEN_COLOR_PROFILES];

/**
 * @brief   Creates a color attribute from foreground and background colors.
 *
 * @details Combines foreground and background colors into a single byte attribute
 *          used by VGA text mode. Background color occupies upper 4 bits, 
 *          foreground color occupies lower 4 bits.
 *
 * @param[in] fg Foreground color (0-15).
 * @param[in] bg Background color (0-15).
 * 
 * @return Combined color attribute byte.
 */
uint8_t set_color(uint8_t fg, uint8_t bg);

/**
 * @brief   Extracts foreground and background colors from a color attribute.
 *
 * @details Decomposes a VGA color attribute byte into its foreground and 
 *          background color components.
 *
 * @param[in]  color Color attribute byte to decompose.
 * @param[out] fg    Pointer to store the foreground color (0-15).
 * @param[out] bg    Pointer to store the background color (0-15).
 */
void get_color(uint8_t color, uint8_t *fg, uint8_t *bg);

/**
 * @brief   Gets the foreground color from a color attribute.
 *
 * @details Extracts the foreground color component from a VGA color attribute byte.
 *
 * @param[in] color Color attribute byte.
 * 
 * @return Foreground color value (0-15).
 */
uint8_t get_fg_color(uint8_t color);

/**
 * @brief   Gets the background color from a color attribute.
 *
 * @details Extracts the background color component from a VGA color attribute byte.
 *
 * @param[in] color Color attribute byte.
 * 
 * @return Background color value (0-15).
 */
uint8_t get_bg_color(uint8_t color);

#endif /* _COLORS_H_ */