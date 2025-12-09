#include "history.h"
#include "printk.h"

void history_clear(history_buffer_t *history_buffer)
{
    if (history_buffer == NULL)
    {
        return;
    }
    
    for (uint32_t i = 0; i < HISTORY_HEIGHT; i++)
    {
        history_buffer->entries[i][0] = 0;
    }
    history_buffer->index = 0;
    history_buffer->first_index = 0;
}

void history_init(history_buffer_t *history_buffer)
{
    if (history_buffer == NULL)
    {
        return;
    }
    
    md_printk(KERN_INFO "Initializing history buffer at %ph\n", history_buffer);
    history_clear(history_buffer);
}

void history_add_entry(history_buffer_t *history_buffer, const uint16_t *entry)
{
    if(history_buffer == NULL || entry == NULL)
    {
        return;
    }

    for (uint32_t i = 0; i < HISTORY_WIDTH; i++)
    {
        history_buffer->entries[history_buffer->index][i] = entry[i];
    }

    history_buffer->index++;
    if (history_buffer->index >= HISTORY_HEIGHT)
    {
        history_buffer->index = 0;
    }
    if (history_buffer->index == history_buffer->first_index)
    {
        history_buffer->first_index++;
        if (history_buffer->first_index >= HISTORY_HEIGHT)
        {
            history_buffer->first_index = 0;
        }
    }
}  

uint16_t* history_get_entry(history_buffer_t *history_buffer, uint32_t index)
{
    if (history_buffer == NULL)
    {
        return NULL;
    }
    
    return history_buffer->entries[index];
}

uint32_t history_get_last_entry_index(history_buffer_t *history_buffer)
{
    if (history_buffer == NULL)
    {
        return 0;
    }
    
    if (history_buffer->index == 0)
    {
        return HISTORY_HEIGHT - 1;
    }
    return history_buffer->index - 1;
}

uint32_t history_get_first_entry_index(history_buffer_t *history_buffer)
{
    if (history_buffer == NULL)
    {
        return 0;
    }
    
    return history_buffer->first_index;
}

uint32_t history_get_num_of_entries(history_buffer_t *history_buffer)
{
    if (history_buffer == NULL)
    {
        return 0;
    }
    
    uint32_t ret;

    if (history_buffer->index >= history_buffer->first_index)
    {
        ret = history_buffer->index - history_buffer->first_index;
    }
    else
    {
        ret =  history_buffer->index + (HISTORY_HEIGHT - history_buffer->first_index);
    }

    return (ret);
}

void history_last_entry_remove(history_buffer_t *history_buffer)
{
    if (history_buffer == NULL)
    {
        return;
    }
    
    if (history_buffer->first_index != history_buffer->index)
    {
        if(history_buffer->index == 0)
        {
            history_buffer->index = HISTORY_HEIGHT - 1;
        }
        else
        {
            history_buffer->index--;
        }
    }
}