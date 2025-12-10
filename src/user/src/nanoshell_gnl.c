#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"
#include "user_syscall.h"
#include "user_str_utils.h"

#define READ_SIZE      32
#define REMAINDER_SIZE READ_SIZE

USER_TEXT
uint32_t nanoshell_gnl(char *arr, uint32_t size)
{
    if (size == 0) return 0;

    static char  remainder[REMAINDER_SIZE] USER_DATA;
    static uint32_t rem_len USER_DATA = 0;

    static uint32_t pos USER_DATA = 0;
    static uint32_t max_pos USER_DATA = 0;

    uint32_t ret;
    char buf[READ_SIZE];
    char delim;
    uint32_t i;
    uint32_t delim_pos;
    uint32_t max_copy;
    uint32_t to_copy;
    uint32_t leftover;
    bool line_complete = 0;
    bool hit_eof = 0;

    if (pos == 0)
    {
        max_pos = size - 1;
    }

    // Process remainder first
    if (rem_len > 0) {
        delim = 0;
        for (i = 0; i < rem_len && remainder[i] != '\n' && remainder[i] != '\0'; ++i); // Scan for delimeter
        if (i < rem_len)
        {
            delim = remainder[i];
        }
        delim_pos = i;
        max_copy = (i < rem_len) ? (delim == '\n' ? i + 1 : i) : rem_len; // Max amount that can be copied from reminder
        to_copy = (max_copy < max_pos - pos) ? max_copy : max_pos - pos;  // Amount that will be copied to arr (limited by free arr size)

        user_memcpy(arr + pos, remainder, to_copy);
        user_syswrite(arr + pos, to_copy);
        pos += to_copy;

        user_memmove(remainder, remainder + to_copy, rem_len - to_copy); // Moves reminder of reminder to begginig of reminder.
        rem_len -= to_copy;

        if (i < rem_len)
        {
            line_complete = (delim == '\n');
            hit_eof       = (delim == '\0');
        }
        if (hit_eof)
        {
            rem_len = 0;
        }
        if (line_complete || hit_eof)
        {
            arr[pos] = '\0';
            ret = pos;
            pos = 0;
            return (ret);
        }

    }

    while (1)
    {
        uint32_t read_len = user_sysread(buf, READ_SIZE);
        if (read_len == 0) {
            arr[pos] = '\0';
            ret = 0;
            break;
        }

        delim = 0;
        for (i = 0; i < read_len && buf[i] != '\n' && buf[i] != '\0'; ++i); // Scan for delimeter
        if (i < read_len)
        {
            delim = buf[i];
        }

        delim_pos = i;
        max_copy = (i < read_len) ? (delim == '\n' ? i + 1 : i) : read_len;
        to_copy = (max_copy < max_pos - pos) ? max_copy : max_pos - pos;
        leftover = 0;

        if (to_copy != 0)
        {
            user_memcpy(arr + pos, buf, to_copy);
            user_syswrite(arr + pos, to_copy);
            pos += to_copy;
            leftover = (delim == '\n') ? read_len - delim_pos - 1 : 0;
        }

        if (leftover > 0) {
            if (leftover > REMAINDER_SIZE)
            {
                leftover = REMAINDER_SIZE;  // defensive, should never happen
            }
            user_memcpy(remainder, buf + to_copy, leftover);
            rem_len = leftover;
        }
        if (i < read_len)
        {
            line_complete = (delim == '\n');
            hit_eof       = (delim == '\0');
        }
        if (line_complete || hit_eof)
        {
            arr[pos] = '\0';
            ret = pos;
            pos = 0;
            break;
        }
    }
    return ret;
}