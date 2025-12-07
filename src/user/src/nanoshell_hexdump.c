#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"
#include "user_syscall.h"
#include "user_str_utils.h"

#define MAX_ADDR_STR_LEN 18u // "0x" + 16 hex digits
#define MAX_LEN_STR_LEN 3u  // Max 3 digits for length to prevent history overflow

USER_TEXT
uint8_t nanoshell_hexdump(char *arg_str)
{
    if (arg_str == NULL)
    {
        return 1;
    }

    char *addr_str = arg_str;
    char *len_str = NULL;
    
    uint32_t i = 0;
    while (addr_str[i] != '\0' && addr_str[i] != ' ')
    {
        i++;
    }

    if (i < 3 || i > MAX_ADDR_STR_LEN)
    {
        return 1;
    }

    if (addr_str[i] == ' ')
    {
        addr_str[i] = '\0';
        len_str = &(addr_str[i + 1]);
    }
    else
    {
        return 1;
    }
    
    // Parse hex address from addr_str
    uint32_t addr = 0;
    if (addr_str[0] == '0' && (addr_str[1] == 'x' || addr_str[1] == 'X'))
    {
        user_atoi_base(&addr, addr_str, HEX_BASE);
    }
    else
    {
        return 1;
    }

    // Parse decimal length from len_str
    uint32_t len = 0;
    if (len_str != NULL)
    {
        if (user_strlen(len_str) > MAX_LEN_STR_LEN) // Arbitrary limit to prevent history overflow
        {
            return 1;
        }
        user_atoi_base(&len, len_str, DECIMAL_BASE);
    }
    
    user_syshexdump((void *)addr, len);
    return 0;
}