void kernel(void)
{
    volatile char *txt_out = (volatile char*)0xB8000;

    txt_out[0] = '4';
    txt_out[1] = 0x20;
    txt_out[2] = '2';
    txt_out[3] = 0x20;
    while (1)
    {
        __asm__("hlt");
    }
}