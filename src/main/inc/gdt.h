#ifndef _GDT_H_
#define _GDT_H_

#include "inttype.h"

#define KERNEL_CODE_SEL  0x08
#define USER_CODE_SEL    0x13
#define KERNEL_DATA_SEL  0x18
#define USER_DATA_SEL    0x23
#define KERNEL_BSS_SEL   0x28
#define USER_BSS_SEL     0x33

void init_gdt(void);



#endif /* _GDT_H_ */