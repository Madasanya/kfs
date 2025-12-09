#ifndef _ISR_STUB_H_
#define _ISR_STUB_H_

extern void isr_stub80(void); // Syscall interrupt handler

// IRQ interrupt service routines
extern void isr_stub21(void);  // Keyboard interrupt handler

#endif /* _ISR_STUB_H_ */