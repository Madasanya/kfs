#ifndef _MULTIBOOT_H_
#define _MULTIBOOT_H_

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
void kernel(void);

#endif /* _MULTIBOOT_H_ */