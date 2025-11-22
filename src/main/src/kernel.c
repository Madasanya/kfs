#include "screen.h"
#include "history.h"
#include "errlog.h"
#include "start.h"
#include "colors.h"
#include "keyboard.h"
#include "printk.h"
#include "str_utils.h"

#define NUM_SCREENS 5

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
    char entry_str[HISTORY_WIDTH];
    errlog_entry_t err;
    char err_tag;
    const char *err_tags = " Eacewnid";

    errlog_read_init(&errlog, lvl);
    while (errlog_read(&errlog, &err) == ERRLOG_RET_OK)
    {
        err_tag = err_tags[err.lvl];
        md_vsnprintf(entry_str, HISTORY_WIDTH, "%c: %s", err_tag, err.message_str);
        screen_put_str(screen, entry_str);
        uint16_t len = md_strlen(entry_str);
        if (len != 0u)
        {
            if (entry_str[len - 1] != '\n')
            {
                screen_put_char(screen, '\n');
            }
        }
        
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

static void ascii_handler(keyboard_t *keyboard, screen_t *screen)
{
    char c;
    if (keyboard_char_get(keyboard, &c) == 1)
    {
        screen_put_char(screen, c);
    }
}

static void command_handler(keyboard_t *keyboard, screen_t *screen, uint8_t *current_screen_index, screen_t *screens, uint8_t *current_color_index)
{
    uint8_t comm;
    if (keyboard_comm_get(keyboard, &comm) == 1)
    {
        if (comm == KEYBOARD_COMM_CHANGE_SCREEN)
        {
            screen_close(screen);
            *current_screen_index = (*current_screen_index + 1) % NUM_SCREENS;
            screen = &(screens[*current_screen_index]);
            screen_open(screen);
        }
        else if (comm >= KEYBOARD_COMM_START_LOG_LVL1 && comm <= KEYBOARD_COMM_START_LOG_LVL8)
        {
            errorlog_key_handler(screen, comm);
        }
        else if (comm == KEYBOARD_COMM_SCROLL_UP)
        {
            screen_scroll_up(screen);
        }
        else if (comm == KEYBOARD_COMM_SCROLL_DOWN)
        {
            screen_scroll_down(screen);
        }
        else if (comm == KEYBOARD_COMM_CHANGE_COLOR)
        {
            *current_color_index = (*current_color_index + 1) % NUM_SCREEN_COLOR_PROFILES;
            screen_set_color(screen, SCREEN_COLOR_PROFILES[*current_color_index]);
        }
    }
}

void kernel(void)
{
    screen_t screens[NUM_SCREENS];
    history_buffer_t history_buffers[NUM_SCREENS];
    screen_t *active_screen = NULL;
    uint8_t current_screen_index = 0;
    uint8_t current_color_index[NUM_SCREENS] = {0};
    char header_buf[15];

    keyboard_t keyboard = {0};
    
    /* KERNEL INITIALIZATION */
    md_printk("Kernel inititalization\n");
    for (uint8_t i = 0; i < NUM_SCREENS; i++)
    {
        md_vsnprintf(header_buf, sizeof(header_buf), "42 - Screen %d", i + 1);
        screen_init(&(screens[i]), &(history_buffers[i]), SCREEN_COLOR_PROFILES[4 - i], header_buf);
        current_color_index[i] = 4 - i;
    }

    active_screen = &(screens[current_screen_index]);
    screen_open(active_screen);

    /* KERNEL RUN */
    md_printk("Kernel running...\n");
    while (1)
    {
        keyboard_run(&keyboard);
        ascii_handler(&keyboard, active_screen);
        command_handler(&keyboard, active_screen, &current_screen_index, screens, &(current_color_index[current_screen_index]));
        active_screen = &(screens[current_screen_index]);
    }
}