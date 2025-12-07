#ifndef _USER_STR_UTILS_H_
#define _USER_STR_UTILS_H_

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
uint16_t user_strlen(const char* str);

/**
 * @brief Copy a block of memory (no overlap allowed)
 *
 * Copies exactly @p byte_count bytes from @p source to @p destination.
 * The behavior is undefined if the source and destination regions overlap.
 * Use user_memmove() when overlap is possible.
 *
 * @param destination  Pointer to the destination memory
 * @param source       Pointer to the source memory
 * @param byte_count   Number of bytes to copy
 *
 * @return Original value of @p destination
 */
void *user_memcpy(void *destination, const void *source, uint32_t byte_count);

/**
 * @brief Copy a block of memory, safely handling overlapping regions
 *
 * Copies @p byte_count bytes from @p source to @p destination.
 * The function works correctly even when the source and destination
 * memory regions overlap.
 *
 * @param destination  Pointer to the destination memory
 * @param source       Pointer to the source memory
 * @param byte_count   Number of bytes to copy
 *
 * @return Original value of @p destination
 */
void *user_memmove(void *destination, const void *source, uint32_t byte_count);

#endif /* _USER_STR_UTILS_H_ */