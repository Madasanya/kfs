#ifndef _ISR_DUMMY_H_
#define _ISR_DUMMY_H_

/**
 * @brief Non-blocking dummy ISR.
 *
 * @details Simply returns from interrupt (IRET). Safe default handler
 *          for unused interrupt vectors that allows execution to continue.
 */
extern void isr_dummy_nonblocking(void);   // just iret — safe default

/**
 * @brief Blocking dummy ISR.
*
 * @details Disables interrupts (CLI) and halts the CPU in a loop (HLT).
 *          Used for debugging to catch unexpected interrupts that should
 *          never occur.
 */
extern void isr_dummy_blocking(void);      // cli + hlt loop — catches bugs loudly

#endif /* _ISR_DUMMY_H_ */