#ifndef _USERMODE_H_
#define _USERMODE_H_

/**
 * @brief Enter user mode and execute code at the specified entry point.
 *
 * @details Performs a privilege level transition from ring 0 (kernel) to
 *          ring 3 (user) and jumps to the given entry point. This function
 *          does not return to kernel mode under normal circumstances.
 *
 * @param[in] entry_point Pointer to the user mode function to execute.
 */
extern void user_enter(void *entry_point);

#endif /* _USERMODE_H_ */