#include "screen.h"
#include "history.h"
#include "errlog.h"
#include "start.h"
#include "colors.h"
#include "io.h"
#include "keyboard.h"
#include "printk.h"

/**
 * @brief Assembles an error log screen display from the kernel error log, filtered by severity level.
 *
 * @details
 * This function initializes a history buffer (intended to represent a screen display) and populates it
 * with formatted error log entries retrieved from the kernel error log subsystem. Each entry is prefixed
 * with a single-character tag corresponding to its severity level (e.g., 'e' for Error, 'w' for Warning).
 *
 * The function reads entries sequentially starting from the last saved  one. Displays only messages specified
 * by @p lvl or more important and continues until no more entries are available.
 *
 * @todo The current implementation uses @c history_buffer_t as the screen representation.This should
 *       be replaced with a dedicated @c screen_t type in the future.
 * 
 * @todo Add colors to error screen.
 *
 * @param[out] screen  Pointer to the history buffer that will hold the formatted error log lines.
 *                     Must be pre-allocated and will be initialized by this function.
 * @param[in]  lvl     Minimum error level to include in the output. Entries with level >= @p lvl are displayed.
 *                     Uses @c errlog_err_lvl_t enumeration.
 *
 * @note @c err_tags string maps @c err.lvl indices to display tag: " Eacewnid"
 *   (index 0: space (none), 1: 'E', 2: 'a', 3: 'c', 4: 'e', 5: 'w', 6: 'n', 7: 'i', 8: 'd')
 */
static void errlog_screen_assemble (screen_t *screen, errlog_err_lvl_t lvl)
{
    char* entry_str[HISTORY_WIDTH];
    errlog_entry_t err;
    char err_tag;
    const char *err_tags = " Eacewnid";

    errlog_read_init(&errlog, lvl);
    while (errlog_read(&errlog, &err) == ERRLOG_RET_OK)
    {
        err_tag = err_tags[err.lvl];
        md_vsnprintf(entry_str, HISTORY_WIDTH, "%c: %s", err_tag, err.message_str);
        screen_put_str(screen, entry_str);
    }
}

static void errorlog_key_handler(screen_t *screen, uint8_t command)
{
    switch (command)
    {
        case KEYBOARD_COMM_START_LOG_LVL1:
            screen_put_str(screen, "ERROR LOG - LEVEL EMERGENCY:\n");
            errlog_screen_assemble(screen, ERRLOG_LVL_EMERG);
            break;
        case KEYBOARD_COMM_START_LOG_LVL2:
            screen_put_str(screen, "ERROR LOG - LEVEL ALERT:\n");
            errlog_screen_assemble(screen, ERRLOG_LVL_ALERT);
            break;
        case KEYBOARD_COMM_START_LOG_LVL3:
            screen_put_str(screen, "ERROR LOG - LEVEL CRITICAL:\n");
            errlog_screen_assemble(screen, ERRLOG_LVL_CRIT);
            break;
        case KEYBOARD_COMM_START_LOG_LVL4:
            screen_put_str(screen, "ERROR LOG - LEVEL ERROR:\n");
            errlog_screen_assemble(screen, ERRLOG_LVL_ERR);
            break;
        case KEYBOARD_COMM_START_LOG_LVL5:
            screen_put_str(screen, "ERROR LOG - LEVEL WARNING:\n");
            errlog_screen_assemble(screen, ERRLOG_LVL_WARNING);
            break;
        case KEYBOARD_COMM_START_LOG_LVL6:
            screen_put_str(screen, "ERROR LOG - LEVEL NOTICE:\n");
            errlog_screen_assemble(screen, ERRLOG_LVL_NOTICE);
            break;
        case KEYBOARD_COMM_START_LOG_LVL7:
            screen_put_str(screen, "ERROR LOG - LEVEL INFO:\n");
            errlog_screen_assemble(screen, ERRLOG_LVL_INFO);
            break;
        case KEYBOARD_COMM_START_LOG_LVL8:
            screen_put_str(screen, "ERROR LOG - LEVEL DEBUG:\n");
            errlog_screen_assemble(screen, ERRLOG_LVL_DEBUG);
            break;
        default:
            break;
    }
}

void kernel(void)
{
    #define NUM_SCREENS 5
    screen_t screens[NUM_SCREENS];
    history_buffer_t history_buffers[NUM_SCREENS];
    screen_t *active_screen = NULL;
    int current_screen_index = 0;

    char temp_c;
    uint8_t temp_comm;
    uint8_t ret;
    keyboard_t keyboard = {0};
    
    md_printk("Kernel starting up...DEFAULT\n");
    md_printk(KERN_EMERG "Kernel starting up...EMERG\n");
    md_printk(KERN_ALERT "Kernel starting up...ALERT\n");
    md_printk(KERN_CRIT "Kernel starting up...CRIT\n");
    md_printk(KERN_ERR "Kernel starting up...ERR\n");
    md_printk(KERN_WARNING "Kernel starting up...WARNING\n");
    md_printk(KERN_NOTICE "Kernel starting up...NOTICE\n");
    md_printk(KERN_INFO "Kernel starting up...INFO\n");
    md_printk(KERN_DEBUG "Kernel starting up...DEBUG\n");

    for (int i = 0; i < NUM_SCREENS; i++)
    {
        screen_init(&screens[i], &history_buffers[i], SCREEN_COLOR_PROFILES[4 - i]);
        screen_put_char(&screens[i], '4');
        screen_put_char(&screens[i], '2');
        screen_set_color(&screens[i], SCREEN_COLOR_PROFILES[i]);
        screen_put_str(&screens[i], " Welcome to KFS!\n");
    }

    active_screen = &screens[current_screen_index];

    while (1)
    {
        keyboard_run(&keyboard);
        ret = keyboard_char_get(&keyboard, &temp_c);
        if (ret == 0)
        {
            ret = keyboard_comm_get(&keyboard, &temp_comm);
            if (ret == 0)
            {
                continue;
            }
            if (temp_comm == KEYBOARD_COMM_CHANGE_SCREEN)
            {
                current_screen_index = (current_screen_index + 1) % NUM_SCREENS;
                active_screen = &screens[current_screen_index];
            }
            else if (temp_comm >= KEYBOARD_COMM_START_LOG_LVL1 && temp_comm <= KEYBOARD_COMM_START_LOG_LVL8)
            {
                errorlog_key_handler(active_screen, temp_comm);
            }
            continue;
        }
        screen_put_char(active_screen, temp_c);
    }
}