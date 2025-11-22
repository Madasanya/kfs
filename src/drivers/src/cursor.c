#include "inttype.h"
#include "io.h"
#include "cursor.h"
#include "screen_settings.h"

void cursor_set_style(cursor_setting_t settings)
{
	md_outb(0x3D4, 0x0A);
	md_outb(0x3D5, (md_inb(0x3D5) & 0xE0) | settings.cursor_start);

	md_outb(0x3D4, 0x0B);
	md_outb(0x3D5, (md_inb(0x3D5) & 0xE0) | settings.cursor_end);
}

void cursor_enable()
{
	md_outb(0x3D4, 0x0A);
	md_outb(0x3D5, (md_inb(0x3D5) & ~0x20));
}

void cursor_disable()
{
	md_outb(0x3D4, 0x0A);
	md_outb(0x3D5, (md_inb(0x3D5) | 0x20));
}

void cursor_update(uint16_t x, uint16_t y)
{
	uint16_t pos = y * SCREEN_WIDTH + x;

	md_outb(0x3D4, 0x0F);
	md_outb(0x3D5, (uint8_t) (pos & 0xFF));
	md_outb(0x3D4, 0x0E);
	md_outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}