#include "keyboard.h"
#include "io.h"

#define PS2_DATA 0x60
#define PS2_STATUS 0x64
#define PS2_FULL_FLAG 0x1u

#define KEYBOARD_FLG_SHIFT_LEFT 0x01u
#define KEYBOARD_FLG_SHIFT_RIGHT 0x02u
#define KEYBOARD_FLG_E0 0x04u

#define KEYBOARD_CHK_SHIFT_FLG 0x03u
#define KEYBOARD_CHK_ASCII_SCANCODE 0xFF00u

#define KEYBOARD_SCANCODE_PRESS_SHIFT_LEFT 0x002Au
#define KEYBOARD_SCANCODE_PRESS_SHIFT_RIGHT 0x0036u
#define KEYBOARD_SCANCODE_RELE_SHIFT_LEFT 0x00AAu
#define KEYBOARD_SCANCODE_RELE_SHIFT_RIGHT 0x00B6u
#define KEYBOARD_SCANCODE_E0 0x00E0u

/**
 * @brief Handles special scancodes for modifier keys like Shift and E0 prefix.
 * 
 * @details
 * This function processes scancodes related to Shift key presses and releases,
 * updating the keyboard flags accordingly. It also handles the E0 prefix by
 * recursively calling keyboard_run to process the next byte.
 * 
 * @param[in,out] keyboard Pointer to the keyboard structure.
 * @param[in] scancode The 16-bit scancode to handle.
 * @return 0 if the scancode was handled as special, 1 otherwise.
 */
static uint8_t special_scancode_handler(keyboard_t *keyboard, uint16_t scancode)
{
    uint8_t ret = 0u;
    switch (scancode)
    {
    case KEYBOARD_SCANCODE_PRESS_SHIFT_LEFT:
        keyboard->flags |= KEYBOARD_FLG_SHIFT_LEFT;  // Set left shift flag on press
        break;
    case KEYBOARD_SCANCODE_PRESS_SHIFT_RIGHT:
        keyboard->flags |= KEYBOARD_FLG_SHIFT_RIGHT;  // Set right shift flag on press
        break;
    case KEYBOARD_SCANCODE_RELE_SHIFT_LEFT:
        keyboard->flags &= ~KEYBOARD_FLG_SHIFT_LEFT;  // Clear left shift flag on release
        break;
    case KEYBOARD_SCANCODE_RELE_SHIFT_RIGHT:
        keyboard->flags &= ~KEYBOARD_FLG_SHIFT_RIGHT;  // Clear right shift flag on release
        break;
    case KEYBOARD_SCANCODE_E0:
        keyboard->flags |= KEYBOARD_FLG_E0;  // Set E0 flag for extended scancodes (next scancode will be extended)
        break;
    default:
        ret = 1u;  // Not a special scancode
        break;
    }
    return ret;
}

/**
 * @brief Handles ASCII-mappable scancodes, converting them to characters.
 * 
 * @details
 * This function checks if the scancode corresponds to an ASCII character,
 * considering the current shift state. If valid, it queues the character
 * in the keyboard's character buffer.
 * 
 * @param[in,out] keyboard Pointer to the keyboard structure.
 * @param[in] scancode The 16-bit scancode to handle.
 * @return 0 if the scancode was handled as ASCII (and queued), 1 otherwise.
 */
static uint8_t ascii_scancode_handler(keyboard_t *keyboard, uint16_t scancode)
{
    uint8_t ret = 1u;
    char new_char;
    if ((scancode & KEYBOARD_CHK_ASCII_SCANCODE) == 0u)  // Check if it's a make code (not break/extended)
    {
        if ((keyboard->flags & KEYBOARD_CHK_SHIFT_FLG) == 0u)
        {
            new_char = keyboard_unshifted_map[(uint8_t)scancode];  // Use unshifted map if no shift
        }
        else
        {
            new_char = keyboard_shifted_map[(uint8_t)scancode];  // Use shifted map if shift is held
        }
        if (new_char != '\0')  // If a valid character
        {
            keyboard->last_chars[keyboard->next_char_write_idx] = new_char;  // Queue the character
            keyboard->next_char_write_idx = (keyboard->next_char_write_idx + 1) % KEYBOARD_CHAR_ARR_LEN;  // Wrap around if needed
            if (keyboard->next_char_write_idx == keyboard->next_char_read_idx)  // Handle buffer full (overwrite oldest)
            {
                keyboard->next_char_read_idx = (keyboard->next_char_read_idx + 1) % KEYBOARD_CHAR_ARR_LEN;
            }
            ret = 0u;  // Successfully handled
        }
    }
    return ret;
}

/**
 * @brief Handles special command scancodes.
 * 
 * @details
 * This function checks if the scancode matches any special command in the
 * keyboard_comm_arr and queues the corresponding command if found.
 * 
 * @param[in,out] keyboard Pointer to the keyboard structure.
 * @param[in] scancode The 16-bit scancode to handle.
 * 
 * @return 0 if the scancode was handled as a command (and queued), 1 otherwise.
 */
static uint8_t comm_scancode_handler(keyboard_t *keyboard, uint16_t scancode)
{
    uint8_t ret = 1u;
    for (uint8_t i = 0; i < KEYBOARD_SPECIAL_COMM_ARR_LEN; i++)  // Iterate through command array
    {
        if (scancode == (keyboard_comm_arr[i]).scancode)  // Match found
        {
            keyboard->last_comms[keyboard->next_comm_write_idx] = (keyboard_comm_arr[i]).command;  // Queue the command
            keyboard->next_comm_write_idx = (keyboard->next_comm_write_idx + 1) % KEYBOARD_COMM_ARR_LEN;  // Wrap around
            if (keyboard->next_comm_write_idx == keyboard->next_comm_read_idx)  // Handle buffer full
            {
                keyboard->next_comm_read_idx = (keyboard->next_comm_read_idx + 1) % KEYBOARD_COMM_ARR_LEN;
            }
            ret = 0u;  // Successfully handled
        }
    }
    return ret;
}

void keyboard_run(keyboard_t *keyboard)
{
    if (keyboard == NULL)
    {
        return;
    }
    
    uint8_t ret;
    uint16_t scancode_full = 0u;
    
    if ((keyboard->flags & KEYBOARD_FLG_E0) != 0u)  // If E0 flag is set, prefix with 0xE0
    {
        scancode_full = 0xE000u;
        keyboard->flags &= ~KEYBOARD_FLG_E0;  // Clear E0 flag after use
    }
    
    scancode_full |= (uint16_t)md_inb(PS2_DATA);  // Combine with prefix if any
    ret = special_scancode_handler(keyboard, scancode_full);  // Try special handler first
    if (ret != 0u)
    {
        ret = ascii_scancode_handler(keyboard, scancode_full);  // Then ASCII if not special
    }
    if(ret != 0)
    {
        ret = comm_scancode_handler(keyboard, scancode_full);  // Finally commands if neither
    }
    
    return;
}

uint8_t keyboard_char_get(keyboard_t *keyboard, char *c)
{
    if (keyboard == NULL || c == NULL)
    {
        return 0u;
    }
    
    uint8_t ret = 0u;
    if (keyboard->next_char_read_idx != keyboard->next_char_write_idx)  // Buffer not empty
    {
        ret = 1u;
        *c = keyboard->last_chars[keyboard->next_char_read_idx];  // Get the character
        keyboard->next_char_read_idx = (keyboard->next_char_read_idx + 1) % KEYBOARD_CHAR_ARR_LEN;  // Wrap around
    }
    return ret;
}

uint8_t keyboard_chararr_get(keyboard_t *keyboard, char *arr_c, uint8_t arr_len)
{
    if (keyboard == NULL || arr_c == NULL)
    {
        return 0u;
    }
    
    uint8_t ret = 0u;
    while ((keyboard->next_char_read_idx != keyboard->next_char_write_idx) && (ret < arr_len))  // While buffer has data and space in array
    {
        ret++;
        *arr_c = keyboard->last_chars[keyboard->next_char_read_idx];  // Get character
        arr_c++;  // Move to next array slot
        keyboard->next_char_read_idx = (keyboard->next_char_read_idx + 1) % KEYBOARD_CHAR_ARR_LEN;  // Wrap around
    }
    return ret;
}

uint8_t keyboard_comm_get(keyboard_t *keyboard, uint8_t *comm)
{
    if (keyboard == NULL || comm == NULL)
    {
        return 0u;
    }
    
    uint8_t ret = 0u;
    if (keyboard->next_comm_read_idx != keyboard->next_comm_write_idx)  // Buffer not empty
    {
        ret = 1u;
        *comm = keyboard->last_comms[keyboard->next_comm_read_idx];  // Get the command
        keyboard->next_comm_read_idx = (keyboard->next_comm_read_idx + 1) % KEYBOARD_COMM_ARR_LEN;  // Wrap around
    }
    return ret;
}

uint8_t keyboard_commarr_get(keyboard_t *keyboard, uint8_t *arr_comm, uint8_t arr_len)
{
    if (keyboard == NULL || arr_comm == NULL)
    {
        return 0u;
    }
    
    uint8_t ret = 0u;
    while ((keyboard->next_comm_read_idx != keyboard->next_comm_write_idx) && (ret < arr_len))  // While buffer has data and space in array
    {
        ret++;
        *arr_comm = keyboard->last_comms[keyboard->next_comm_read_idx];  // Get command
        arr_comm++;  // Move to next array slot
        keyboard->next_comm_read_idx = (keyboard->next_comm_read_idx + 1) % KEYBOARD_COMM_ARR_LEN;  // Wrap around
    }
    return ret;
}
