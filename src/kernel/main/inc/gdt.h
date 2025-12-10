#ifndef _GDT_H_
#define _GDT_H_

#include "inttype.h"

/**
 * @brief Initialize the Global Descriptor Table.
 *
 * @details Sets up the GDT with necessary segments for kernel and user mode
 *          operation, including code and data segments with appropriate
 *          privilege levels (ring 0 for kernel, ring 3 for user).
 */
void gdt_init(void);

#endif /* _GDT_H_ */