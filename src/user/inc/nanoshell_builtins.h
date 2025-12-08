#ifndef _NANOSHELL_BUILTINS_H_
#define _NANOSHELL_BUILTINS_H_

#include "inttype.h"
uint8_t nanoshell_echo(char* str);
uint32_t nanoshell_gnl(char *arr, uint32_t size);
uint32_t nanoshell_gnc(uint8_t *comm);


#endif /* _NANOSHELL_BUILTINS_H_ */