#ifndef _USER_SYSCALL_H_
#define _USER_SYSCALL_H_

#include "inttype.h"

/**
 * @brief Read character input from keyboard.
 *
 * @param[out] arr Buffer to store characters.
 * @param[in]  len Maximum number of characters to read.
 * @return Number of characters read.
 */
uint32_t user_sysread(char *arr, uint8_t len);

/**
 * @brief Read command input from keyboard.
 *
 * @param[out] arr Buffer to store command codes.
 * @param[in]  len Maximum number of commands to read.
 * @return Number of commands read.
 */
uint32_t user_syscommread(uint8_t *arr, uint8_t len);

/**
 * @brief Write data to the screen.
 *
 * @param[in] arr Pointer to data to write.
 * @param[in] len Length of data.
 * @return Number of characters written.
 */
uint32_t user_syswrite(char *arr, uint32_t len);

/**
 * @brief Scroll the screen view.
 *
 * @param[in] dir Direction to scroll.
 * @param[in] num Number of lines to scroll.
 * @return Status code.
 */
uint32_t user_sysscroll(uint32_t dir, uint32_t num);

/**
 * @brief Switch to a different virtual screen.
 *
 * @param[in] mode Operation mode.
 * @param[in] num  Screen number or delta.
 * @return Status code.
 */
uint32_t user_sysscreenset(uint8_t mode, uint8_t num);

/**
 * @brief Change the screen color scheme.
 *
 * @param[in] mode Operation mode.
 * @param[in] num  Color scheme number or delta.
 * @return Status code.
 */
uint32_t user_syscolorset(uint8_t mode, uint8_t num);

/**
 * @brief Display hexadecimal dump of memory.
 *
 * @param[in] start_addr Starting address.
 * @param[in] len        Number of bytes to dump.
 * @return Number of bytes dumped.
 */
uint32_t user_syshexdump(void *start_addr, uint32_t len);

/**
 * @brief Write an entry to the error log.
 *
 * @param[in] arr Pointer to error message.
 * @param[in] len Length of message.
 * @param[in] lvl Error severity level.
 * @return Number of characters written.
 */
uint32_t user_syserrwrite(char *arr, uint32_t len, uint32_t lvl);

/**
 * @brief Print error log entries.
 *
 * @param[in] lvl Maximum error level to display.
 * @return Status code.
 */
uint32_t user_syserrprint(uint32_t lvl);

/**
 * @brief Halt the CPU via system call.
 *
 * @details Issues a system call to halt the processor. This puts the CPU
 *          in a low-power halted state. Typically does not return.
 */
void user_syshalt(void);

/**
 * @brief Print kernel stack contents via system call.
 *
 * @details Requests the kernel to display the entire kernel stack memory
 *          region in hexadecimal format. Useful for debugging and analysis.
 *
 * @return Number of bytes dumped from the kernel stack.
 */
uint32_t user_stackprint(void);


#endif /* _USER_SYSCALL_H_ */