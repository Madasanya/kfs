#ifndef _PIC_H_
#define _PIC_H_

#include "inttype.h"

/**
 * @brief Send End-Of-Interrupt signal to PIC
 * @param irq The IRQ line that was serviced (0-15)
 */
void pic_sendEOI(uint8_t irq);

/**
 * @brief Remap the PIC interrupt vectors
 * @param offset1 Vector offset for master PIC (IRQ 0-7)
 * @param offset2 Vector offset for slave PIC (IRQ 8-15)
 */
void pic_remap(int offset1, int offset2);

/**
 * @brief Disable the PIC by masking all interrupts
 */
void pic_disable(void);

/**
 * @brief Mask (disable) a specific IRQ line
 * @param IRQline The IRQ line to mask (0-15)
 */
void irq_set_mask(uint8_t IRQline);

/**
 * @brief Unmask (enable) a specific IRQ line
 * @param IRQline The IRQ line to unmask (0-15)
 */
void irq_clear_mask(uint8_t IRQline);

/**
 * @brief Get the Interrupt Request Register value
 * @return 16-bit value showing pending IRQs
 */
uint16_t pic_get_irr(void);

/**
 * @brief Get the In-Service Register value
 * @return 16-bit value showing IRQs being serviced
 */
uint16_t pic_get_isr(void);

/**
 * @brief Initialize the PIC and keyboard hardware.
 *
 * @details Performs complete PIC and keyboard initialization sequence:
 *          1. Remaps PIC interrupt vectors to 0x20-0x2F (32-47) to avoid
 *             conflicts with CPU exception handlers (vectors 0-31)
 *          2. Enables IRQ1 (keyboard) by clearing its mask bit
 *          3. Flushes any pending keyboard data from the controller buffer
 *          4. Sends reset command (0xFF) to keyboard controller
 *          5. Waits for reset to complete
 *          6. Drains buffer again to clear any reset response
 *
 *          This initialization must be called after IDT setup but before
 *          enabling interrupts with STI instruction.
 */
void pic_init(void);

#endif /* _PIC_H_ */
