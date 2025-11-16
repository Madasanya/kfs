#include "str_utils.h"
#include "history.h"
#include "errlog.h"
#include "start.h"

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
static void errlog_screen_assamble (history_buffer_t *screen, errlog_err_lvl_t lvl)
{
    char* entry_str[HISTORY_WIDTH];
    errlog_entry_t err;
    char err_tag;
    const char *err_tags = " Eacewnid";

    md_history_init(screen);
    errlog_read_init(&errlog, lvl);
    while (errlog_read(&errlog, &err) == ERRLOG_RET_OK)
    {
        err_tag = err_tags[err.lvl];
        md_vsnprintf(entry_str, HISTORY_WIDTH, "%c: %s", err_tag, err.message_str);
        md_history_add_entry(screen, entry_str);
    }
}

void kernel(void)
{
    history_buffer_t history_buffer;
    
    md_history_init(&history_buffer);
    md_put_str("42");
}