#include "str_utils.h"
#include "errlog.h"
#include "start.h"
#define KERN_SOH_ASCII '\001'
#define KERN_EMERG_ASCII '0' /* system is unusable */
#define KERN_ALERT_ASCII '1' /* action must be taken immediately */
#define KERN_CRIT_ASCII '2' /* critical conditions */
#define KERN_ERR_ASCII '3' /* error conditions */
#define KERN_WARNING_ASCII '4' /* warning conditions */
#define KERN_NOTICE_ASCII '5' /* normal but significant condition */
#define KERN_INFO_ASCII '6' /* informational */
#define KERN_DEBUG_ASCII '7' /* debug-level messages */
#define PRINTK_BUFF_SIZE 82

/**
 * @brief Determines the error log level from a kernel-style format string prefix and advances the format pointer.
 *
 * @details
 * This function checks if the format string starts with KERN_SOH_ASCII ('\001')
 * followed by a character indicating the log level (e.g., '3' for error). It maps
 * this character to an errlog_err_lvl_t enum value. If no valid prefix is found,
 * it returns ERRLOG_LVL_DEFAULT. The function advances the format pointer past the
 * prefix if present, allowing the caller to use the updated pointer for further processing.
 *
 * @param [inout] fmt Pointer to the format string to parse and advance.
 * @return The corresponding errlog_err_lvl_t level.
 */
static errlog_err_lvl_t printk_lvl_get(const char **fmt)
{
    errlog_err_lvl_t ret;
    char tmp = **fmt;
    if (tmp == KERN_SOH_ASCII)
    {
        (*fmt)++;
        tmp = **fmt;
        if (tmp == '\0')
        {
            ret = ERRLOG_LVL_DEFAULT;
        }
        else
        {
            (*fmt)++;
            switch (tmp)
            {
            case KERN_EMERG_ASCII:
                ret = ERRLOG_LVL_EMERG;
                break;
            case KERN_ALERT_ASCII:
                ret = ERRLOG_LVL_ALERT;
                break;
            case KERN_CRIT_ASCII:
                ret = ERRLOG_LVL_CRIT;
                break;
            case KERN_ERR_ASCII:
                ret = ERRLOG_LVL_ERR;
                break;
            case KERN_WARNING_ASCII:
                ret = ERRLOG_LVL_WARNING;
                break;
            case KERN_NOTICE_ASCII:
                ret = ERRLOG_LVL_NOTICE;
                break;
            case KERN_INFO_ASCII:
                ret = ERRLOG_LVL_INFO;
                break;
            case KERN_DEBUG_ASCII:
                ret = ERRLOG_LVL_DEBUG;
                break;
            default:
                ret = ERRLOG_LVL_DEFAULT;
                break;
            }
        }
    }
    else
    {
        ret = ERRLOG_LVL_DEFAULT;
    }
    return (ret);
}


void printk(const char *fmt, ...)
{
    errlog_err_lvl_t lvl;
    char buff[PRINTK_BUFF_SIZE];
    va_list args;

    if (fmt == NULL)
    {
        return;
    }
    lvl = printk_lvl_get(&fmt);
    va_start(args, fmt);
    (void)vsnprintf_args(buff, PRINTK_BUFF_SIZE, fmt, args);
    va_end(args);
    errlog_write(&errlog, lvl, buff);
}