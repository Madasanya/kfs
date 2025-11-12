#include "history.h"

history_buffer_t history_buffer;

void md_history_clear(void)
{
    for (uint32_t i = 0; i < HISTORY_HEIGHT; i++)
    {
        history_buffer.entries[i].command[0] = '\0';
    }
    history_buffer.index = 0;
}

void md_history_init(void)
{
    md_history_clear();
}

void md_history_add_entry(const char *entry)
{
    uint32_t i;
    
    if(entry == NULL)
    {
        return;
    }

    for (i = 0; i < HISTORY_WIDTH - 1 && entry[i] != '\0'; i++)
    {
        history_buffer.entries[history_buffer.index].command[i] = entry[i];
    }
    history_buffer.entries[history_buffer.index].command[i] = '\0';

    history_buffer.index++;
    if (history_buffer.index >= HISTORY_HEIGHT)
    {
        history_buffer.index = 0;
    }
}  

char* md_history_get_entry(uint32_t index)
{
    return history_buffer.entries[index].command;
}

uint32_t md_history_get_last_command_index()
{
    if (history_buffer.index == 0)
    {
        return HISTORY_HEIGHT - 1;
    }
    return history_buffer.index - 1;
}

void md_history_print(void)
{
    int16_t pos_helper = md_history_get_last_command_index() - (SCREEN_HEIGHT - 1);

    if (pos_helper < 0)
    {
        for (uint32_t i = HISTORY_HEIGHT + pos_helper; i <= HISTORY_HEIGHT - 1; i++)
        {
            md_put_str(md_history_get_entry(i));
        }
        for (uint32_t i = 0; i <= md_history_get_last_command_index(); i++)
        {
            md_put_str(md_history_get_entry(i));
        }
    }
    else
    {
        for (uint32_t i = pos_helper; i <= md_history_get_last_command_index(); i++)
        {
            md_put_str(md_history_get_entry(i));
        }
    }
}