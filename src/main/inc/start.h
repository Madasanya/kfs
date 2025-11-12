#ifndef _START_H_
#define _START_H_

#include "inttype.h"

#define SCREEN_WIDTH 80u
#define SCREEN_HEIGHT 25u

/** @brief Structur for holding informations about kernel. */
typedef struct __attribute__((packed)) kernel_ver_s
{
    uint8_t major_ver;
    uint8_t minor_ver;
} kernel_ver_t;

/** @brief Current version macros. */
#define MAJOR_VER 0x01
#define MINOR_VER 0x00

extern kernel_ver_t version;

#endif /* _START_H_ */
