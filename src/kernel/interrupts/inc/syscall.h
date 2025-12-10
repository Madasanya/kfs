#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#define SYS_WRITE       0u
#define SYS_READ        1u
#define SYS_COMMREAD    2u
#define SYS_ERRWRITE    3u
#define SYS_ERRPRINT    4u
#define SYS_HALT        5u
#define SYS_SCROLL      10u
#define SYS_SCREENSET   11u
#define SYS_COLORSET    12u
#define SYS_HEXDUMP     13u
#define SYS_STACKPRINT  14u


#define SYS_SCREENSET_MODE_ABSOLUTE 0u
#define SYS_SCREENSET_MODE_INCREASE 1u
#define SYS_SCREENSET_MODE_DECREASE 2u

#define SYS_COLORSET_MODE_ABSOLUTE 0u
#define SYS_COLORSET_MODE_INCREASE 1u
#define SYS_COLORSET_MODE_DECREASE 2u

#define SYS_SCROLL_UP 0u
#define SYS_SCROLL_DOWN 1u

#endif /* _SYSCALL_H_ */