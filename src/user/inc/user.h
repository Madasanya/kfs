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

/**
 * @brief Main entry point for user mode program.
 *
 * @details Called after transitioning to user mode (ring 3).
 *          Implements the main user space application logic.
 */
void user_main(void) USER_TEXT;

#endif /* _USER_H_ */