#include "inttype.h"
#include "kernel.h"
#include "errlog.h"
#include "history.h"
#include "start.h"
#include "str_utils.h"

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
 * @param[out] screen  Pointer to the history buffer that will hold the formatted error log lines.
 *                     Must be pre-allocated and will be initialized by this function.
 * @param[in]  lvl     Minimum error level to include in the output. Entries with level >= @p lvl are displayed.
 *                     Uses @c errlog_err_lvl_t enumeration.
 *
 * @note @c err_tags string maps @c err.lvl indices to display tag: " Eacewnid"
 *   (index 0: space (none), 1: 'E', 2: 'a', 3: 'c', 4: 'e', 5: 'w', 6: 'n', 7: 'i', 8: 'd')
 */
uint32_t isr_syscall_errprint(uint32_t lvl)
{
    char entry_str[HISTORY_WIDTH];
    errlog_entry_t err;
    char err_tag;
    const char *err_tags = " Eacewnid";

    errlog_read_init(&errlog, lvl);
    while (errlog_read(&errlog, &err) == ERRLOG_RET_OK)
    {
        err_tag = err_tags[err.lvl];
        int len = md_vsnprintf(entry_str, HISTORY_WIDTH, "%c: %s", err_tag, err.message_str);
        screen_put_str(g_screen, entry_str);
        if (len > 0)
        {
            if (entry_str[len - 1] != '\n')
            {
                screen_put_char(g_screen, '\n');
            }
        }
        
    }
    return 0;
}