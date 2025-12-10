#ifndef _VA_ARG_H_
#define _VA_ARG_H_

/**
 * @file va_arg.h
 * @brief Provides macros for handling variable arguments, similar to stdarg.h but custom implementation.
 */

/**
 * @typedef va_list
 * @brief Type definition for variable argument list pointer.
 */
typedef char *va_list;

/**
 * @def ALIGNMENT
 * @brief Defines the alignment size used for pointer arithmetic in variable arguments.
 */
#define ALIGNMENT sizeof(int)

/**
 * @def ALIGN(type)
 * @brief Calculates the aligned size for a given type, rounding up to the next multiple of ALIGNMENT.
 *
 * @param type The type for which to compute the aligned size.
 */
#define ALIGN(type) (((sizeof(type) / ALIGNMENT) * ALIGNMENT) + (((sizeof(type) % ALIGNMENT) != 0u) ? ALIGNMENT : 0u))

/**
 * @def va_start(arg_p, prev_arg)
 * @brief Initializes the va_list to point just after the last fixed argument.
 *
 * @param arg_p The va_list to initialize.
 * @param prev_arg The last named (fixed) argument in the function.
 */
#define va_start(arg_p, prev_arg) (arg_p = ((char *)&(prev_arg) + ALIGN(prev_arg)))

/**
 * @def va_arg(arg_p, type)
 * @brief Retrieves the next argument of the specified type from the va_list and advances the pointer.
 *
 * @param arg_p The va_list from which to extract the argument.
 * @param type The type of the argument to retrieve.
 */
#define va_arg(arg_p, type) (arg_p += ALIGN(type), *((type *)(void *)(arg_p - ALIGN(type))))

/**
 * @def va_end(arg_p)
 * @brief Cleans up the va_list (no-op in this implementation).
 *
 * @param arg_p The va_list to clean up.
 */
#define va_end(arg_p) (arg_p = (void *)0)

#endif /* _VA_ARG_H_ */
