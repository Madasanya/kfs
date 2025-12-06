#ifndef _ISR_IRQ_H_
#define _ISR_IRQ_H_

#include "inttype.h"

// IRQ interrupt service routines
extern void isr_irq1_keyboard(void);  // Keyboard interrupt handler

// C handler called from assembly
void irq1_keyboard_handler(void);

#endif /* _ISR_IRQ_H_ */
