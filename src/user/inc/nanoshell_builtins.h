#ifndef _NANOSHELL_BUILTINS_H_
#define _NANOSHELL_BUILTINS_H_

#include "inttype.h"
uint8_t nanoshell_echo(char* str);
uint8_t nanoshell_errlog_write(char *str);
uint8_t nanoshell_errlog_print(char *lvl_str);
uint8_t nanoshell_screen_switch(char *num_str);
//extern uint8_t exit_to_kernel(char *str);

#endif /* _NANOSHELL_BUILTINS_H_ */