#include "errlog.h"
#include "str_utils.h"

errlog_ret_t errlog_init(errlog_t *err_log, errlog_err_lvl_t default_lvl)
{
    if ((default_lvl <= ERRLOG_LVL_DEFAULT) || (default_lvl >= ERRLOG_LVL_LEN))
    {
        if ((ERRLOG_DEFAULT_ERR_LVL <= ERRLOG_LVL_DEFAULT) || (ERRLOG_DEFAULT_ERR_LVL >= ERRLOG_LVL_LEN))
        {
            return ERRLOG_RET_ERR;
        }
        else
        {
            err_log->default_lvl = ERRLOG_DEFAULT_ERR_LVL;
        }
    }
    else
    {
        err_log->default_lvl = default_lvl;
    }
    err_log->first_idx = 0u;
    err_log->write_idx = 0u;
    err_log->read_lvl = ERRLOG_LVL_DEFAULT;
    return (ERRLOG_RET_OK);
}

errlog_ret_t errlog_default_lvl_set(errlog_t *err_log, errlog_err_lvl_t default_lvl)
{
    if ((default_lvl <= ERRLOG_LVL_DEFAULT) || (default_lvl >= ERRLOG_LVL_LEN))
    {
        return ERRLOG_RET_ERR;
    }
    err_log->default_lvl = default_lvl;
    return ERRLOG_RET_OK;
}

errlog_ret_t errlog_read_init(errlog_t *err_log, errlog_err_lvl_t lvl)
{
    if ((lvl < ERRLOG_LVL_DEFAULT) || (lvl >= ERRLOG_LVL_LEN))
    {
        return ERRLOG_RET_ERR;
    }
    if (lvl == ERRLOG_LVL_DEFAULT)
    {
        lvl = err_log->default_lvl;
    }
    if (err_log->write_idx == err_log->first_idx)
    {
        return ERRLOG_RET_EMPTY;
    }
    /* Set read session */
    err_log->read_lvl = lvl;
    err_log->read_idx = err_log->write_idx;
    return ERRLOG_RET_OK;
}

errlog_ret_t errlog_read(errlog_t *err_log, errlog_entry_t *entry)
{
    errlog_ret_t ret = ERRLOG_RET_OK;
    entry->message_str[0] = '\0';
    entry->lvl = ERRLOG_LVL_DEFAULT;
    if ((err_log->read_lvl <= ERRLOG_LVL_DEFAULT) || (err_log->read_lvl >= ERRLOG_LVL_LEN))
    {
        ret = ERRLOG_RET_ERR;
    }
    else if (err_log->read_idx == err_log->first_idx)
    {
        ret = ERRLOG_RET_EMPTY;
    }
    else
    {
        while (err_log->read_idx != err_log->first_idx)
        {
            /* Move read index */
            if (err_log->read_idx != 0u)
            {
                err_log->read_idx--;
            }
            else
            {
                err_log->read_idx = ERRLOG_LOG_LEN;
            }
            if (err_log->log[err_log->read_idx].lvl <= err_log->read_lvl)
            {
                break;
            }
        }
        if (err_log->log[err_log->read_idx].lvl <= err_log->read_lvl)
        {
            (void)md_strlencpy((entry->message_str), err_log->log[err_log->read_idx].message_str, ERRLOG_MSG_LEN + 1);
            entry->lvl = err_log->log[err_log->read_idx].lvl;
        }
        else
        {
            ret = ERRLOG_RET_EMPTY;
        }
    }
    return (ret);
}

void errlog_read_done(errlog_t *err_log)
{
    err_log->read_lvl = ERRLOG_LVL_DEFAULT;
}

errlog_ret_t errlog_write(errlog_t *err_log, errlog_err_lvl_t lvl, const char *message)
{
    errlog_ret_t ret = ERRLOG_RET_OK;
    uint16_t len;
    if ((lvl < ERRLOG_LVL_DEFAULT) || (lvl >= ERRLOG_LVL_LEN))
    {
        return ERRLOG_RET_ERR;
    }
    if (lvl == ERRLOG_LVL_DEFAULT)
    {
        lvl = err_log->default_lvl;
    }
    len = md_strlencpy(err_log->log[err_log->write_idx].message_str, message, ERRLOG_MSG_LEN + 1);
    if (len == (ERRLOG_MSG_LEN + 1))
    {   
        /* Truncation */
        err_log->log[err_log->write_idx].message_str[ERRLOG_MSG_LEN - 1] = '.';
        err_log->log[err_log->write_idx].message_str[ERRLOG_MSG_LEN - 2] = '.';
        err_log->log[err_log->write_idx].message_str[ERRLOG_MSG_LEN - 3] = '.';
        ret = ERRLOG_RET_CHNG;
    }
    err_log->log[err_log->write_idx].lvl = lvl;
    /* Move write index */
    if (err_log->write_idx == ERRLOG_LOG_LEN)
    {
        err_log->write_idx = 0;
    }
    else
    {
        err_log->write_idx++;
    }
    /* Move first index */
    if (err_log->write_idx == err_log->first_idx)
    {
        if (err_log->first_idx == ERRLOG_LOG_LEN)
        {
            err_log->first_idx = 0;
        }
        else
        {
            err_log->first_idx++;
        }
    }
    return (ret);
}
