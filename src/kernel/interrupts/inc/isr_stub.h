#ifndef _ISR_STUB_H_
#define _ISR_STUB_H_

/**
 * @brief System call interrupt handler stub (INT 0x80).
 *
 * @details Entry point for user mode system calls. Saves context and
 *          dispatches to the appropriate system call handler.
 */
extern void isr_stub80(void); // Syscall interrupt handler

/**
 * @brief Keyboard interrupt handler stub (IRQ 1).
 *
 * @details Entry point for keyboard hardware interrupts. Saves context
 *          and dispatches to the keyboard interrupt handler.
 */
extern void isr_stub21(void);  // Keyboard interrupt handler

#endif /* _ISR_STUB_H_ */