#ifndef _NANOSHELL_BUILTINS_H_
#define _NANOSHELL_BUILTINS_H_

#include "inttype.h"

/**
 * @brief Echo text to the screen.
 *
 * @param[in] str Text to display.
 * @return Status code (0 on success).
 */
uint8_t nanoshell_echo(char* str);

/**
 * @brief Write a message to the error log.
 *
 * @param[in] str Message to log.
 * @return Status code (0 on success).
 */
uint8_t nanoshell_errlog_write(char *str);

/**
 * @brief Print error log entries up to specified level.
 *
 * @param[in] lvl_str String representation of log level.
 * @return Status code (0 on success).
 */
uint8_t nanoshell_errlog_print(char *lvl_str);

/**
 * @brief Switch to a different virtual screen.
 *
 * @param[in] num_str String representation of screen number.
 * @return Status code (0 on success).
 */
uint8_t nanoshell_screen_switch(char *num_str);

/**
 * @brief Switch to a different color scheme.
 *
 * @param[in] num_str String representation of color scheme number.
 * @return Status code (0 on success).
 */
uint8_t nanoshell_color_switch(char *num_str);

/**
 * @brief Display hexadecimal dump of memory.
 *
 * @param[in] hex_str Hexadecimal address string.
 * @return Status code (0 on success).
 */
uint8_t nanoshell_hexdump(char *hex_str);

/**
 * @brief Print kernel stack information.
 *
 * @return Status code (0 on success).
 */
uint8_t nanoshell_kernel_stack_print();

#endif /* _NANOSHELL_BUILTINS_H_ */