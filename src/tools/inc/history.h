#ifndef _HISTORY_H
#define _HISTORY_H

#include "start.h"
#include "inttype.h"
#include "str_utils.h"

#define HISTORY_WIDTH      SCREEN_WIDTH
#define HISTORY_HEIGHT     100u

typedef struct history_entry_s
{
    char command[HISTORY_WIDTH];
} history_entry_t;

typedef struct history_buffer_s
{
    history_entry_t entries[HISTORY_HEIGHT];
    uint32_t index;
} history_buffer_t;

/**
 * @brief   Clears the history buffer.
 *
 * @details Clears all entries in the history buffer by setting the first character
 *          of each command to null terminator and resetting the index to 0.
 *
 * @param[in,out] history_buffer Pointer to the history buffer to clear.
 */
void md_history_clear(history_buffer_t *history_buffer);

/**
 * @brief   Initializes the history buffer.
 *
 * @details Initializes the history buffer by clearing all entries.
 *
 * @param[in,out] history_buffer Pointer to the history buffer to initialize.
 */
void md_history_init(history_buffer_t *history_buffer);

/**
 * @brief   Adds a new entry to the history buffer.
 *
 * @details Adds a new command entry to the history buffer. Copies the entry string 
 *          character by character up to HISTORY_WIDTH - 1 characters. The buffer 
 *          operates as a circular buffer, wrapping around when full.
 *
 * @param[in,out] history_buffer Pointer to the history buffer.
 * @param[in]     entry          Pointer to the null-terminated string to add to history.
 */
void md_history_add_entry(history_buffer_t *history_buffer, const char *entry);

/**
 * @brief   Retrieves an entry from the history buffer by index.
 *
 * @details Retrieves a history entry by index. Returns a pointer to the command 
 *          string at the specified index.
 *
 * @param[in] history_buffer Pointer to the history buffer.
 * @param[in] index          The index of the history entry to retrieve.
 * 
 * @return Pointer to the null-terminated string of the history entry.
 */
char* md_history_get_entry(history_buffer_t *history_buffer, uint32_t index);

/**
 * @brief   Retrieves the index of the most recently added command.
 *
 * @details Gets the index of the most recently added command. Handles wrap-around 
 *          case when index is at 0 by returning HISTORY_HEIGHT - 1.
 *
 * @param[in] history_buffer Pointer to the history buffer.
 * 
 * @return The index of the last command in the history buffer.
 */ 
uint32_t md_history_get_last_command_index(history_buffer_t *history_buffer);

/**
 * @brief   Prints the history buffer to the screen.
 *
 * @details Prints the history buffer to screen, displaying up to SCREEN_HEIGHT entries.
 *          Handles circular buffer wrap-around to show the most recent commands.
 *          Calculates starting position for printing based on screen height. If 
 *          wrap-around is needed, prints from the end of the buffer first, then 
 *          from the beginning up to the last command.
 *
 * @param[in] history_buffer Pointer to the history buffer to print.
 */
void md_history_print(history_buffer_t *history_buffer);

#endif /* _HISTORY_H */