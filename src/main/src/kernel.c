#include "screen.h"
#include "history.h"
#include "errlog.h"
#include "start.h"
#include "colors.h"
#include "keyboard.h"
#include "printk.h"
#include "str_utils.h"
#include "user.h"
#include "usermode.h"
#include "kernel.h"
#include "isr_syscall.h"


#define SCREEN_HEADER_BUF_LEN 15u

screen_t *g_active_screen = NULL;
uint8_t g_current_screen_index = 0;
uint8_t g_current_color_index[NUM_SCREENS] = {0};

/**
 * @brief Handles error log display commands triggered by keyboard input.
 *
 * @details
 * This function processes keyboard commands related to error log display requests.
 * When a valid log level command is received, it displays an appropriate header
 * and calls @c isr_syscall_errprint to populate the screen with error log entries
 * filtered by the corresponding severity level.
 *
 * The function maps keyboard commands (KEYBOARD_COMM_START_LOG_LVL1 through
 * KEYBOARD_COMM_START_LOG_LVL8) to error log levels (Emergency through Debug).
 *
 * @param[in,out] screen   Pointer to the screen where error log will be displayed.
 * @param[in]     command  Keyboard command code indicating which error log level to display.
 *                         Expected range: KEYBOARD_COMM_START_LOG_LVL1 to KEYBOARD_COMM_START_LOG_LVL8.
 */
static void errorlog_key_handler(screen_t *screen, uint8_t command)
{
    switch (command)
    {
        case KEYBOARD_COMM_START_LOG_LVL1:
            screen_put_str(screen, "ERROR LOG - LEVEL EMERGENCY:\n");
            isr_syscall_errprint(ERRLOG_LVL_EMERG);
            break;
        case KEYBOARD_COMM_START_LOG_LVL2:
            screen_put_str(screen, "ERROR LOG - LEVEL ALERT:\n");
            isr_syscall_errprint(ERRLOG_LVL_ALERT);
            break;
        case KEYBOARD_COMM_START_LOG_LVL3:
            screen_put_str(screen, "ERROR LOG - LEVEL CRITICAL:\n");
            isr_syscall_errprint(ERRLOG_LVL_CRIT);
            break;
        case KEYBOARD_COMM_START_LOG_LVL4:
            screen_put_str(screen, "ERROR LOG - LEVEL ERROR:\n");
            isr_syscall_errprint(ERRLOG_LVL_ERR);
            break;
        case KEYBOARD_COMM_START_LOG_LVL5:
            screen_put_str(screen, "ERROR LOG - LEVEL WARNING:\n");
            isr_syscall_errprint(ERRLOG_LVL_WARNING);
            break;
        case KEYBOARD_COMM_START_LOG_LVL6:
            screen_put_str(screen, "ERROR LOG - LEVEL NOTICE:\n");
            isr_syscall_errprint(ERRLOG_LVL_NOTICE);
            break;
        case KEYBOARD_COMM_START_LOG_LVL7:
            screen_put_str(screen, "ERROR LOG - LEVEL INFO:\n");
            isr_syscall_errprint(ERRLOG_LVL_INFO);
            break;
        case KEYBOARD_COMM_START_LOG_LVL8:
            screen_put_str(screen, "ERROR LOG - LEVEL DEBUG:\n");
            isr_syscall_errprint(ERRLOG_LVL_DEBUG);
            break;
        default:
            break;
    }
}

/**
 * @brief Processes and displays ASCII character input from the keyboard.
 *
 * @details
 * This function retrieves a single ASCII character from the keyboard buffer
 * and displays it on the active screen. If a character is successfully retrieved,
 * it is immediately rendered to the screen output.
 *
 * @param[in,out] keyboard  Pointer to the keyboard state structure from which to retrieve input.
 * @param[in,out] screen    Pointer to the screen where the character will be displayed.
 */
static void ascii_handler(keyboard_t *keyboard, screen_t *screen)
{
    char c;
    if (keyboard_char_get(keyboard, &c) == 1)
    {
        screen_put_char(screen, c);
    }
}

/**
 * @brief Processes special keyboard commands for screen management and error log display.
 *
 * @details
 * This function retrieves and executes special keyboard commands that control screen behavior.
 * Supported commands include:
 * - Screen switching: Cycles through available screens (KEYBOARD_COMM_CHANGE_SCREEN)
 * - Error log display: Displays error logs at various severity levels (KEYBOARD_COMM_START_LOG_LVL1-8)
 * - Scrolling: Moves screen viewport up or down (KEYBOARD_COMM_SCROLL_UP/DOWN)
 * - Color change: Cycles through available color profiles (KEYBOARD_COMM_CHANGE_COLOR)
 *
 * When switching screens, the function properly closes the current screen before opening the new one.
 *
 * @param[in,out] keyboard               Pointer to the keyboard state structure.
 * @param[in,out] screen                 Pointer to the currently active screen.
 * @param[in,out] current_screen_index   Pointer to the index of the currently active screen.
 *                                       Updated when switching screens.
 * @param[in,out] screens                Array of all available screens.
 * @param[in,out] current_color_index    Pointer to the current color profile index for the active screen.
 *                                       Updated when changing colors.
 */
static void command_handler(keyboard_t *keyboard, screen_t *screen, uint8_t *current_screen_index, screen_t *screens, uint8_t *current_color_index)
{
    uint8_t comm;
    current_screen_index = current_screen_index;
    screens = screens;
    if (keyboard_comm_get(keyboard, &comm) == 1)
    {
        if (comm == KEYBOARD_COMM_CHANGE_SCREEN)
        {
            
        }
        else if (comm >= KEYBOARD_COMM_START_LOG_LVL1 && comm <= KEYBOARD_COMM_START_LOG_LVL8)
        {
            errorlog_key_handler(screen, comm);
        }
        else if (comm == KEYBOARD_COMM_CHANGE_COLOR)
        {
            *current_color_index = (*current_color_index + 1) % NUM_SCREEN_COLOR_PROFILES;
            screen_set_color(screen, SCREEN_COLOR_PROFILES[*current_color_index]);
        }
    }
}

/**
 * @brief Main kernel entry point and execution loop.
 *
 * @details
 * This function initializes the kernel subsystems and enters the main kernel loop.
 * It sets up multiple virtual screens with history buffers, initializes the keyboard
 * driver, and continuously processes keyboard input to handle both ASCII character
 * display and special commands (screen switching, scrolling, color changes, error logs).
 *
 * The kernel manages NUM_SCREENS virtual screens, each with its own history buffer
 * and color profile, allowing users to switch between different terminal views.
 *
 * @note This function never returns - it runs in an infinite loop processing keyboard events.
 */
void kernel(void)
{
    screen_t screens[NUM_SCREENS];
    history_buffer_t history_buffers[NUM_SCREENS];
    char header_buf[SCREEN_HEADER_BUF_LEN];

    keyboard_t keyboard = {0};
    
    /* KERNEL INITIALIZATION */
    md_printk("Kernel inititalization\n");
    for (uint8_t i = 0; i < NUM_SCREENS; i++)
    {
        md_vsnprintf(header_buf, sizeof(header_buf), "42 - Screen %d", i + 1);
        screen_init(&(screens[i]), &(history_buffers[i]), SCREEN_COLOR_PROFILES[4 - i], header_buf);
        g_current_color_index[i] = 4 - i;
    }

    g_active_screen = &(screens[g_current_screen_index]);
    screen_open(g_active_screen);

    /* KERNEL RUN */
    md_printk("Kernel running...\n");

    while (1)
    {
        keyboard_run(&keyboard);
        ascii_handler(&keyboard, g_active_screen);
        command_handler(&keyboard, g_active_screen, &g_current_screen_index, screens, &(g_current_color_index[g_current_screen_index]));
        g_active_screen = &(screens[g_current_screen_index]);
        // Now we can jump to user mode if needed
        user_enter((void*)user_main); // Uncomment when user code is loaded
    }
}