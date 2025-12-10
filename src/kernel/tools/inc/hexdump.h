#ifndef _HEXDUMP_H_
#define _HEXDUMP_H_

#include "inttype.h"

/**
 * @brief Print a hexadecimal dump of memory contents.
 *
 * @details Displays memory contents in hexadecimal format.
 *          Output format typically shows address and hex bytes.
 *
 * @param[in] start_addr Pointer to the starting address of memory to dump.
 * @param[in] len        Number of bytes to dump.
 *
 * @return Number of bytes successfully dumped.
 */
uint32_t md_hexdump_print(const void *start_addr, uint32_t len);

#endif /* _HEXDUMP_H_ */
