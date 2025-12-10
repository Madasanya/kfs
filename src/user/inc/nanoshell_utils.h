#ifndef _NANOSHELL_UTILS_H_
#define _NANOSHELL_UTILS_H_

#include "inttype.h"

#define SYS_SCROLL_UP 0u
#define SYS_SCROLL_DOWN 1u
#define KEYBOARD_COMM_SCROLL_UP 0x01u         /** < Command to scroll up (triggered by Up Arrow key). */
#define KEYBOARD_COMM_SCROLL_DOWN 0x02u     /** < Command to scroll down (triggered by Down Arrow key). */

/**
 * @brief Scroll the screen in the specified direction.
 *
 * @param[in] dir Scroll direction (SYS_SCROLL_UP or SYS_SCROLL_DOWN).
 * @return Status code (0 on success).
 */
uint8_t nanoshell_scroll(uint8_t dir);

/**
 * @brief Get a line of input from the user.
 *
 * @details Reads characters from keyboard until Enter is pressed,
 *          supporting basic line editing and history browsing.
 *
 * @param[out] arr  Buffer to store the input line.
 * @param[in]  size Maximum size of the buffer.
 * @return Number of characters read (excluding null terminator).
 */
uint32_t nanoshell_gnl(char *arr, uint32_t size);

/**
 * @brief Get a single command from keyboard.
 *
 * @details Reads special command input (like scroll commands)
 *          from the keyboard.
 *
 * @param[out] comm Pointer to store the command code.
 * @return Number of commands read (0 or 1).
 */
uint32_t nanoshell_gnc(uint8_t *comm);

void nanoshell_wait(void);


#endif /* _NANOSHELL_UTILS_H_ */