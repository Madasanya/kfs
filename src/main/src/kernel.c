#include "str_utils.h"
#include "io.h"
#include "keyboard.h"

void kernel(void)
{
    char temp_c;
    uint8_t ret;
    keyboard_t keyboard = {0};

    md_put_str("42");
    while (1)
    {
        keyboard_run(&keyboard);
        ret = keyboard_char_get(&keyboard, &temp_c);
        if (ret == 0)
        {
            continue;
        }
        md_put_char(temp_c);
    }

}