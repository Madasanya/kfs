#ifndef CURSOR_H
# define CURSOR_H

#include "inttype.h"

/**
 * @brief   Enables the hardware cursor with specified scan lines.
 *
 * @details Configures the VGA hardware cursor by setting the cursor start 
 *          and end scan lines in the VGA CRT Controller registers at 0x0A 
 *          through the ports 0x3D4/0x3D5.
 *          The cursor will be visible between the start and end scan lines.
 *
 * @param[in] cursor_start The starting scan line for the cursor (0-15).
 * @param[in] cursor_end   The ending scan line for the cursor (0-15).
 */
void cursor_enable(uint8_t cursor_start, uint8_t cursor_end);

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
