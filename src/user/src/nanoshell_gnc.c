#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"
#include "user_syscall.h"

#define GNC_SIZE 8

/**
 * @brief Increment circular buffer pointer.
 *
 * @details Increments the buffer pointer and wraps around to 0 when reaching
 *          GNC_SIZE, implementing circular buffer behavior.
 *
 * @param[in,out] ptr Pointer to the buffer index to increment.
 */
USER_TEXT
static void ptr_inc(uint8_t *ptr)
{
    (*ptr)++;
    if (*ptr >= GNC_SIZE)
    {
        *ptr = 0u;
    }
}

USER_TEXT
uint32_t nanoshell_gnc(uint8_t *comm)
{
    static uint8_t buffer[GNC_SIZE] = {0u};
    static uint8_t w_ptr = 0u;
    static uint8_t r_ptr = 0u;

    uint32_t ret = 0;

    if (w_ptr == r_ptr)
    {
        r_ptr = 0;
        w_ptr = user_syscommread(buffer, GNC_SIZE - 1);
    }

    if (w_ptr != r_ptr)
    {
        *comm = buffer[r_ptr];
        ptr_inc(&r_ptr);
        ret = 1;
    }

    return (ret);
}