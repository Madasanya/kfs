#ifndef _ISR_SYSCALL_H_
#define _ISR_SYSCALL_H_

#include "inttype.h"

uint32_t isr_syscall_errprint(uint32_t lvl);
uint32_t isr_syscall_errwrite(char *arr, uint32_t len, uint32_t lvl);


#endif /* _ISR_SYSCALL_H_ */