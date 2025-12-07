#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"

#define STR_MAX_LEN 65535u

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