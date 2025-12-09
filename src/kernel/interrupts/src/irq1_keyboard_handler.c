#include "keyboard.h"
#include "io.h"
#include "screen.h"
#include "pic.h"
#include "kernel.h"


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
    // Process the scancode
    keyboard_run(&g_keyboard);
    
    // Send EOI to PIC1 (master PIC)
    PIC_sendEOI(1);
}
