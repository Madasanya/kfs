#ifndef _ISR_DUMMY_H_
#define _ISR_DUMMY_H_

// These two functions are defined in dummy_isr.asm
extern void isr_dummy_nonblocking(void);   // just iret — safe default
extern void isr_dummy_blocking(void);      // cli + hlt loop — catches bugs loudly

#endif /* _ISR_DUMMY_H_ */