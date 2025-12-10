#ifndef IDT_H
#define IDT_H

#include "inttype.h"

#define IDT_PRESENT         0x80

#define GATE_32BIT_INT      0x0E
#define GATE_32BIT_TRAP     0x0F

#define DPL_0               0x00
#define DPL_3               0x60    // 11 << 5

// The only 3 you will ever use:
#define KERNEL_INT_GATE     (IDT_PRESENT | DPL_0 | GATE_32BIT_INT)   // 0x8E
#define KERNEL_TRAP_GATE    (IDT_PRESENT | DPL_0 | GATE_32BIT_TRAP)  // 0x8F (rare)
#define USER_TRAP_GATE      (IDT_PRESENT | DPL_3 | GATE_32BIT_TRAP)  // 0xEE ← syscall

#define IDT_ATTR_INTERRUPTS KERNEL_INT_GATE
#define IDT_ATTR_SYSCALL    USER_TRAP_GATE

/**
 * @brief IDT entry configuration data.
 *
 * @details Simplified structure used to configure an IDT entry,
 *          containing the interrupt service routine address and attributes.
 */
typedef struct {
    uint32_t isr;        /**< Address of the interrupt service routine. */
    uint8_t  attributes; /**< Gate type and privilege level attributes. */
} idt_entry_data_t;


/**
 * @brief Save the current IDT pointer.
 *
 * @details Stores the current IDT register using the SIDT instruction.
 *          Used for debugging or state preservation.
 */
void idt_save(void);

/**
 * @brief Initialize the Interrupt Descriptor Table.
 *
 * @details Sets up the IDT with default handlers and loads it into the CPU
 *          using the LIDT instruction.
 */
void idt_init(void);

/**
 * @brief Set an interrupt handler in the IDT.
 *
 * @details Configures the specified interrupt vector with the given handler
 *          address and attributes.
 *
 * @param[in] num      Interrupt vector number (0-255).
 * @param[in] isr_data Pointer to the ISR configuration data.
 */
void idt_interrupt_set(uint8_t num, const idt_entry_data_t *isr_data);

#endif