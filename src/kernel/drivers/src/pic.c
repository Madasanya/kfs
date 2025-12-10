#include "inttype.h"
#include "io.h"

#define PIC1		    0x20		/* IO base address for master PIC */
#define PIC2		    0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	    (PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	    (PIC2+1)

#define PIC_READ_IRR    0x0a    /* OCW3 irq ready next CMD read */
#define PIC_READ_ISR    0x0b    /* OCW3 irq service next CMD read */

#define PIC_EOI		    0x20		/* End-of-interrupt command code */

void pic_sendEOI(uint8_t irq)
{
	if(irq >= 8)
		md_outb(PIC2_COMMAND,PIC_EOI);
	
	md_outb(PIC1_COMMAND,PIC_EOI);
}

/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */

#define ICW1_ICW4	    0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	    0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	    0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	    0x10		/* Initialization - required! */

#define ICW4_8086	    0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	    0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	    0x10		/* Special fully nested (not) */

#define CASCADE_IRQ     2

/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/
void pic_remap(int offset1, int offset2)
{
	md_outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	md_io_wait();
	md_outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	md_io_wait();
	md_outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	md_io_wait();
	md_outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	md_io_wait();
	md_outb(PIC1_DATA, 1 << CASCADE_IRQ);        // ICW3: tell Master PIC that there is a slave PIC at IRQ2
	md_io_wait();
	md_outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	md_io_wait();
	
	md_outb(PIC1_DATA, ICW4_8086);               // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	md_io_wait();
	md_outb(PIC2_DATA, ICW4_8086);
	md_io_wait();

	// Unmask both PICs.
	md_outb(PIC1_DATA, 0xff);
	md_outb(PIC2_DATA, 0xff);
    md_io_wait();

}

void pic_disable(void) {
    md_outb(PIC1_DATA, 0xff);
    md_outb(PIC2_DATA, 0xff);
}

void irq_set_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = md_inb(port) | (1 << IRQline);
    md_outb(port, value);        
}

void irq_clear_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = md_inb(port) & ~(1 << IRQline);
    md_outb(port, value);        
}

/**
 * @brief Read IRQ register from cascaded PICs.
 *
 * @details Helper function that reads either the Interrupt Request Register (IRR)
 *          or In-Service Register (ISR) from both PICs. The PICs are cascaded,
 *          with PIC2 handling IRQs 8-15 and PIC1 handling IRQs 0-7 (with IRQ2
 *          being the cascade connection).
 *
 * @param[in] ocw3 Operation Control Word 3 value (PIC_READ_IRR or PIC_READ_ISR).
 * @return Combined 16-bit value: upper 8 bits from PIC2, lower 8 bits from PIC1.
 */
static uint16_t pic_get_irq_reg(int ocw3)
{
    /* OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
     * represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain */
    md_outb(PIC1_COMMAND, ocw3);
    md_outb(PIC2_COMMAND, ocw3);
    return (md_inb(PIC2_COMMAND) << 8) | md_inb(PIC1_COMMAND);
}

/* Returns the combined value of the cascaded PICs irq request register */
uint16_t pic_get_irr(void)
{
    return pic_get_irq_reg(PIC_READ_IRR);
}

/* Returns the combined value of the cascaded PICs in-service register */
uint16_t pic_get_isr(void)
{
    return pic_get_irq_reg(PIC_READ_ISR);
}

void pic_init(void)
{  
        // Remap PIC to use interrupts 0x20-0x2F (32-47)
        // This avoids conflicts with CPU exceptions (0-31)
        pic_remap(0x20, 0x28);

        // Enable IRQ1 (keyboard) by unmasking it in PIC
        irq_clear_mask(1);
        
        // Simple keyboard initialization - just reset it
        // Flush any existing data
        while (md_inb(0x64) & 0x01) {
            md_inb(0x60);
        }
        
        // Reset keyboard
        md_outb(0x60, 0xFF);  // Reset command to keyboard
        // Wait a bit for reset
        md_io_wait();
        
        // Drain buffer after reset
        while (md_inb(0x64) & 0x01) {
            md_inb(0x60);
        }
}