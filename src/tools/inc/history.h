#ifndef _HISTORY_H
#define _HISTORY_H

#define HISTORY_HEIGHT     100u

/**
 * @brief   Clears the history buffer.
 *
 * @details This function iterates through the buffer and sets the entries to an initial state.
 */
void md_history_clear(void);

/**
 * @brief   Initializes the history buffer.
 *
 * @details This function sets up the history buffer for use, preparing any necessary structures.
 */
void md_history_init(void);

/**
 * @brief   Adds a new entry to the history buffer.
 *
 * @details This function appends a new entry to the history buffer.
 *
 * @param[in] entry Pointer to the null-terminated string to add to history.
 */
void md_history_add_entry(char *entry);

/**
 * @brief   Retrieves an entry from the history buffer by index.
 *
 * @details This function returns a pointer to the history entry at the specified index.
 *
 * @param[in] index The index of the history entry to retrieve.
 * 
 * @return Pointer to the null-terminated string of the history entry.
 */
char* md_history_get_entry(uint32_t index);

/**
 * @brief   Prints the history buffer to the screen.
 *
 * @details This function iterates through the history buffer and outputs each entry taking into consideration the SCREEN_HEIGHT.
 */
void md_history_print(void);

#endif /* _HISTORY_H */