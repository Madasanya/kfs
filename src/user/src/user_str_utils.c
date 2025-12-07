#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"
#include "user_str_utils.h"

USER_TEXT
uint16_t user_strlen(const char* str)
{
    uint16_t ret = 0;
    while (*str != '\0')
    {
        if (ret == STR_MAX_LEN)
        {
            break;
        }
        ret++;
        str++;
    }
    return(ret);
}

USER_TEXT
static uint8_t hexstr_to_val(uint32_t *val, const char* str)
{
    uint8_t i = 2; // Skip "0x"
    while (str[i] != '\0')
    {
        char c = str[i];
        uint8_t digit;
        
        if (c >= '0' && c <= '9')
            digit = c - '0';
        else if (c >= 'a' && c <= 'f')
            digit = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F')
            digit = c - 'A' + 10;
        else
            return 1;
        
        *val = (*val << 4) | digit;
        i++;
    }
    return 0;
}

USER_TEXT
static uint8_t decstr_to_val(uint32_t *val, const char* str)
{
    uint8_t i = 0;
    while (str[i] != '\0')
    {
        char c = str[i];
        if (c >= '0' && c <= '9')
        {
            uint8_t digit = c - '0';
            *val = *val * 10 + digit;
        }
        else
        {
            return 1;
        }
        i++;
    }
    return 0;
}

USER_TEXT
uint8_t user_atoi_base(uint32_t *val, const char* str, uint8_t base)
{
    if (str == NULL || (base != DECIMAL_BASE && base != HEX_BASE))
    {
        return 1;
    }

    switch (base)
    {
    case DECIMAL_BASE:
        decstr_to_val(val, str);
        break;
    case HEX_BASE:
        hexstr_to_val(val, str);
        break;
    default:
        break;
    }
    return 0;
}

void *user_memcpy(void *destination, const void *source, uint32_t byte_count)
{
    uint8_t       *dest = (uint8_t *)destination;
    const uint8_t *src  = (const uint8_t *)source;

    for (uint32_t index = 0; index < byte_count; ++index) {
        dest[index] = src[index];
    }

    return destination;
}

USER_TEXT
void *user_memmove(void *destination, const void *source, uint32_t byte_count)
{
    uint8_t       *dest = (uint8_t *)destination;
    const uint8_t *src  = (const uint8_t *)source;

    /* Fast path: nothing to do */
    if (byte_count == 0 || destination == source) {
        return destination;
    }

    /* Destination is before source → safe to copy forward */
    if (dest < src) {
        for (uint32_t index = 0; index < byte_count; ++index) {
            dest[index] = src[index];
        }
    }
    /* Destination after source → copy backward to avoid overwriting data */
    else {
        for (uint32_t index = byte_count; index > 0; --index) {
            dest[index - 1] = src[index - 1];
        }
    }

    return destination;
}