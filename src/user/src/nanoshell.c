#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"
#include "nanoshell_builtins.h"
#include "user_syscall.h"
#include "errlog.h"
#include "user_str_utils.h"
#include "nanoshell_utils.h"
#include "hexdump.h"

#define SHELL_NAME "nanoshell> "
#define USER_SEGMENT_BASE 0x200000

/**
 * @brief Handle unknown command input.
 *
 * @details Displays "Unknown command" message to both the screen and error log
 *          when the user enters an unrecognized command.
 *
 * @param[in] str Command string (unused, kept for consistency).
 * @return Always returns 0.
 */
USER_TEXT
static unsigned char wrong_comm(char *str __attribute__((unused)))
{
    user_syswrite("Unknown command\n", user_strlen("Unknown command\n"));
    user_syserrwrite("Unknown command\n", user_strlen("Unknown command\n"), (uint32_t)ERRLOG_LVL_WARNING);
    return (0);
}

USER_DATA static const char *command_list[8] = {
    (const char *)((uint32_t)"_" - USER_SEGMENT_BASE),
    (const char *)((uint32_t)"echo" - USER_SEGMENT_BASE),
    (const char *)((uint32_t)"report" - USER_SEGMENT_BASE),
    (const char *)((uint32_t)"errlog" - USER_SEGMENT_BASE),
    (const char *)((uint32_t)"screen" - USER_SEGMENT_BASE),
    (const char *)((uint32_t)"color" - USER_SEGMENT_BASE),
    (const char *)((uint32_t)"hexdump" - USER_SEGMENT_BASE),
    (const char *)((uint32_t)"print-kernel-stack-thingy" - USER_SEGMENT_BASE)
};

USER_DATA static uint8_t (*command_funcs[8])(char *) = {
    (uint8_t (*)(char *))((uint32_t)wrong_comm - USER_SEGMENT_BASE),
    (uint8_t (*)(char *))((uint32_t)nanoshell_echo - USER_SEGMENT_BASE),
    (uint8_t (*)(char *))((uint32_t)nanoshell_errlog_write - USER_SEGMENT_BASE),
    (uint8_t (*)(char *))((uint32_t)nanoshell_errlog_print - USER_SEGMENT_BASE),
    (uint8_t (*)(char *))((uint32_t)nanoshell_screen_switch - USER_SEGMENT_BASE),
    (uint8_t (*)(char *))((uint32_t)nanoshell_color_switch - USER_SEGMENT_BASE),
    (uint8_t (*)(char *))((uint32_t)nanoshell_hexdump - USER_SEGMENT_BASE),
    (uint8_t (*)(char *))((uint32_t)nanoshell_kernel_stack_print - USER_SEGMENT_BASE)
};

USER_TEXT
unsigned char find_comm(char **str, const char *command_list[], unsigned char list_len)
{
    if ((str == NULL) || (command_list == NULL) || (list_len == 0))
    {
        return (0);
    }
    if (*str == NULL)
    {
        return (0);
    }

    const char *start = *str;
    char *itr_comm, *itr_str;
    unsigned char comm_idx = 0;

    while ((**str != '\0') && ((**str != ' ')) && (**str != '\n'))
    {
        (*str)++;
    }
    const char *comm_end = *str;

    if (**str == ' ')
    {
        (*str)++;
    }

    for (comm_idx = 0; comm_idx < list_len; comm_idx++)
    {
        itr_str = (char *)start;
        itr_comm = (char *)command_list[comm_idx];
        while ((itr_str != comm_end) && (*itr_comm != '\0'))
        {
            if (*itr_str != *itr_comm)
            {
                break;
            }
            itr_str++;
            itr_comm++;
        }
        if ((itr_str == comm_end) && (*itr_comm == '\0'))
        {
            break;
        }   
    }

    return (comm_idx % list_len);
}

USER_TEXT
void nanoshell_run()
{
    uint8_t comm_list_len = 8;
    char line_buff[81] = {0};
    uint8_t comm = 0;

    user_syswrite(SHELL_NAME, user_strlen(SHELL_NAME));
    while (1)
    {
        if (nanoshell_gnl(line_buff, 81) != 0u)
        {
            char *line_ptr = line_buff;
            if (command_funcs[find_comm(&line_ptr, command_list, comm_list_len)](line_ptr) != 0u)
            {
                user_syswrite("Not executed\n", user_strlen("Not executed\n"));
                user_syserrwrite("Not executed\n", user_strlen("Not executed\n"), (uint32_t)ERRLOG_LVL_ERR);
            }
            user_syswrite(SHELL_NAME, user_strlen(SHELL_NAME));
        }

        if (nanoshell_gnc(&comm))
        {
            if (comm == KEYBOARD_COMM_SCROLL_UP)
            {
                nanoshell_scroll(SYS_SCROLL_UP);
            }
            else if (comm == KEYBOARD_COMM_SCROLL_DOWN)
            {
                nanoshell_scroll(SYS_SCROLL_DOWN);
            }
        }
        nanoshell_wait();
    }
}
