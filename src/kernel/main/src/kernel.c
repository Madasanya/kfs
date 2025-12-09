#include "screen.h"
#include "history.h"
#include "errlog.h"
#include "start.h"
#include "colors.h"
#include "keyboard.h"
#include "printk.h"
#include "str_utils.h"
#include "user.h"
#include "usermode.h"
#include "kernel.h"
#include "isr_syscall.h"


#define SCREEN_HEADER_BUF_LEN 15u

screen_t screens[NUM_SCREENS];
history_buffer_t history_buffers[NUM_SCREENS];
char header_buf[SCREEN_HEADER_BUF_LEN];

screen_t *g_active_screen = NULL;
uint8_t g_current_screen_index = 0;
uint8_t g_current_color_index[NUM_SCREENS] = {0};
uint32_t g_super_important_global_var = 42;

// Global keyboard instance (accessed by IRQ handler)
keyboard_t g_keyboard = {0};

/**
 * @brief Main kernel entry point and execution loop.
 *
 * @details
 * This function initializes the kernel subsystems and enters the main kernel loop.
 * It sets up multiple virtual screens with history buffers, initializes the keyboard
 * driver, and continuously processes keyboard input to handle both ASCII character
 * display and special commands (screen switching, scrolling, color changes, error logs).
 *
 * The kernel manages NUM_SCREENS virtual screens, each with its own history buffer
 * and color profile, allowing users to switch between different terminal views.
 *
 * @note This function never returns - it runs in an infinite loop processing keyboard events.
 */
void kernel(void)
{
    /* KERNEL INITIALIZATION */
    md_printk("Kernel inititalization\n");
    for (uint8_t i = 0; i < NUM_SCREENS; i++)
    {
        md_vsnprintf(header_buf, sizeof(header_buf), "42 - Screen %d", i + 1);
        screen_init(&(screens[i]), &(history_buffers[i]), SCREEN_COLOR_PROFILES[i], header_buf);
        g_current_color_index[i] = i;
    }

    g_active_screen = &(screens[g_current_screen_index]);
    screen_open(g_active_screen);

    /* KERNEL RUN */
    md_printk("Kernel running...\n");
    md_printk("Interrupts enabled. Press any key to test...\n");
    user_enter((void*)user_main);
}