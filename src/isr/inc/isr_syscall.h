#ifndef _ISR_SYSCALL_H_
#define _ISR_SYSCALL_H_

#include "inttype.h"
#include "syscall.h"

uint32_t isr_syscall_errprint(uint32_t lvl);
uint32_t isr_syscall_errwrite(char *arr, uint32_t len, uint32_t lvl);
uint32_t isr_syscall_write(const char* data_arr, uint32_t len);
uint32_t isr_syscall_scroll(uint8_t dir, uint32_t num);
uint32_t isr_syscall_screenset(uint8_t mode, uint8_t num);
uint32_t isr_syscall_colorset(uint8_t mode, uint8_t num);
uint32_t isr_syscall_hexdump(const void* start_addr, uint32_t len);

#endif /* _ISR_SYSCALL_H_ */