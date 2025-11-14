#ifndef _PRINTK_H_
#define _PRINTK_H_

#define KERN_SOH	"\001"

#define KERN_EMERG	KERN_SOH "0"	/* system is unusable */
#define KERN_ALERT	KERN_SOH "1"	/* action must be taken immediately */
#define KERN_CRIT	KERN_SOH "2"	/* critical conditions */
#define KERN_ERR	KERN_SOH "3"	/* error conditions */
#define KERN_WARNING	KERN_SOH "4"	/* warning conditions */
#define KERN_NOTICE	KERN_SOH "5"	/* normal but significant condition */
#define KERN_INFO	KERN_SOH "6"	/* informational */
#define KERN_DEBUG	KERN_SOH "7"	/* debug-level messages */

#define KERN_DEFAULT	""		/* the default kernel loglevel */

/**
 * @brief Logs a formatted message to the error log with kernel-style log level support.
 *
 * @details
 * This function emulates kernel printk by parsing the log level from the format
 * string (if prefixed with '\001' followed by a level character), advances the format
 * pointer to strip the prefix, formats the message using vsnprintf_args into a fixed-size
 * buffer, and writes it to the error log using errlog_write. The buffer size is limited
 * to PRINTK_BUFF_SIZE to prevent overflows. If fmt is NULL, the function returns early
 * without logging.
 *
 * @param [in] fmt The format string, potentially prefixed with a log level.
 * @param ... Variable arguments for the format string.
 */
void printk(const char *fmt, ...)
    __attribute__((format(printf, 1, 2)));
/* This attribute enables GCC to perform format string and argument type checking
 * on printk calls, similar to how it checks printf. It treats the format string
 * as parameter 1 and the variable arguments starting at parameter 2. */

#endif /* _PRINTK_H_ */ 