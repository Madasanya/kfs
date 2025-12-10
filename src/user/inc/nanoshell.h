#ifndef _NANOSHELL_H
#define _NANOSHELL_H

/**
 * @brief Run the nanoshell command interpreter.
 *
 * @details Starts the main loop of the shell, accepting and processing
 *          user commands until exit. Handles command parsing, execution,
 *          and basic line editing.
 */
void nanoshell_run(void);

#endif /* _NANOSHELL_H */