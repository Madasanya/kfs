#include "keyboard.h"
#include "io.h"
#include "screen.h"
#include "pic.h"

// External references
extern keyboard_t g_keyboard;
extern screen_t *g_active_screen;

// PIC commands
#define PIC1_COMMAND 0x20
#define PIC_EOI 0x20
#define PS2_DATA 0x60

/**
 * @brief Keyboard interrupt handler (IRQ1)
 * 
 * @details
 * Processes keyboard input and displays characters on the screen.
 */
void irq1_keyboard_handler(void)
{
    char c;
    uint8_t scancode;
    // Read scancode from keyboard data port
    scancode = md_inb(PS2_DATA);
    
    // Process the scancode
    keyboard_process_scancode(&g_keyboard, scancode);
    
    // Print any characters to the screen
    if (g_active_screen != NULL) {
        while (keyboard_char_get(&g_keyboard, &c) == 1) {
            screen_put_char(g_active_screen, c);
        }
    }
    
    // Send EOI to PIC1 (master PIC)
    PIC_sendEOI(1);
}
