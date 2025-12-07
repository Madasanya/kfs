#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "inttype.h"
#include "nanoshell_builtins.h"
#include "user_syscall.h"
#include "errlog.h"
#include "user_str_utils.h"

#define SHELL_NAME "\nnanoshell> "

extern uint8_t gnl(char * buffer);

USER_TEXT
static unsigned char wrong_comm(char *str)
{
    str = str; // Suppress unused variable warning
    nanoshell_echo("Unknown command\n");
    user_syserrwrite("Unknown command\n", user_strlen("Unknown command\n"), (uint32_t)ERRLOG_LVL_WARNING);
    return (0);
}

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

    while ((**str != '\0') && ((**str != ' ')))
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
        if (itr_str == comm_end)
        {
            break;
        }   
    }

    return (comm_idx % list_len);
}

USER_TEXT
uint32_t gnl_fake_echo(char *buffer)
{
    // Ensure clean buffer initialization - this is critical
    // because find_comm has a bug where it doesn't verify
    // that *itr_comm == '\0' when itr_str == comm_end
    int i;
    for (i = 0; i < 81; i++)
    {
        buffer[i] = '\0';
    }
    
    // Write exactly: "echo Hello" with no extra characters
    // The command must match exactly to work around find_comm bug
    buffer[0] = 'e';
    buffer[1] = 'c';
    buffer[2] = 'h';
    buffer[3] = 'o';
    buffer[4] = ' ';  // Space separator between command and args
    buffer[5] = 'H';
    buffer[6] = 'e';
    buffer[7] = 'l';
    buffer[8] = 'l';
    buffer[9] = 'o';
    buffer[10] = '\n'; // Newline at end
    buffer[11] = '\0';  // Null terminator
    
    return 1;
}

USER_TEXT
uint32_t gnl_fake_report(char *buffer)
{
    // Ensure clean buffer initialization - this is critical
    // because find_comm has a bug where it doesn't verify
    // that *itr_comm == '\0' when itr_str == comm_end
    int i;
    for (i = 0; i < 81; i++)
    {
        buffer[i] = '\0';
    }
    
    // Write exactly: "echo Hello" with no extra characters
    // The command must match exactly to work around find_comm bug
    buffer[0] = 'r';
    buffer[1] = 'e';
    buffer[2] = 'p';
    buffer[3] = 'o';
    buffer[4] = 'r';
    buffer[5] = 't';
    buffer[6] = ' ';
    buffer[7] = 'O';
    buffer[8] = 'h';
    buffer[9] = ' ';
    buffer[10] = 'N';
    buffer[11] = 'o';
    buffer[12] = '\n'; // Newline at end
    buffer[13] = '\0';  // Null terminator
    
    return 1;
}

USER_TEXT
uint32_t gnl_fake_errlog(char *buffer)
{
    // Ensure clean buffer initialization - this is critical
    // because find_comm has a bug where it doesn't verify
    // that *itr_comm == '\0' when itr_str == comm_end
    int i;
    for (i = 0; i < 81; i++)
    {
        buffer[i] = '\0';
    }
    
    // Write exactly: "echo Hello" with no extra characters
    // The command must match exactly to work around find_comm bug
    buffer[0] = 'e';
    buffer[1] = 'r';
    buffer[2] = 'r';
    buffer[3] = '_';
    buffer[4] = 'l';
    buffer[5] = 'o';
    buffer[6] = 'g';
    buffer[7] = ' ';
    buffer[8] = '8'; // Level 8
    buffer[9] = '\0';  // Null terminator
    
    return 1;
}

USER_TEXT
uint32_t gnl_fake_screen(char *buffer)
{
    // Ensure clean buffer initialization - this is critical
    // because find_comm has a bug where it doesn't verify
    // that *itr_comm == '\0' when itr_str == comm_end
    int i;
    for (i = 0; i < 81; i++)
    {
        buffer[i] = '\0';
    }
    
    // Write exactly: "echo Hello" with no extra characters
    // The command must match exactly to work around find_comm bug
    buffer[0] = 's';
    buffer[1] = 'c';
    buffer[2] = 'r';
    buffer[3] = 'e';
    buffer[4] = 'e';
    buffer[5] = 'n';
    buffer[6] = ' ';
    buffer[7] = '4'; // set screen 3
    buffer[8] = '\0';  // Null terminator
    
    return 1;
}

USER_TEXT
void nanoshell_run()
{
    const char *command_list[] = {"_", "echo", "report", "err_log", "screen", "exit" };
    uint8_t (*command_funcs[])(char *) =    {wrong_comm, \
                                            nanoshell_echo, \
                                            nanoshell_errlog_write, \
                                            nanoshell_errlog_print, \
                                            nanoshell_screen_switch};//, change_screen, exit_to_kernel};
    const uint8_t comm_list_len = sizeof(command_list)/sizeof(char *);
    char line_buff[81] = {0};

    nanoshell_echo(SHELL_NAME);
    while (1)
    {
        // if (gnl_fake_echo((char *)&line_buff) != 0u)
        // {
        //     char *line_ptr = line_buff;
        //     if (command_funcs[find_comm(&line_ptr, command_list, comm_list_len)](line_ptr) != 0u)
        //     {
        //         nanoshell_echo("Not executed\n");
        //         user_syserrwrite("Not executed\n", user_strlen("Not executed\n"), (uint32_t)ERRLOG_LVL_ERR);
        //     }
        //     nanoshell_echo(SHELL_NAME);
        // }
        gnl_fake_echo((char *)&line_buff);
        char *line_ptr = line_buff;
        if (command_funcs[find_comm(&line_ptr, command_list, comm_list_len)](line_ptr) != 0u)
        {
            nanoshell_echo("Not executed\n");
            user_syserrwrite("Not executed\n", user_strlen("Not executed\n"), (uint32_t)ERRLOG_LVL_ERR);
        }
        nanoshell_echo(SHELL_NAME);

        gnl_fake_screen((char *)&line_buff);
        line_ptr = line_buff;
        if (command_funcs[find_comm(&line_ptr, command_list, comm_list_len)](line_ptr) != 0u)
        {
            nanoshell_echo("Not executed\n");
            user_syserrwrite("Not executed\n", user_strlen("Not executed\n"), (uint32_t)ERRLOG_LVL_ERR);
        }

        gnl_fake_report((char *)&line_buff);
        line_ptr = line_buff;
        if (command_funcs[find_comm(&line_ptr, command_list, comm_list_len)](line_ptr) != 0u)
        {
            nanoshell_echo("Not executed\n");
            user_syserrwrite("Not executed\n", user_strlen("Not executed\n"), (uint32_t)ERRLOG_LVL_ERR);
        }

        gnl_fake_errlog((char *)&line_buff);
        line_ptr = line_buff;
        if (command_funcs[find_comm(&line_ptr, command_list, comm_list_len)](line_ptr) != 0u)
        {
            nanoshell_echo("Not executed\n");
            user_syserrwrite("Not executed\n", user_strlen("Not executed\n"), (uint32_t)ERRLOG_LVL_ERR);
        }
        nanoshell_echo(SHELL_NAME);

        // if (gnc(comm) != 0)
        // {
            
        // }
    }
}
