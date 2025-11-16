#include "history.h"
#include "screen.h"

void history_clear(history_buffer_t *history_buffer)
{
    for (uint32_t i = 0; i < HISTORY_HEIGHT; i++)
    {
        history_buffer->entries[i].command[0] = '\0';
    }
    history_buffer->index = 0;
}

void history_init(history_buffer_t *history_buffer)
{
    history_clear(history_buffer);
}

void history_add_entry(history_buffer_t *history_buffer, const char *entry)
{
    uint32_t i;
    
    if(entry == NULL)
    {
        return;
    }

    for (i = 0; i < HISTORY_WIDTH - 1 && entry[i] != '\0'; i++)
    {
        history_buffer->entries[history_buffer->index].command[i] = entry[i];
    }
    history_buffer->entries[history_buffer->index].command[i] = '\0';

    history_buffer->index++;
    if (history_buffer->index >= HISTORY_HEIGHT)
    {
        history_buffer->index = 0;
    }
}  

char* history_get_entry(history_buffer_t *history_buffer, uint32_t index)
{
    return history_buffer->entries[index].command;
}

uint32_t history_get_last_command_index(history_buffer_t *history_buffer)
{
    if (history_buffer->index == 0)
    {
        return HISTORY_HEIGHT - 1;
    }
    return history_buffer->index - 1;
}