/**
 * @file keyboard_settings.c
 * @brief Keyboard configuration data and scancode mappings.
 * 
 * @details This file contains constant data arrays used by the keyboard driver,
 *          including scancode-to-ASCII mappings and special command bindings.
 *          Separating these from the driver logic improves maintainability and
 *          makes it easier to customize keyboard layouts or command mappings.
 */

#include "keyboard.h"

/* Scancode definitions for special commands */
#define KEYBOARD_SCANCODE_SCROLL_UP 0xE048u
#define KEYBOARD_SCANCODE_SCROLL_DOWN 0xE050u
#define KEYBOARD_SCANCODE_CHANGE_SCREEN 0x000Fu
#define KEYBOARD_SCANCODE_CHANGE_COLOR 0xE038u
#define KEYBOARD_SCANCODE_START_LOG_LVL1 0x003Bu
#define KEYBOARD_SCANCODE_START_LOG_LVL2 0x003Cu
#define KEYBOARD_SCANCODE_START_LOG_LVL3 0x003Du
#define KEYBOARD_SCANCODE_START_LOG_LVL4 0x003Eu
#define KEYBOARD_SCANCODE_START_LOG_LVL5 0x003Fu
#define KEYBOARD_SCANCODE_START_LOG_LVL6 0x0040u
#define KEYBOARD_SCANCODE_START_LOG_LVL7 0x0041u
#define KEYBOARD_SCANCODE_START_LOG_LVL8 0x0042u

/**
 * @brief Array mapping special keyboard scancodes to their corresponding commands.
 * 
 * @details This constant array associates predefined scancodes with special keyboard commands,
 * such as scrolling, changing screens, or displaying error logs at various levels.
 */
const keyboard_special_comm_t keyboard_comm_arr[] = {
    {KEYBOARD_SCANCODE_SCROLL_UP, KEYBOARD_COMM_SCROLL_UP},
    {KEYBOARD_SCANCODE_SCROLL_DOWN, KEYBOARD_COMM_SCROLL_DOWN},
    {KEYBOARD_SCANCODE_CHANGE_SCREEN, KEYBOARD_COMM_CHANGE_SCREEN},
    {KEYBOARD_SCANCODE_CHANGE_COLOR, KEYBOARD_COMM_CHANGE_COLOR},
    {KEYBOARD_SCANCODE_START_LOG_LVL1, KEYBOARD_COMM_START_LOG_LVL1},
    {KEYBOARD_SCANCODE_START_LOG_LVL2, KEYBOARD_COMM_START_LOG_LVL2},
    {KEYBOARD_SCANCODE_START_LOG_LVL3, KEYBOARD_COMM_START_LOG_LVL3},
    {KEYBOARD_SCANCODE_START_LOG_LVL4, KEYBOARD_COMM_START_LOG_LVL4},
    {KEYBOARD_SCANCODE_START_LOG_LVL5, KEYBOARD_COMM_START_LOG_LVL5},
    {KEYBOARD_SCANCODE_START_LOG_LVL6, KEYBOARD_COMM_START_LOG_LVL6},
    {KEYBOARD_SCANCODE_START_LOG_LVL7, KEYBOARD_COMM_START_LOG_LVL7},
    {KEYBOARD_SCANCODE_START_LOG_LVL8, KEYBOARD_COMM_START_LOG_LVL8}
};

/**
 * @brief Mapping of PS/2 Set 1 scancodes to unshifted ASCII characters.
 * 
 * @details This array maps keyboard scancodes to their corresponding unshifted characters
 * (e.g., lowercase letters, numbers without shift). Non-printable keys are mapped to 0.
 * Assumes Num Lock is on for keypad mappings.
 */
const char keyboard_unshifted_map[0x100] = {
    [0x00] = 0,
    [0x01] = 0, // Esc
    [0x02] = '1',
    [0x03] = '2',
    [0x04] = '3',
    [0x05] = '4',
    [0x06] = '5',
    [0x07] = '6',
    [0x08] = '7',
    [0x09] = '8',
    [0x0A] = '9',
    [0x0B] = '0',
    [0x0C] = '-',
    [0x0D] = '=',
    [0x0E] = 0, // Backspace
    [0x0F] = 0, // Tab
    [0x10] = 'q',
    [0x11] = 'w',
    [0x12] = 'e',
    [0x13] = 'r',
    [0x14] = 't',
    [0x15] = 'y',
    [0x16] = 'u',
    [0x17] = 'i',
    [0x18] = 'o',
    [0x19] = 'p',
    [0x1A] = '[',
    [0x1B] = ']',
    [0x1C] = '\n', // Enter
    [0x1D] = 0, // Left Ctrl
    [0x1E] = 'a',
    [0x1F] = 's',
    [0x20] = 'd',
    [0x21] = 'f',
    [0x22] = 'g',
    [0x23] = 'h',
    [0x24] = 'j',
    [0x25] = 'k',
    [0x26] = 'l',
    [0x27] = ';',
    [0x28] = '\'',
    [0x29] = '`',
    [0x2A] = 0, // Left Shift
    [0x2B] = '\\',
    [0x2C] = 'z',
    [0x2D] = 'x',
    [0x2E] = 'c',
    [0x2F] = 'v',
    [0x30] = 'b',
    [0x31] = 'n',
    [0x32] = 'm',
    [0x33] = ',',
    [0x34] = '.',
    [0x35] = '/',
    [0x36] = 0, // Right Shift
    [0x37] = '*', // Keypad *
    [0x38] = 0, // Left Alt
    [0x39] = ' ', // Space
    [0x3A] = 0, // Caps Lock
    [0x3B] = 0, // F1
    [0x3C] = 0, // F2
    [0x3D] = 0, // F3
    [0x3E] = 0, // F4
    [0x3F] = 0, // F5
    [0x40] = 0, // F6
    [0x41] = 0, // F7
    [0x42] = 0, // F8
    [0x43] = 0, // F9
    [0x44] = 0, // F10
    [0x45] = 0, // Num Lock
    [0x46] = 0, // Scroll Lock
    [0x47] = '7', // Keypad 7 (assuming Num Lock on)
    [0x48] = '8', // Keypad 8
    [0x49] = '9', // Keypad 9
    [0x4A] = '-', // Keypad -
    [0x4B] = '4', // Keypad 4
    [0x4C] = '5', // Keypad 5
    [0x4D] = '6', // Keypad 6
    [0x4E] = '+', // Keypad +
    [0x4F] = '1', // Keypad 1
    [0x50] = '2', // Keypad 2
    [0x51] = '3', // Keypad 3
    [0x52] = '0', // Keypad 0
    [0x53] = '.', // Keypad .
    [0x57] = 0, // F11
    [0x58] = 0, // F12
    [0xff] = 0
};

/**
 * @brief Mapping of PS/2 Set 1 scancodes to shifted ASCII characters.
 * 
 * @details This array maps keyboard scancodes to their corresponding shifted characters
 * (e.g., uppercase letters, symbols with shift). Non-printable keys are mapped to 0.
 * Keypad digits remain the same assuming Num Lock is on.
 */
const char keyboard_shifted_map[0x100] = {
    [0x00] = 0,
    [0x01] = 0,
    [0x02] = '!',
    [0x03] = '@',
    [0x04] = '#',
    [0x05] = '$',
    [0x06] = '%',
    [0x07] = '^',
    [0x08] = '&',
    [0x09] = '*',
    [0x0A] = '(',
    [0x0B] = ')',
    [0x0C] = '_',
    [0x0D] = '+',
    [0x0E] = 0, // Backspace
    [0x0F] = 0, // Tab
    [0x10] = 'Q',
    [0x11] = 'W',
    [0x12] = 'E',
    [0x13] = 'R',
    [0x14] = 'T',
    [0x15] = 'Y',
    [0x16] = 'U',
    [0x17] = 'I',
    [0x18] = 'O',
    [0x19] = 'P',
    [0x1A] = '{',
    [0x1B] = '}',
    [0x1C] = '\n',
    [0x1D] = 0,
    [0x1E] = 'A',
    [0x1F] = 'S',
    [0x20] = 'D',
    [0x21] = 'F',
    [0x22] = 'G',
    [0x23] = 'H',
    [0x24] = 'J',
    [0x25] = 'K',
    [0x26] = 'L',
    [0x27] = ':',
    [0x28] = '"',
    [0x29] = '~',
    [0x2A] = 0,
    [0x2B] = '|',
    [0x2C] = 'Z',
    [0x2D] = 'X',
    [0x2E] = 'C',
    [0x2F] = 'V',
    [0x30] = 'B',
    [0x31] = 'N',
    [0x32] = 'M',
    [0x33] = '<',
    [0x34] = '>',
    [0x35] = '?',
    [0x36] = 0,
    [0x37] = '*',
    [0x38] = 0,
    [0x39] = ' ',
    [0x3A] = 0,
    [0x3B] = 0,
    [0x3C] = 0,
    [0x3D] = 0,
    [0x3E] = 0,
    [0x3F] = 0,
    [0x40] = 0,
    [0x41] = 0,
    [0x42] = 0,
    [0x43] = 0,
    [0x44] = 0,
    [0x45] = 0,
    [0x46] = 0,
    [0x47] = '7', // Keypad digits remain the same with shift (assuming Num Lock on)
    [0x48] = '8',
    [0x49] = '9',
    [0x4A] = '-',
    [0x4B] = '4',
    [0x4C] = '5',
    [0x4D] = '6',
    [0x4E] = '+',
    [0x4F] = '1',
    [0x50] = '2',
    [0x51] = '3',
    [0x52] = '0',
    [0x53] = '.',
    [0x57] = 0,
    [0x58] = 0,
    [0xff] = 0
};
