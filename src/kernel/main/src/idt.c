#include "idt.h"
#include "isr_dummy.h"

#define KERNEL_CS 0x08
#define NUM_IDT_ENTRIES 256

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__ ((packed)) idt_reg_t;

typedef struct {
	uint16_t    isr0_15;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t     reserved;     // Set to zero
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isr16_31;     // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;

__attribute__((aligned(0x8))) 
static idt_entry_t idt[256] = {0}; // Create an array of IDT entries; aligned for performance

idt_reg_t idt_reg;

extern void idt_flush(idt_reg_t *idt);

void idt_interrupt_set(uint8_t num, const idt_entry_data_t *isr_data)
{
    idt[num].isr0_15    = isr_data->isr & 0xFFFF;
    idt[num].kernel_cs  = KERNEL_CS;
    idt[num].reserved   = 0u;
    idt[num].attributes = isr_data->attributes;
    idt[num].isr16_31   = (isr_data->isr >> 16) & 0xFFFF;
}

void idt_init(void)
{
    idt_entry_data_t default_isr = {.isr        = (uint32_t)isr_dummy_nonblocking,
                                    .attributes = IDT_ATTR_INTERRUPTS};
    
    for (uint16_t i = 0; i < NUM_IDT_ENTRIES; i++)
    {
        idt_interrupt_set((uint8_t)i, &default_isr);
    }
    idt_reg.limit = (NUM_IDT_ENTRIES * sizeof(idt_entry_t)) - 1;
    idt_reg.base  = (uint32_t)&idt;
}

void idt_save(void)
{
    idt_flush(&idt_reg);
}
