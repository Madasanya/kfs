#include "history.h"

void md_history_clear(void)
{
    for (uint32_t i = 0; i < HISTORY_HEIGHT; i++)
    {
        // Clear each history entry if needed
    }
}

void md_history_init(void)
{
    history_buffer_t history_buffer = {0};
    md_history_clear();
    history_buffer.index = 0;
}

void md_history_add_entry(char *entry)
{
    // Add a new entry to the history buffer
}  

char* md_history_get_entry(uint32_t index)
{
    return history_buffer.entries[index];
}

void md_history_print(void)
{
    uint16_t pos_helper = history_buffer.index - (SCREEN_HEIGHT - 1);

    if (pos_helper < 0)
    {
        for (uint32_t i = HISTORY_HEIGHT + pos_helper; i <= HISTORY_HEIGHT - 1; i++)
        {
            md_put_str(md_history_get_entry(i));
        }
        for (uint32_t i = 0; i <= history_buffer.index; i++)
        {
            md_put_str(md_history_get_entry(i));
        }
    }
    else
    {
        for (uint32_t i = pos_helper; i <= history_buffer.index; i++)
        {
            md_put_str(md_history_get_entry(i));
        }
    }
}

