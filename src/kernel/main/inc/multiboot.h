#ifndef _MULTIBOOT_H_
#define _MULTIBOOT_H_

/** 
 * @brief Main kernel function.
 * 
 * @details Entry point called by the bootloader. Initializes kernel
 *          subsystems and starts the main kernel loop.
 */
void kernel(void);

#endif /* _MULTIBOOT_H_ */