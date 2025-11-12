/**
 * @file errlog.h
 * @brief Error logging module with fixed-size circular buffer.
 *
 * @details
 * The `errlog` module provides a lightweight, in-memory circular buffer for
 * storing and retrieving error and diagnostic messages with associated severity
 * levels. It is designed for kernel message logging.
 *
 * ### Key Features:
 * - **Fixed-size circular buffer** of `ERRLOG_LOG_LEN` entries.
 * - **Severity-based filtering** during readback using `errlog_err_lvl_t`.
 * - **Default level substitution** via `ERRLOG_LVL_DEFAULT`.
 * - **Safe string handling** with truncation and ellipsis (`...`) indication.
 * - **Non-destructive read** — entries remain until overwritten.
 *
 * ### Usage Pattern:
 * 1. Call `errlog_init()` to initialize the log structure.
 * 2. Use `errlog_write()` to record messages.
 * 3. To read entries:
 *    - `errlog_read_init(errlog, level)` to start reading up to a given level.
 *    - Repeated `errlog_read()` to retrieve matching entries (most recent first).
 *    - `errlog_read_done()` to finalize.
 *
 * ### Buffer Full Behavior:
 * When full, the oldest entry is overwritten.
 *
 * @note All functions assume valid pointers and initialized structures.
 * @note Message strings must be null-terminated.
 * @note Maximum message length is `ERRLOG_MSG_LEN` (excluding null terminator).
 * @note In case of longer messages, messages will be truncated and ended with "..."
 */

 
#ifndef _ERRLOG_H_
#define _ERRLOG_H_

#include "inttype.h"

/**
 * @brief Return codes for error log operations.
 *
 * @details
 * These values indicate the outcome of errlog functions. Negative values
 * represent errors, zero indicates success, and positive values provide
 * additional status information.
 */
typedef enum
{
    /** Operation failed due to invalid parameter or internal error. */
    ERRLOG_RET_ERR   = -1,
    /** Operation completed successfully with no changes. */
    ERRLOG_RET_OK    = 0,
    /** Operation succeeded but message was truncated or modified. */
    ERRLOG_RET_CHNG  = 1,
    /** No entries available (log empty or no matching level). */
    ERRLOG_RET_EMPTY = 2
} errlog_ret_t;

/**
 * @brief Error severity levels for logging.
 *
 * @details
 * Levels are ordered from most severe (EMERG) to least severe (DEBUG).
 * Higher numeric values mean lower severity. ERRLOG_LVL_DEFAULT acts as
 * a placeholder to use the current default level.
 *
 * @note ERRLOG_LVL_DEFAULT set via errlog_default_lvl_set or init if non is set ERRLOG_DEFAULT_ERR_LVL is used.
 * @note ERRLOG_LVL_LEN is a sentinel value indicating the number of levels.
 */
typedef enum
{
    /** Use current default level (set via errlog_default_lvl_set or init). */
    ERRLOG_LVL_DEFAULT = 0,
    ERRLOG_LVL_EMERG   = 1,
    ERRLOG_LVL_ALERT   = 2,
    ERRLOG_LVL_CRIT    = 3,
    ERRLOG_LVL_ERR     = 4,
    ERRLOG_LVL_WARNING = 5,
    ERRLOG_LVL_NOTICE  = 6,
    ERRLOG_LVL_INFO    = 7,
    ERRLOG_LVL_DEBUG   = 8,
    ERRLOG_LVL_LEN
} errlog_err_lvl_t;

/** @brief Maximum number of entries stored in the circular log buffer. */
#define ERRLOG_LOG_LEN 5

/** @brief Maximum length of a log message, including null terminator. */
#define ERRLOG_MSG_LEN 10

/** @brief Default logging level used when ERRLOG_LVL_DEFAULT is not in init specified. */
#define ERRLOG_DEFAULT_ERR_LVL ERRLOG_LVL_WARNING

/**
 * @brief Log entry containing message and severity level.
 *
 * @details
 * Each entry holds a null-terminated string of up to ERRLOG_MSG_LEN
 * characters (excluding the null terminator). The level indicates the
 * severity of the logged event.
 */
typedef struct
{
    /** Message string, null-terminated, max ERRLOG_MSG_LEN chars. */
    char message_str[ERRLOG_MSG_LEN + 1];
    /** Severity level of this entry. */
    errlog_err_lvl_t lvl;
} errlog_entry_t;

/**
 * @brief Main error log structure with circular buffer.
 *
 * @details
 * Implements a fixed-size circular buffer with ERRLOG_LOG_LEN + 1 slots
 * to allow distinguishing between empty and full states. The extra slot
 * is never used for storage but enables the full/empty detection logic.
 */
typedef struct
{
    /** Array of log entries forming the circular buffer. */
    errlog_entry_t log[ERRLOG_LOG_LEN + 1];
    /** Index where the next write will occur. */
    uint8_t write_idx;
    /** Index of the oldest (first) entry in the buffer. */
    uint8_t first_idx;
    /** Current read pointer used during errlog_read() sequence. */
    uint8_t read_idx;
    /** Default level used when ERRLOG_LVL_DEFAULT is passed. */
    errlog_err_lvl_t default_lvl;
    /** Maximum level to include during current read session. */
    errlog_err_lvl_t read_lvl;
} errlog_t;

/**
 * @brief Initializes the error log structure with a default logging level.
 *
 * @details
 * This function initializes an errlog_t instance. If the user-provided default_lvl
 * is invalid (out of enum range), it falls back to the compile-time constant
 * ERRLOG_DEFAULT_ERR_LVL. If that constant is also invalid, the function fails.
 * All internal circular-buffer indices are reset, and the read level is set to
 * ERRLOG_LVL_DEFAULT.
 *
 * @param[in,out] err_log     Pointer to the error log structure to be initialized.
 * @param[in]     default_lvl Desired default error level (may be ERRLOG_LVL_DEFAULT).
 *
 * @return ERRLOG_RET_OK     Initialization successful.
 * @return ERRLOG_RET_ERR    Both provided and fallback default levels are invalid.
 *
 * @note Must be called before any other errlog_ functions.
 */
errlog_ret_t errlog_init(errlog_t *err_log, errlog_err_lvl_t default_lvl);

/**
 * @brief Changes the default logging level used when ERRLOG_LVL_DEFAULT is specified.
 *
 * @details
 * Updates the default_lvl field of the error log. This value will be substituted
 * whenever ERRLOG_LVL_DEFAULT is passed to errlog_write() or errlog_read_init().
 * The function performs range checking and rejects invalid levels.
 *
 * @param[in,out] err_log     Pointer to the error log structure.
 * @param[in]     default_lvl New default error level.
 *
 * @return ERRLOG_RET_OK     Default level updated successfully.
 * @return ERRLOG_RET_ERR    Provided level is out of valid range.
 */
errlog_ret_t errlog_default_lvl_set(errlog_t *err_log, errlog_err_lvl_t default_lvl);

/**
 * @brief Prepares the log for reading entries up to a specific severity level.
 *
 * @details
 * Initializes a read session by setting the maximum level (read_lvl) that will be
 * returned by subsequent calls to errlog_read(). If lvl is ERRLOG_LVL_DEFAULT,
 * the current default_lvl (or ERRLOG_DEFAULT_ERR_LVL) is used.
 * The read pointer starts at the most recent entry (write_idx).
 * If the log is empty, ERRLOG_RET_EMPTY is returned immediately.
 *
 * @param[in,out] err_log Pointer to the error log structure.
 * @param[in]     lvl     Maximum level to read (inclusive). Use ERRLOG_LVL_DEFAULT
 *                        to use the current default.
 *
 * @return ERRLOG_RET_OK     Read session started successfully.
 * @return ERRLOG_RET_ERR    Provided level is invalid.
 * @return ERRLOG_RET_EMPTY  Log contains no entries.
 *
 * @note Call errlog_read_done() when finished reading to reset internal state.
 */
errlog_ret_t errlog_read_init(errlog_t *err_log, errlog_err_lvl_t lvl);

/**
 * @brief Retrieves the next (most recent) log entry that matches the current read level.
 *
 * @details
 * Scans backward from the last written entry, skipping any entries with a level
 * strictly greater than read_lvl (set by errlog_read_init()). The first entry
 * that satisfies lvl <= read_lvl is copied into the user-provided entry structure.
 * The message is guaranteed to be null-terminated and truncated to ERRLOG_MSG_LEN
 * (including terminator). If no matching entry exists, ERRLOG_RET_EMPTY is returned.
 *
 * The read_idx is advanced only when a matching entry is found; otherwise it moves
 * past non-matching entries. This allows repeated calls to retrieve older matching
 * entries in reverse chronological order.
 *
 * @param[in,out] err_log Pointer to the error log structure.
 * @param[out]    entry   Pointer to structure receiving the log entry (message + level).
 *
 * @return ERRLOG_RET_OK     Entry copied successfully.
 * @return ERRLOG_RET_ERR    Internal read_lvl is invalid (should not happen if init used).
 * @return ERRLOG_RET_EMPTY  No more entries match the current read level.
 *
 * @note The entry buffer is cleared (first byte set to '\0') before use.
 * @note This function does **not** remove entries from the log.
 */
errlog_ret_t errlog_read(errlog_t *err_log, errlog_entry_t *entry);

/**
 * @brief Ends a read session and resets read-related state.
 *
 * @details
 * Resets the internal read_lvl to ERRLOG_LVL_DEFAULT. This should be called after
 * a sequence of errlog_read() calls to indicate that the read session is complete.
 * It helps prevent accidental continued use of a stale read_lvl.
 *
 * @param[in,out] err_log Pointer to the error log structure.
 */
void errlog_read_done(errlog_t *err_log);

/**
 * @brief Write a new message to the log.
 *
 * @details
 * Stores a message with the given severity level. If lvl is ERRLOG_LVL_DEFAULT,
 * the current default_lvl is used. The message is copied safely using md_strlcpy();
 * if it exceeds ERRLOG_MSG_LEN characters, it is truncated and the last three
 * characters are replaced with "..." to indicate truncation. In this case,
 * ERRLOG_RET_CHNG is returned.
 *
 * The log operates as a fixed-size circular buffer. When full, the oldest entry
 * (pointed to by first_idx) is overwritten.
 *
 * @param[in,out] err_log Pointer to the error log structure.
 * @param[in]     lvl     Severity level of the message.
 * @param[in]     message Null-terminated message string to log.
 *
 * @return ERRLOG_RET_OK     Message stored completely.
 * @return ERRLOG_RET_CHNG   Message was truncated (ends with "...").
 * @return ERRLOG_RET_ERR    Invalid level provided.
 *
 * @warning message must be null-terminated.
 */
errlog_ret_t errlog_write(errlog_t *err_log, errlog_err_lvl_t lvl, const char *message);

#endif /* _ERRLOG_H_ */