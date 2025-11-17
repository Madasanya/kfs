/**
 * @file keyboard.h
 * @brief PS/2 Keyboard Driver
 * 
 * @details
 * This driver provides support for handling input from a PS/2 keyboard. It processes scancodes received
 * from the keyboard hardware, maps them to ASCII characters or special commands, and maintains circular
 * buffers for queued characters and commands. The driver supports shift modifiers for uppercase/lowercase
 * and symbol mapping, extended scancodes (E0 prefix), and a set of predefined special commands for actions
 * like scrolling, screen/color changes, and logging controls.
 * 
 * Key Features:
 * - Handles PS/2 Set 1 scancodes.
 * - Maps printable keys to ASCII characters, considering shift state.
 * - Detects and handles modifier keys (left/right shift).
 * - Supports extended scancodes for keys like arrow keys.
 * - Queues characters and commands in fixed-size circular buffers (length defined by KEYBOARD_CHAR_ARR_LEN
 *   and KEYBOARD_COMM_ARR_LEN).
 * - Provides functions to poll for new input and retrieve queued data.
 * - Special commands are mapped via a configurable array (keyboard_comm_arr).
 * - Assumes Num Lock is on for numeric keypad mappings.
 * 
 * Limitations:
 * - Does not handle Caps Lock, Num Lock, or Scroll Lock toggles (mappings assume Num Lock on, no Caps Lock support).
 * - Ignores break codes (key releases) except for shifts.
 * - No support for other modifiers like Ctrl, Alt.
 * - Buffers are fixed-size; overflow overwrites oldest entries.
 * - Designed for polling (e.g., in a main loop); no built-in interrupt setup.
 * - Keypad shifted mappings remain numeric (no Home/End/etc. support without Num Lock handling).
 * 
 * Usage Guide:
 * 1. Include this header: #include "keyboard.h"
 * 2. Declare and initialize a keyboard_t structure:
 *    keyboard_t kb = {0};  // Zero-initialize all fields
 * 3. In your main loop call keyboard_run(&kb) to process any available scancodes.
 *    - This should be called frequently to avoid missing inputs.
 * 4. To retrieve input:
 *    - For characters: Use keyboard_char_get(&kb, &c) or keyboard_chararr_get(&kb, arr, len)
 *      - Returns 1 (or count) if data available, 0 otherwise.
 *    - For commands: Use keyboard_comm_get(&kb, &comm) or keyboard_commarr_get(&kb, arr, len)
 *      - Commands are uint8_t values from KEYBOARD_COMM_* defines.
 * 5. Special commands:
 *    - Defined in keyboard_comm_arr (can be customized if needed).
 *    - Examples: F1-F8 for log levels, Esc for stop logging, Tab for screen change, etc.
 * 6. Flags:
 *    - Accessed via kb.flags.
 *    - Bits: SHIFT_LEFT (0x01), SHIFT_RIGHT (0x02), E0 (0x04 for extended code handling).
 * 7. Customization:
 *    - Modify keyboard_unshifted_map and keyboard_shifted_map for different layouts.
 *    - Extend keyboard_comm_arr for more commands (update KEYBOARD_SPECIAL_COMM_ARR_LEN).
 * 8. Dependencies:
 *    - "io.h" for md_inb() and md_io_wait() (I/O port access).
 *    - Assumes x86 architecture for PS/2 ports (0x60 data, 0x64 status).
 * 
 * Error Handling:
 * - Functions return 0 on no data or unhandled scancode.
 * - Buffer overflow: Oldest entry is discarded (circular buffer with overwrite).
 * 
 * Example:
 * @code
 * #include "keyboard.h"
 * 
 * int main() {
 *     keyboard_t kb = {0};
 *     char c;
 *     uint8_t comm;
 * 
 *     while (1) {
 *         keyboard_run(&kb);
 *         if (keyboard_char_get(&kb, &c))
 *         {
 *             // Process character c
 *         }
 *         if (keyboard_comm_get(&kb, &comm))
 *         {
 *             // Process command comm (e.g., switch (comm) { ... })
 *         }
 *     }
 *     return 0;
 * }
 * @endcode
 * 
 * For more details on individual functions and structures, see the documentation below.
 */

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "inttype.h"

#define KEYBOARD_CHAR_ARR_LEN 32u  /**< Length of the character buffer array. */
#define KEYBOARD_COMM_ARR_LEN 32u  /**< Length of the command buffer array. */
#define KEYBOARD_SPECIAL_COMM_ARR_LEN 12  /**< Number of special command mappings. */

/** 
 * @name Keyboard Command Codes
 * 
 * @brief Define the command codes for special keyboard actions.
 */
#define KEYBOARD_COMM_SCROLL_UP 0x01u         /**< Command to scroll up. */
#define KEYBOARD_COMM_SCROLL_DOWN 0x02u       /**< Command to scroll down. */
#define KEYBOARD_COMM_CHANGE_SCREEN 0x03u     /**< Command to change screen. */
#define KEYBOARD_COMM_CHANGE_COLOR 0x04u      /**< Command to change color. */
#define KEYBOARD_COMM_START_LOG_LVL1 0x10u    /**< Start logging at level 1. */
#define KEYBOARD_COMM_START_LOG_LVL2 0x11u    /**< Start logging at level 2. */
#define KEYBOARD_COMM_START_LOG_LVL3 0x12u    /**< Start logging at level 3. */
#define KEYBOARD_COMM_START_LOG_LVL4 0x13u    /**< Start logging at level 4. */
#define KEYBOARD_COMM_START_LOG_LVL5 0x14u    /**< Start logging at level 5. */
#define KEYBOARD_COMM_START_LOG_LVL6 0x15u    /**< Start logging at level 6. */
#define KEYBOARD_COMM_START_LOG_LVL7 0x16u    /**< Start logging at level 7. */
#define KEYBOARD_COMM_START_LOG_LVL8 0x17u    /**< Start logging at level 8. */
#define KEYBOARD_COMM_STOP_LOG_ALLLVL 0x18u   /**< Stop logging at all levels. */

#if KEYBOARD_CHAR_ARR_LEN >= 0xff
#error "Invalid KEYBOARD_CHAR_ARR_LEN"  // Ensure buffer length fits in uint8_t
#endif
#if KEYBOARD_COMM_ARR_LEN >= 0xff
#error "Invalid KEYBOARD_COMM_ARR_LEN"  // Ensure buffer length fits in uint8_t
#endif

/**
 * @brief Keyboard state structure.
 * 
 * @details This structure holds the state of the keyboard, including buffers for
 * characters and commands, flags for modifiers, and indices for reading/writing
 * to the circular buffers.
 */
typedef struct
{
    char last_chars[KEYBOARD_CHAR_ARR_LEN];         /**< Buffer for recently input characters. */
    uint8_t flags;                                  /**< Bit flags for keyboard state (e.g., shift, E0). */
    uint8_t last_comms[KEYBOARD_COMM_ARR_LEN];      /**< Buffer for recently input commands. */
    uint8_t next_char_write_idx;                    /**< Next write index for character buffer (circular). */
    uint8_t next_comm_write_idx;                    /**< Next write index for command buffer (circular). */
    uint8_t next_char_read_idx;                     /**< Next read index for character buffer (circular). */
    uint8_t next_comm_read_idx;                     /**< Next read index for command buffer (circular). */
} keyboard_t;

/**
 * @brief Special command mapping structure.
 * 
 * @details This structure maps a keyboard scancode to a corresponding special command.
 */
typedef struct
{
    uint16_t scancode;  /**< The scancode associated with the command. */
    uint8_t command;    /**< The command code triggered by the scancode. */
} keyboard_special_comm_t;

/**
 * @brief Processes incoming keyboard data from the PS/2 port.
 * 
 * @details
 * This function checks the PS/2 status register for available data. If data is present,
 * it reads the scancode, handles extended codes if necessary, and delegates to
 * special, ASCII, or command handlers in sequence.
 * 
 * @note This function shell be called often.
 * 
 * @param[in,out] keyboard Pointer to the keyboard structure.
 */
void keyboard_run(keyboard_t *keyboard);

/**
 * @brief Retrieves a single queued character from the keyboard buffer.
 * 
 * @details
 * Dequeues the next available character if any, updating the read index.
 * 
 * @param[in,out] keyboard Pointer to the keyboard structure.
 * @param[out] c Pointer to store the retrieved character.
 * @return 1 if a character was retrieved, 0 if the buffer is empty.
 */
uint8_t keyboard_char_get(keyboard_t *keyboard, char *c);

/**
 * @brief Retrieves multiple queued characters from the keyboard buffer.
 * 
 * @details Dequeues up to arr_len characters into the provided array, updating the read index.
 * 
 * @param[in,out] keyboard Pointer to the keyboard structure.
 * @param[out] arr_c Pointer to the array to store retrieved characters.
 * @param[in] arr_len Maximum number of characters to retrieve.
 * @return Number of characters actually retrieved (0 to arr_len).
 */
uint8_t keyboard_chararr_get(keyboard_t *keyboard, char *arr_c, uint8_t arr_len);

/**
 * @brief Retrieves a single queued command from the keyboard buffer.
 * 
 * @details
 * Dequeues the next available command if any, updating the read index.
 * 
 * @param[in,out] keyboard Pointer to the keyboard structure.
 * @param[out] comm Pointer to store the retrieved command.
 * @return 1 if a command was retrieved, 0 if the buffer is empty.
 */
uint8_t keyboard_comm_get(keyboard_t *keyboard, uint8_t *comm);

/**
 * @brief Retrieves multiple queued commands from the keyboard buffer.
 * 
 * @details
 * Dequeues up to arr_len commands into the provided array, updating the read index.
 * 
 * @param[in,out] keyboard Pointer to the keyboard structure.
 * @param[out] arr_comm Pointer to the array to store retrieved commands.
 * @param[in] arr_len Maximum number of commands to retrieve.
 * 
 * @return Number of commands actually retrieved (0 to arr_len).
 */
uint8_t keyboard_commarr_get(keyboard_t *keyboard, uint8_t *arr_comm, uint8_t arr_len);

#endif /* _KEYBOARD_H_ */
