#ifndef _NANOSHELL_UTILS_H_
#define _NANOSHELL_UTILS_H_

#include "inttype.h"

#define SYS_SCROLL_UP 0u
#define SYS_SCROLL_DOWN 1u
#define KEYBOARD_COMM_SCROLL_UP 0x01u         /** < Command to scroll up (triggered by Up Arrow key). */
#define KEYBOARD_COMM_SCROLL_DOWN 0x02u     /** < Command to scroll down (triggered by Down Arrow key). */

uint8_t nanoshell_scroll(uint8_t dir);
uint32_t nanoshell_gnl(char *arr, uint32_t size);
uint32_t nanoshell_gnc(uint8_t *comm);

#endif /* _NANOSHELL_UTILS_H_ */