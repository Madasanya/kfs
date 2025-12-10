#include "colors.h"

const uint8_t SCREEN_COLOR_PROFILES[NUM_SCREEN_COLOR_PROFILES] = {
	VGA_MODE_GREEN_ON_BLACK,
    VGA_MODE_WHITE_ON_BLACK,
    VGA_MODE_BLACK_ON_WHITE,
    VGA_MODE_BLACK_ON_RED,
    VGA_MODE_BLUE_ON_CYAN
};

uint8_t set_color(uint8_t fg, uint8_t bg) 
{
	return fg | bg << 4;
}

void get_color(uint8_t color, uint8_t *fg, uint8_t *bg)
{
    *fg = color & 0x0F;
    *bg = (color >> 4) & 0x0F;
}

uint8_t get_fg_color(uint8_t color)
{
    return color & 0x0F;
}

uint8_t get_bg_color(uint8_t color)
{
    return (color >> 4) & 0x0F;
}
