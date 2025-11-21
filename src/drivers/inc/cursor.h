#ifndef CURSOR_H
# define CURSOR_H

#include "inttype.h"

typedef struct
{
    uint8_t cursor_start;
    uint8_t cursor_end;
} cursor_setting_t;

#define CURSOR_SETTING_THIN     (cursor_setting_t){14, 15}
#define CURSOR_SETTING_MEDIUM   (cursor_setting_t){10, 15}
#define CURSOR_SETTING_THICK    (cursor_setting_t){6, 15}
#define CURSOR_SETTING_FULL     (cursor_setting_t){0, 15}

void cursor_enable();

void cursor_set_style(cursor_setting_t settings);

/**
 * @brief   Disables the hardware cursor.
 *
 * @details Disables the VGA hardware cursor by setting the cursor disable bit
 *          in the VGA CRT Controller register 0x0A through ports 0x3D4/0x3D5.
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
