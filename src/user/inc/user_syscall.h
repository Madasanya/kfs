#ifndef _USER_SYSCALL_H_
#define _USER_SYSCALL_H_

#include "inttype.h"

uint32_t user_sysread(char *arr, uint32_t len);
uint32_t user_sysreadcomm(uint8_t *arr, uint32_t len);

uint32_t user_syswrite(char *arr, uint32_t len);
uint32_t user_sysscroll(uint32_t dir, uint32_t num);
uint32_t user_syssetscreen(uint32_t num);
uint32_t user_syssetfont(uint32_t font);

uint32_t user_syswriterr(char *arr, uint32_t len, uint32_t lvl);
uint32_t user_sysprinterr(uint32_t lvl);


#endif /* _USER_SYSCALL_H_ */