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

#endif /* _USER_STR_UTILS_H_ */