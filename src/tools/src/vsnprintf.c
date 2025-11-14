#include "inttype.h"
#include "va_arg.h"
#include "str_utils.h"
#define DEC_BASE "0123456789"
#define HEX_BASE_UPPER "0123456789ABCDEF"
#define HEX_BASE_LOWER "0123456789abcdef"

/**
 * @brief Appends a single character to the buffer if space is available.
 *
 * @details This function checks if the current buffer position is before the end,
 * and if so, appends the character `c` to the buffer and advances the pointer.
 *
 * @param [inout] buf_ptr Pointer to the current position in the buffer.
 * @param [in] end Pointer to the end of the buffer.
 * @param [in] c The character to append.
 */
static void append_char(char **buf_ptr, char *end, char c)
{
    if (*buf_ptr < end) {
        *(*buf_ptr)++ = c;
    }
}

/**
 * @brief Appends a string to the buffer until the end or null terminator.
 *
 * @details This function copies characters from the string `s` to the buffer
 * pointed to by `buf_ptr`, advancing the pointer, until either the end of the
 * buffer is reached or the null terminator of `s` is encountered.
 *
 * @param [inout] buf_ptr Pointer to the current position in the buffer.
 * @param [in] end Pointer to the end of the buffer.
 * @param [in] s The string to append.
 */
static void append_string(char **buf_ptr, char *end, const char *s)
{
    while (*s && *buf_ptr < end) {
        *(*buf_ptr)++ = *s++;
    }
}

/**
 * @brief Appends a number string to the buffer.
 *
 * @details This function delegates to append_string to copy the contents of
 * `numbuf` (assumed to be a null-terminated string representation of a number)
 * to the buffer.
 *
 * @param [inout] buf_ptr Pointer to the current position in the buffer.
 * @param [in] end Pointer to the end of the buffer.
 * @param [in] numbuf The buffer containing the number as a string.
 */
static inline void append_number(char **buf_ptr, char *end, const char *numbuf)
{
    append_string(buf_ptr, end, numbuf);
}

/**
 * @brief Handles conversion and appending of a signed 32-bit integer in decimal.
 *
 * @details Converts the signed integer `val` to a decimal string using itoa_base
 * and appends it to the buffer via append_number.
 *
 * @param [inout] buf_ptr Pointer to the current position in the buffer.
 * @param [in] end Pointer to the end of the buffer.
 * @param [in] val The signed 32-bit integer to convert and append.
 * @param [out] numbuf Temporary buffer for the number string.
 */
static inline void handle_signed(char **buf_ptr, char *end, int32_t val, char numbuf[32])
{
    itoa_base(numbuf, val, DEC_BASE);
    append_number(buf_ptr, end, numbuf);
}

/**
 * @brief Handles conversion and appending of an unsigned 32-bit integer in decimal.
 *
 * @details Converts the unsigned integer `val` to a decimal string using uitoa_base
 * and appends it to the buffer via append_number.
 *
 * @param [inout] buf_ptr Pointer to the current position in the buffer.
 * @param [in] end Pointer to the end of the buffer.
 * @param [in] val The unsigned 32-bit integer to convert and append.
 * @param [out] numbuf Temporary buffer for the number string.
 */
static inline void handle_unsigned(char **buf_ptr, char *end, uint32_t val, char numbuf[32])
{
    uitoa_base(numbuf, val, DEC_BASE);
    append_number(buf_ptr, end, numbuf);
}

/**
 * @brief Handles conversion and appending of a signed 64-bit integer in decimal.
 *
 * @details Converts the signed long long `val` to a decimal string using lltoa_base
 * and appends it to the buffer via append_number.
 *
 * @param [inout] buf_ptr Pointer to the current position in the buffer.
 * @param [in] end Pointer to the end of the buffer.
 * @param [in] val The signed 64-bit integer to convert and append.
 * @param [out] numbuf Temporary buffer for the number string.
 */
static inline void handle_signedll(char **buf_ptr, char *end, int64_t val, char numbuf[32])
{
    lltoa_base(numbuf, val, DEC_BASE);
    append_number(buf_ptr, end, numbuf);
}

/**
 * @brief Handles conversion and appending of an unsigned 64-bit integer in decimal.
 *
 * @details Converts the unsigned long long `val` to a decimal string using ulltoa_base
 * and appends it to the buffer via append_number.
 *
 * @param [inout] buf_ptr Pointer to the current position in the buffer.
 * @param [in] end Pointer to the end of the buffer.
 * @param [in] val The unsigned 64-bit integer to convert and append.
 * @param [out] numbuf Temporary buffer for the number string.
 */
static inline void handle_unsignedll(char **buf_ptr, char *end, uint64_t val, char numbuf[32])
{
    ulltoa_base(numbuf, val, DEC_BASE);
    append_number(buf_ptr, end, numbuf);
}

/**
 * @brief Handles conversion and appending of a 32-bit integer in lowercase hexadecimal.
 *
 * @details Converts the integer `val` to a lowercase hex string using itoa_base
 * with HEX_BASE_LOWER and appends it to the buffer via append_number.
 *
 * @param [inout] buf_ptr Pointer to the current position in the buffer.
 * @param [in] end Pointer to the end of the buffer.
 * @param [in] val The 32-bit integer to convert and append.
 * @param [out] numbuf Temporary buffer for the number string.
 */
static inline void handle_lower_hex(char **buf_ptr, char *end, int32_t val, char numbuf[32])
{
    itoa_base(numbuf, val, HEX_BASE_LOWER);
    append_number(buf_ptr, end, numbuf);
}

/**
 * @brief Handles conversion and appending of a 32-bit integer in uppercase hexadecimal.
 *
 * @details Converts the integer `val` to an uppercase hex string using itoa_base
 * with HEX_BASE_UPPER and appends it to the buffer via append_number.
 *
 * @param [inout] buf_ptr Pointer to the current position in the buffer.
 * @param [in] end Pointer to the end of the buffer.
 * @param [in] val The 32-bit integer to convert and append.
 * @param [out] numbuf Temporary buffer for the number string.
 */
static inline void handle_upper_hex(char **buf_ptr, char *end, int32_t val, char numbuf[32])
{
    itoa_base(numbuf, val, HEX_BASE_UPPER);
    append_number(buf_ptr, end, numbuf);
}

/**
 * @brief Handles appending of a string, or "(null)" if the string is NULL.
 *
 * @details If `str` is NULL, appends "(null)" to the buffer; otherwise, appends
 * the contents of `str` using append_string.
 *
 * @param [inout] buf_ptr Pointer to the current position in the buffer.
 * @param [in] end Pointer to the end of the buffer.
 * @param [in] str The string to append, or NULL.
 */
static void handle_string(char **buf_ptr, char *end, const char *str) {
    if (!str)
    {
        append_string(buf_ptr, end, "(null)");
    }
    else
    {
        append_string(buf_ptr, end, str);
    }
}

/**
 * @brief Handles formatting and appending of a pointer value.
 *
 * @details For %p, appends "0x" followed by the lowercase hex representation.
 * For %ph or %pH, appends a hex dump of 8 bytes in lowercase or uppercase,
 * respectively, enclosed in brackets with spaces between bytes.
 * Advances `fmt` if 'h' or 'H' is consumed.
 *
 * @param [inout] buf_ptr Pointer to the current position in the buffer.
 * @param [in] end Pointer to the end of the buffer.
 * @param [in] ptr The pointer value to format.
 * @param [out] numbuf Temporary buffer for number strings.
 * @param [in] fmt Current position in the format string.
 * @return Updated position in the format string.
 */
static const char *handle_pointer(char **buf_ptr, char *end, uintptr_t ptr, char numbuf[32], const char *fmt)
{
    char subtype = *fmt; // Peek for extension
    if ((subtype == 'h' || subtype == 'H') && (ptr != 0u))
    { // %ph or %pH: hex dump (lowercase/uppercase)
        fmt++; /* Consume h or H */
        append_char(buf_ptr, end, '[');
        for (int i = 0; i < 4 && *buf_ptr < end; i++) // Dump 8 bytes
        {
            unsigned char byte = ((uint8_t *)ptr)[i];
            if (subtype == 'h')
            {
                uitoa_base(numbuf, byte, HEX_BASE_LOWER);
            }
            else
            {
                uitoa_base(numbuf, byte, HEX_BASE_UPPER);
            }
            if (md_strlen(numbuf) == 1)
            {
                append_char(buf_ptr, end, '0'); // Pad to 2 digits
            }
            append_number(buf_ptr, end, numbuf);
            if (i < 7) {
                append_char(buf_ptr, end, ' ');
            }
        }
        append_char(buf_ptr, end, ']');
    }
    else
    {
        append_char(buf_ptr, end, '0');
        append_char(buf_ptr, end, 'x');
        uitoa_base(numbuf, ptr, HEX_BASE_LOWER);
        append_number(buf_ptr, end, numbuf);
    }
    return fmt;
}

int vsnprintf_args(char *buf, uint16_t size, const char *fmt, va_list args)
{
    char *start = buf;
    char *end = buf + size - 1;
    char numbuf[32];
    int32_t temp_l;
    uint32_t temp_ul;
    int64_t temp_ll;
    uint64_t temp_ull;
    char temp_c;
    char *temp_str;
    uintptr_t temp_ptr;
    while (*fmt && buf < end) {
        if (*fmt != '%') {
            *buf++ = *fmt++;
            continue;
        }
        fmt++;
        char spec = *fmt++;
        switch (spec) {
        case 'd':
        case 'i':
            temp_l = va_arg(args, int32_t);
            handle_signed(&buf, end, temp_l, numbuf);
            break;
        case 'u':
            temp_ul = va_arg(args, uint32_t);
            handle_unsigned(&buf, end, temp_ul, numbuf);
            break;
        case 'l':
            if (*fmt == 'd')
            {
                temp_l = va_arg(args, int32_t);
                handle_signed(&buf, end, temp_l, numbuf);
            }
            else if (*fmt == 'u')
            {
                temp_ul = va_arg(args, uint32_t);
                handle_unsigned(&buf, end, temp_ul, numbuf);
            }
            else if (*fmt == 'l')
            {
                fmt++;
                if (*fmt == 'd')
                {
                    temp_ll = va_arg(args, int64_t);
                    handle_signedll(&buf, end, temp_ll, numbuf);
                }
                else if (*fmt == 'u')
                {
                    temp_ull = va_arg(args, uint64_t);
                    handle_unsignedll(&buf, end, temp_ull, numbuf);
                }
                else
                {
                    append_char(&buf, end, '%');
                    append_char(&buf, end, 'l');
                    append_char(&buf, end, 'l');
                    append_char(&buf, end, *fmt);
                }
            }
            else
            {
                append_char(&buf, end, '%');
                append_char(&buf, end, 'l');
                append_char(&buf, end, *fmt);
            }
            fmt++;
            break;
        case 'x':
            temp_l = va_arg(args, int32_t);
            handle_lower_hex(&buf, end, temp_l, numbuf);
            break;
        case 'X':
            temp_l = va_arg(args, int32_t);
            handle_upper_hex(&buf, end, temp_l, numbuf);
            break;
        case 's':
            temp_str = va_arg(args, char *);
            handle_string(&buf, end, temp_str);
            break;
        case 'c':
            temp_c = va_arg(args, char);
            append_char(&buf, end, temp_c);
            break;
        case 'p':
            temp_ptr = va_arg(args, uintptr_t);
            fmt = handle_pointer(&buf, end, temp_ptr, numbuf, fmt);
            break;
        case '%':
            append_char(&buf, end, '%');
            break;
        default:
            append_char(&buf, end, '%');
            append_char(&buf, end, spec);
            break;
        }
    }
    *buf = '\0';
    return buf - start;
}

int vsnprintf(char *buf, uint16_t size, const char *fmt, ...)
{
    va_list args;
    int ret;
    va_start(args, fmt);
    ret = vsnprintf_args(buf, size, fmt, args);
    va_end(args);
    return (ret);
}