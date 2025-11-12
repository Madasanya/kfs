#ifndef _STR_UTILS_H_
#define _STR_UTILS_H_

#include "inttype.h"

#define STR_MAX_LEN 65535u

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
uint16_t md_strlen(const char* str);

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
int16_t md_put_str(const char *str);

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

/**
 * @brief Safely copies a string to a fixed-size buffer with truncation.
 *
 * @details
 * Copies at most (len - 1) characters from src to dest, always ensuring dest
 * is null-terminated. If src is longer than (len - 1), the copy is truncated,
 * but the full length of src (capped at len) is returned. This allows detecting
 * truncation by checking if the return value == len.
 *
 * If len == 0, does nothing and returns 0 (dest unchanged, but should be valid).
 * If len == 1, sets dest[0] = '\0' and returns min(1, strlen(src)).
 *
 * This is similar to the standard strlcpy() function, providing bounds-checked
 * string copying to prevent buffer overflows. Note: unlike standard strlcpy,
 * the return value is min(strlen(src), len), not always strlen(src).
 *
 * @param[out] dest Destination buffer to copy into (must be at least len bytes).
 * @param[in]  src  Null-terminated source string to copy from.
 * @param[in]  len  Maximum size of dest buffer (including null terminator).
 *
 * @return min(strlen(src), len) - the number of iterations performed.
 *         If return == len, truncation likely occurred (unless strlen(src) exactly == len, but see note).
 *
 * @note src must be null-terminated; otherwise, behavior is undefined.
 * @note dest must be writable and at least len bytes; no overlap with src allowed.
 * @warning If len is 0 or dest is NULL, may cause undefined behavior.
 * @note For exact strlen(src) == len, it truncates the last character (does not copy it), so treat == len as truncated.
 */
uint16_t md_strlencpy(char *dest, const char *src, uint16_t len);

#endif /* _STR_UTILS_H_ */
