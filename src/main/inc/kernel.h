#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "screen.h"
#include "keyboard.h"

#define NUM_SCREENS 5

extern uint8_t g_current_screen_index;
extern uint8_t g_current_color_index[NUM_SCREENS];
extern screen_t *g_active_screen;
extern keyboard_t g_keyboard;

#endif /* _KERNEL_H_ */
