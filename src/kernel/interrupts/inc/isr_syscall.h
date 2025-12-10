#ifndef _ISR_SYSCALL_H_
#define _ISR_SYSCALL_H_

#include "inttype.h"
#include "syscall.h"

#define MAX_USER_ADDR (void *)0x400000
#define MIN_USER_ADDR (void *)0x200000

/**
 * @brief Print error log entries.
 *
 * @param[in] lvl Maximum error level to display.
 * @return Status code.
 */
uint32_t isr_syscall_errprint(uint32_t lvl);

/**
 * @brief Write an entry to the error log.
 *
 * @param[in] arr Pointer to the error message string.
 * @param[in] len Length of the message.
 * @param[in] lvl Error severity level.
 * @return Number of characters written.
 */
uint32_t isr_syscall_errwrite(const char *arr, uint32_t len, uint32_t lvl);

/**
 * @brief Write data to the screen.
 *
 * @param[in] data_arr Pointer to the data to write.
 * @param[in] len      Length of the data.
 * @return Number of characters written.
 */
uint32_t isr_syscall_write(const char* data_arr, uint32_t len);

/**
 * @brief Scroll the screen view.
 *
 * @param[in] dir Direction to scroll (up or down).
 * @param[in] num Number of lines to scroll.
 * @return Status code.
 */
uint32_t isr_syscall_scroll(uint8_t dir, uint32_t num);

/**
 * @brief Switch to a different virtual screen.
 *
 * @param[in] mode Operation mode (absolute, increase, decrease).
 * @param[in] num  Screen number or delta value.
 * @return Status code.
 */
uint32_t isr_syscall_screenset(uint8_t mode, uint8_t num);

/**
 * @brief Change the screen color scheme.
 *
 * @param[in] mode Operation mode (absolute, increase, decrease).
 * @param[in] num  Color scheme number or delta value.
 * @return Status code.
 */
uint32_t isr_syscall_colorset(uint8_t mode, uint8_t num);

/**
 * @brief Display a hexadecimal dump of memory.
 *
 * @param[in] start_addr Starting address to dump.
 * @param[in] len        Number of bytes to dump.
 * @return Number of bytes dumped.
 */
uint32_t isr_syscall_hexdump(const void* start_addr, uint32_t len);

/**
 * @brief Read character input from keyboard.
 *
 * @param[out] data_arr Buffer to store characters.
 * @param[in]  len      Maximum number of characters to read.
 * @return Number of characters read.
 */
uint32_t isr_syscall_read(char* data_arr, uint8_t len);

/**
 * @brief Read command input from keyboard.
 *
 * @param[out] data_arr Buffer to store command codes.
 * @param[in]  len      Maximum number of commands to read.
 * @return Number of commands read.
 */
uint32_t isr_syscall_commread(uint8_t* data_arr, uint8_t len);

uint32_t isr_syscall_halt(void);

uint32_t isr_syscall_stackprint(void);


#endif /* _ISR_SYSCALL_H_ */