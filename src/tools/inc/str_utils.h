#ifndef _STR_UTILS_H_
#define _STR_UTILS_H_

#include "inttype.h"

/**
 * @brief   Calculates the length of a null-terminated string.
 *
 * @details This function iterates through the string until it finds the null terminator,
 *          counting the number of characters.
 *
 * @param[in] str Pointer to the null-terminated string.
 * 
 * @return The length of the string (excluding the null terminator).
 */
uint16_t md_strlen(char* str);

/**
 * @brief Outputs a single character to the text screen at the current cursor position.
 *
 * @details This function writes the character to video memory and advances the cursor.
 *          It also sets the attribute byte to 0x20 (space with default attributes).
 *          Fails if cursor is at or beyond position 80.
 *
 * @param[in] c The character to output.
 * 
 * @return 1 on success, -1 if cursor is out of bounds.
 */
int8_t md_put_char(char c);

/**
 * @brief   Writes a null-terminated string to the text screen.
 *
 * @details This function calls md_put_char for each character in the string until
 *          the null terminator or an error occurs.
 *
 * @param[in] str Pointer to the null-terminated string to write.
 * 
 * @return The number of characters written on success, -1 on error.
 */
int16_t md_put_str(char *str);

/**
 * @brief   Converts a 32-bit unsigned integer to a hexadecimal string.
 *
 * @details This function generates the hexadecimal representation (uppercase letters for A-F)
 *          of the number and stores it in the output buffer.
 *          Handles zero as a special case. The output is null-terminated.
 *
 * @param[in] num       The unsigned 32-bit integer to convert.
 * @param[out] output   Pointer to the buffer where the hexadecimal string will be stored.
 *                      The buffer should be large enough (at least 9 bytes for 32-bit hex + null).
 */
void md_ptoa(uint32_t num, char *output);

#endif /* _STR_UTILS_H_ */