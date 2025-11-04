#ifndef _MULTIBOOT_H_
#define _MULTIBOOT_H_

#include <stdint.h>

typedef struct {
    uint32_t magic;      /* 0x1BADB002 */
    uint32_t flags;      /* flags(bits) :   0 -     Page-align modules
                                            1 -     Memory info requierd
                                            2 -     Video info required
                                            3-15 -  Reserved (0)
                                            16 -    Address fields valid [advanced]
                                            17 -    Load address specified (default 1M) [advanced]
                                            18 -    Entry address specified (defult fro ELF header) [adavnced]
                                            19-31 - Reserved (0)  */
    uint32_t checksum;   /* -(magic + flags) */
} __attribute__((packed)) multiboot1_header_t; /* packed forces no padding in binary file */

void kernel(void);

#endif /* _MULTIBOOT_H_ */