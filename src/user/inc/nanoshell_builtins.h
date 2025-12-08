#ifndef _NANOSHELL_BUILTINS_H_
#define _NANOSHELL_BUILTINS_H_

#include "inttype.h"
uint8_t nanoshell_echo(char* str);
uint8_t nanoshell_errlog_write(char *str);
uint8_t nanoshell_errlog_print(char *lvl_str);
uint8_t nanoshell_screen_switch(char *num_str);
uint8_t nanoshell_color_switch(char *num_str);
uint8_t nanoshell_hexdump(char *hex_str);
//extern uint8_t exit_to_kernel(char *str);
uint32_t nanoshell_gnl(char *arr, uint32_t size);
uint32_t nanoshell_gnc(uint8_t *comm);


#endif /* _NANOSHELL_BUILTINS_H_ */