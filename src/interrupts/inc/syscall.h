#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#define SYS_WRITE       0u
#define SYS_READ        1u
#define SYS_ERRWRITE    3u
#define SYS_ERRPRINT    4u

#define SYS_SCROLL 10u
#define SYS_SCREENSET 11u
#define SYS_COLORSET 12u


#define SYS_SCREENSET_ABSOLUTE 0u
#define SYS_SCREENSET_INCREASE 1u
#define SYS_SCREENSET_DECREASE 2u

#define SYS_COLORSET_ABSOLUTE 0u
#define SYS_COLORSET_INCREASE 1u
#define SYS_COLORSET_DECREASE 2u


#define SYS_SCROLL_UP 0u
#define SYS_SCROLL_DOWN 1u

#endif /* _SYSCALL_H_ */