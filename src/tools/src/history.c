#include "history.h"

void md_history_clear(void)
{
    for (uint32_t i = 0; i < HISTORY_HEIGHT; i++)
    {
        history_buffer.entries[i].command[0] = '\0';
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
    history_buffer.index++;
    if (HISTORY_HEIGHT % history_buffer.index == 0 && history_buffer.index != 0)
    {
        history_buffer.index = 0;
    }
    for (uint32_t i = 0; i < HISTORY_WIDTH - 1 && entry[i] != '\0'; i++)
    {
        history_buffer.entries[history_buffer.index].command[i] = entry[i];
        history_buffer.entries[history_buffer.index].command[i + 1] = '\0';
    }
}  

char* md_history_get_entry(uint32_t index)
{
    return history_buffer.entries[index].command;
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

