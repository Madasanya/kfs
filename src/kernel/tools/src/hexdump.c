#include "inttype.h"
#include "screen.h"
#include "kernel.h"
#include "str_utils.h"
#include "printk.h"

#define MAX_ADDR 0x800000 // 8MiB
#define ALIGMENT 16u
#define NUM_BASE "0123456789abcdef"

/**
 * @brief Validate and bound-check the hexdump parameters.
 *
 * @details Ensures the memory range specified by start_addr and len does not
 *          exceed MAX_ADDR (8MiB). If start_addr is beyond MAX_ADDR, returns 0.
 *          Otherwise, clamps len to fit within the valid address range.
 *
 * @param[in] start_addr Starting address to validate.
 * @param[in] len        Number of bytes to dump.
 * @return Adjusted length that fits within valid address range.
 */
static uint32_t input_chk(const void * start_addr, uint32_t len)
{
    uint32_t temp = MAX_ADDR;

    if (temp < (uint32_t)start_addr)
    {
        len = 0;
    }
    else
    {
        temp -= (uint32_t)start_addr;
        if (len > temp)
        {
            len = temp;
        }
    }
    
    return (len);
}

/**
 * @brief Format memory address as hexadecimal string.
 *
 * @details Converts the address to a hexadecimal string with leading zeros,
 *          formatted as "XXXXXXXX:" where X represents hex digits. The string
 *          is written to addr_line starting at position 2.
 *
 * @param[in]  addr      Address to format.
 * @param[out] addr_line Buffer to store formatted address string (minimum 11 bytes).
 */
static void addr_str_set(const void *addr, char *addr_line)
{
    uint32_t test = 0xf << 28;
    uint16_t cnt = 2;

    while (((uint32_t)addr & test) == 0)
    {
        addr_line[cnt] = '0';
        test >>= 4;
        cnt++;
    }
    md_uitoa_base(&(addr_line[cnt]), (uint32_t)addr, NUM_BASE);
    addr_line[10] = ':';
}

/**
 * @brief Prints n byte dump in format
 *
 * Output format: FF FF FF ...
 *
 * @param ptr Pointer whose address to print
 */
static void print_dump(const void *ptr, uint32_t n)
{
    uint8_t *ptr_c = (uint8_t*)ptr;
    uint8_t num;
    char byte[3] = {0};

    while (n != 0)
    {
        num = (uint8_t)*ptr_c;
        if ((num & 0xf0) == 0)
        {
            byte[0] = '0';
            md_itoa_base(&(byte[1]), num, NUM_BASE);
        }
        else
        {
            md_itoa_base(&(byte[0]), num, NUM_BASE);
        }
        screen_put_str(g_active_screen, byte);
        screen_put_char(g_active_screen, ' ');
        n--;
        if (n == 0u)
        {
            break;
        }
        ptr_c++;
    }
}

uint32_t md_hexdump_print(const void *start_addr, uint32_t len)
{
    const void *curr_addr;
    uint32_t offset = 0u;
    char addr_line[] = "0x        :         ";

    len = input_chk(start_addr, len);
    if(len == 0 || start_addr == NULL)
    {
        md_printk(KERN_ERR "Hexdump: Bad input");
        return(1);
    }
    screen_put_str(g_active_screen, "\nAddress \\ Offset     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");

    while (len > offset)
    {
        curr_addr = start_addr + offset;
        addr_str_set(curr_addr, addr_line);
        screen_put_str(g_active_screen ,addr_line);
        uint32_t print_size = ((len - offset) < ALIGMENT) ? (len - offset) : ALIGMENT;
        print_dump(curr_addr, print_size);
        screen_put_char(g_active_screen, '\n');
        offset += ALIGMENT;
    }
    return(0);
}

