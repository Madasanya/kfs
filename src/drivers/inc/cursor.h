#ifndef CURSOR_H
# define CURSOR_H

#include "inttype.h"

/**
 * @brief Structure defining cursor settings.
 *
 * This structure holds the starting and ending scan line values for the cursor,
 * which determine its shape and height in a text mode display, typically for VGA-compatible systems.
 */
typedef struct
{
    /**
     * Starting scan line of the cursor (0-15, where 0 is the top).
     */
    uint8_t cursor_start;
    /**
     * Ending scan line of the cursor (0-15, must be >= cursor_start).
     */
    uint8_t cursor_end;
} cursor_setting_t;

/**
 * @brief Predefined cursor style presets.
 *
 * These macros provide commonly used cursor_setting_t configurations for different
 * visual styles in VGA text mode:
 * - @ref CURSOR_SETTING_THIN:   Thin underscore (scan lines 14-15)
 * - @ref CURSOR_SETTING_MEDIUM: Medium height block (scan lines 10-15)
 * - @ref CURSOR_SETTING_THICK:  Thick block (scan lines 6-15)
 * - @ref CURSOR_SETTING_FULL:   Full cell block (scan lines 0-15)
 */
#define CURSOR_SETTING_THIN     (cursor_setting_t){14, 15}
#define CURSOR_SETTING_MEDIUM   (cursor_setting_t){10, 15}
#define CURSOR_SETTING_THICK    (cursor_setting_t){6, 15}
#define CURSOR_SETTING_FULL     (cursor_setting_t){0, 15}

/**
 * @brief Sets the style of the cursor.
 *
 * @details This function configures the starting and ending scan lines of the cursor
 *          using the provided settings. It interacts with VGA CRT Controller register 0x0A
 *          through ports 0x3D4/0x3D5 to define the cursor's size.
 *
 * @param settings A structure containing cursor_start and cursor_end values,
 *                 which specify the scan line positions for the cursor.
 */
void cursor_set_style(cursor_setting_t settings);

/**
 * @brief Enables the cursor.
 *
 * @details Enables the VGA hardware cursor by clearing the cursor disable bit
 *          in the VGA CRT Controller register 0x0A through ports 0x3D4/0x3D5.
 *          Other flags are not affected.
 */
void cursor_enable();

/**
 * @brief   Disables the hardware cursor.
 *
 * @details Disables the VGA hardware cursor by setting the cursor disable bit
 *          in the VGA CRT Controller register 0x0A through ports 0x3D4/0x3D5.
 *          Other flags are not affected.
 */
void cursor_disable();

/**
 * @brief   Updates the cursor position on the screen.
 *
 * @details Sets the hardware cursor position by calculating the linear position
 *          from x,y coordinates and writing it to the VGA CRT Controller registers
 *          (0x0E high register and 0x0F low register) through ports 0x3D4/0x3D5.
 *
 * @param[in] x The column position (0-based).
 * @param[in] y The row position (0-based).
 */
void cursor_update(uint16_t x, uint16_t y);

#endif /* _CURSOR_H */
