#ifndef _USER_STR_UTILS_H_
#define _USER_STR_UTILS_H_

#include "inttype.h"

#define STR_MAX_LEN 65535u
#define DECIMAL_BASE 10u
#define HEX_BASE 16u

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

uint8_t user_atoi_base(uint32_t *val, const char* str, uint8_t base);

#endif /* _USER_STR_UTILS_H_ */