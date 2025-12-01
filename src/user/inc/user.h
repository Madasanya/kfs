#ifndef _USER_H_
#define _USER_H_
// user_program.h
#ifdef DEFINE_USER_FUNCTIONS
    #define USER_TEXT __attribute__((section(".user_text")))
    #define USER_DATA __attribute__((section(".user_data")))
#else
    #define USER_TEXT
    #define USER_DATA
#endif

void user_main(void) USER_TEXT;          // prototype stays clean


#endif /* _USER_H_ */