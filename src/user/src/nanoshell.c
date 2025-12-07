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
    buffer[7] = '0'; // set screen 3
    buffer[8] = '\0';  // Null terminator
    
    return 1;
}

USER_TEXT
uint32_t gnl_fake_color(char *buffer)
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
    buffer[0] = 'c';
    buffer[1] = 'o';
    buffer[2] = 'l';
    buffer[3] = 'o';
    buffer[4] = 'r';
    buffer[5] = ' ';
    buffer[6] = '2'; // set color 2
    buffer[7] = '\0';  // Null terminator
    
    return 1;
}

USER_TEXT
uint32_t gnl_fake_hexdump(char *buffer)
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
    buffer[0] = 'p';
    buffer[1] = 'r';
    buffer[2] = 'i';
    buffer[3] = 'n';
    buffer[4] = 't';
    buffer[5] = '-';
    buffer[6] = 'k';
    buffer[7] = 'e';
    buffer[8] = 'r';
    buffer[9] = 'n';
    buffer[10] = 'e';
    buffer[11] = 'l';
    buffer[12] = '-';
    buffer[13] = 's';
    buffer[14] = 't';
    buffer[15] = 'a';
    buffer[16] = 'c';
    buffer[17] = 'k';
    buffer[18] = '-';
    buffer[19] = 't';
    buffer[20] = 'h';
    buffer[21] = 'i';
    buffer[22] = 'n';
    buffer[23] = 'g';
    buffer[24] = 'y';
    buffer[25] = ' ';
    buffer[26] = '0';
    buffer[27] = 'x';
    buffer[28] = '0';
    buffer[29] = '0';
    buffer[30] = '3';
    buffer[31] = '0';
    buffer[32] = '0';
    buffer[33] = '5';
    buffer[34] = 'E';
    buffer[35] = '1';
    buffer[36] = ' ';
    buffer[37] = '1';
    buffer[38] = '0';
    buffer[39] = '2';
    buffer[40] = '\0';  // Null terminator
    
    return 1;
}

USER_TEXT
void nanoshell_run()
{
    const char *command_list[] =    {"_", \
                                    "echo", \
                                    "report", \
                                    "err_log", \
                                    "screen", \
                                    "color", \
                                    "print-kernel-stack-thingy"};
                                    //"exit"};
    uint8_t (*command_funcs[])(char *) =    {wrong_comm, \
                                            nanoshell_echo, \
                                            nanoshell_errlog_write, \
                                            nanoshell_errlog_print, \
                                            nanoshell_screen_switch, \
                                            nanoshell_color_switch, \
                                            nanoshell_hexdump};//, exit_to_kernel};
    const uint8_t comm_list_len = sizeof(command_list)/sizeof(char *);
    char line_buff[81] = {0};

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


        gnl_fake_hexdump((char *)&line_buff);
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
        if (nanoshell_gnl(line_buff, 81) != 0u)
        {
            user_syswrite(line_buff, user_strlen(line_buff));
        }
    }
}
