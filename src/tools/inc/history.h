#ifndef _HISTORY_H
#define _HISTORY_H

#include "inttype.h"
#include "kernel.h"

#define HISTORY_WIDTH      SCREEN_WIDTH + 1
#define HISTORY_HEIGHT     100u

typedef struct history_entry_s
{
    uint16_t command[HISTORY_WIDTH];
} history_entry_t;

typedef struct history_buffer_s
{
    history_entry_t entries[HISTORY_HEIGHT];
    uint32_t index;
} history_buffer_t;

/**
 * @brief   Clears the history buffer.
 *
 * @details Clears all entries in the history buffer by zeroing out the first
 *          colored character value and resetting the index to 0.
 *
 * @param[in,out] history_buffer Pointer to the history buffer to clear.
 */
void history_clear(history_buffer_t *history_buffer);

/**
 * @brief   Initializes the history buffer.
 *
 * @details Initializes the history buffer by clearing all entries.
 *
 * @param[in,out] history_buffer Pointer to the history buffer to initialize.
 */
void history_init(history_buffer_t *history_buffer);

/**
 * @brief   Adds a new entry to the history buffer.
 *
 * @details Adds a new screen line to the history buffer. Copies the pre-computed 
 *          colored character values for the entire screen width. The buffer 
 *          operates as a circular buffer, wrapping around when full.
 *
 * @param[in,out] history_buffer Pointer to the history buffer.
 * @param[in]     entry          Pointer to the colored character buffer (uint16_t array).
 */
void history_add_entry(history_buffer_t *history_buffer, const uint16_t *entry);

/**
 * @brief   Retrieves an entry from the history buffer by index.
 *
 * @details Retrieves a history entry by index. Returns a pointer to the colored 
 *          character buffer at the specified index.
 *
 * @param[in] history_buffer Pointer to the history buffer.
 * @param[in] index          The index of the history entry to retrieve.
 * 
 * @return Pointer to the colored character buffer (uint16_t array).
 */
uint16_t* history_get_entry(history_buffer_t *history_buffer, uint32_t index);

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
uint32_t history_get_last_command_index(history_buffer_t *history_buffer);

#endif /* _HISTORY_H */