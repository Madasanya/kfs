void kernel(void)
{
    volatile char *txt_out = (volatile char*)0xB8000;

    txt_out[80] = '4';
    txt_out[81] = 0x20;
    txt_out[82] = '2';
    txt_out[83] = 0x20;
    while (1)
    {
        __asm__("hlt");
    }
}