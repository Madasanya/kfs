#ifndef _START_H_
#define _START_H_

#include "inttype.h"
#include "errlog.h"

/** @brief Structure for holding information about kernel. */
typedef struct __attribute__((packed)) kernel_ver_s
{
    uint8_t major_ver;
    uint8_t minor_ver;
} kernel_ver_t;

/** @brief Current version macros. */
#define MAJOR_VER 0x02
#define MINOR_VER 0x01

extern kernel_ver_t version;
extern errlog_t errlog;

#endif /* _START_H_ */
