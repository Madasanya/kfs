#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"
#include "user_syscall.h"
#include "user_str_utils.h"

#define NUM_COLORS 5
#define USER_COLOR_MODE_SET 0u

USER_TEXT
uint8_t nanoshell_color_switch(char *num_str)
{
    if (num_str == NULL)
    {
        return 1; // Invalid argument
    }

    uint8_t num = 0;
    if (user_strlen(num_str) == 0u)
    {
        return 1; // Invalid argument
    }

    // Convert string to integer level
    for (uint16_t i = 0; num_str[i] != '\0' && num_str[i] != '\n'; i++)
    {
        if ((num_str[i] < '0') || (num_str[i] > '9'))
        {
            return 1; // Invalid character
        }
        num = (num * 10u) + (uint32_t)(num_str[i] - '0');
    }

    if (num >= NUM_COLORS)
    {
        return 1; // Invalid color number
    }
    user_syscolorset(USER_COLOR_MODE_SET, num);
    return 0; // Return success
}