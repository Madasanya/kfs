#define DEFINE_USER_FUNCTIONS
#include "user.h"
#include "nanoshell.h"
#include "inttype.h"

uint32_t var USER_DATA = 42;

USER_TEXT
void user_main(void)
{
    nanoshell_run();
}

