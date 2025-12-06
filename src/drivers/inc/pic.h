#ifndef _PIC_H_
#define _PIC_H_

#include "inttype.h"

/**
 * @brief Send End-Of-Interrupt signal to PIC
 * @param irq The IRQ line that was serviced (0-15)
 */
void PIC_sendEOI(uint8_t irq);

/**
 * @brief Remap the PIC interrupt vectors
 * @param offset1 Vector offset for master PIC (IRQ 0-7)
 * @param offset2 Vector offset for slave PIC (IRQ 8-15)
 */
void PIC_remap(int offset1, int offset2);

/**
 * @brief Disable the PIC by masking all interrupts
 */
void PIC_disable(void);

/**
 * @brief Mask (disable) a specific IRQ line
 * @param IRQline The IRQ line to mask (0-15)
 */
void IRQ_set_mask(uint8_t IRQline);

/**
 * @brief Unmask (enable) a specific IRQ line
 * @param IRQline The IRQ line to unmask (0-15)
 */
void IRQ_clear_mask(uint8_t IRQline);

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

#endif /* _PIC_H_ */
