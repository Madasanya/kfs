#ifndef _ISR_SYSCALL_H_
#define _ISR_SYSCALL_H_

#include "inttype.h"
#include "syscall.h"

#define MAX_USER_ADDR (void *)0x400000
#define MIN_USER_ADDR (void *)0x100000

uint32_t isr_syscall_errprint(uint32_t lvl);
uint32_t isr_syscall_errwrite(const char *arr, uint32_t len, uint32_t lvl);
uint32_t isr_syscall_write(const char* data_arr, uint32_t len);
uint32_t isr_syscall_scroll(uint8_t dir, uint32_t num);
uint32_t isr_syscall_screenset(uint8_t mode, uint8_t num);
uint32_t isr_syscall_colorset(uint8_t mode, uint8_t num);
uint32_t isr_syscall_hexdump(const void* start_addr, uint32_t len);
uint32_t isr_syscall_read(char* data_arr, uint8_t len);
uint32_t isr_syscall_commread(uint8_t* data_arr, uint8_t len);


#endif /* _ISR_SYSCALL_H_ */