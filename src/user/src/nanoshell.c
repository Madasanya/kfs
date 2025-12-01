#define SHELL_NAME "nanoshell> "
#define NULL (void *)0

extern unsigned char screen_write(char* str);
extern unsigned char errlog_write(char *str);
extern unsigned char errlog_print(char *str);
extern unsigned char change_screen(char *str);
extern unsigned char exit_to_kernel(char *str);

extern unsigned char gnl(char * buffer);

static unsigned char wrong_comm(char *str)
{
    screen_write("Unknown command\n");
    errlog_print("E Unknown command\n");
    return (0);
}

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
        itr_str = start;
        itr_comm = command_list[comm_idx];
        while ((itr_str != comm_end) && (itr_comm != '\0'))
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

void nanoshell_run()
{
    const char *command_list[] = {"_", "echo", "report", "switch", "err_log", "exit" };
    const unsigned char (*command_funcs[])(char) = {wrong_comm, screen_write, errlog_write, change_screen, errlog_print, exit_to_kernel};
    const unsigned char comm_list_len = sizeof(command_list)/sizeof(char *);
    char line_buff[81] = {0};

    screen_write(SHELL_NAME);
    while (1)
    {
        if (gnl(line_buff) != 0u)
        {
            if (command_funcs[find_comm(&line_buff, command_list, comm_list_len)](line_buff) != 0u)
            {
                screen_write("Not executed\n");
                errlog_print("E Not executed\n");
            }
            screen_write(SHELL_NAME);
        }
    }
}
