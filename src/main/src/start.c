#include "multiboot.h"
#include "start.h"
#include "gdt.h"
#include "idt.h"
#include "isr_dummy.h"
#include "isr_stub.h"
#include "isr_irq.h"
#include "pic.h"
#include "io.h"

kernel_ver_t version __attribute__((section(".kernel_info"))) = \
{
    .major_ver = MAJOR_VER,
    .minor_ver = MINOR_VER
};

errlog_t errlog;

void start_multiboot1(uint32_t magic)
{
    errlog_init(&errlog, ERRLOG_LVL_DEFAULT);
    if (magic == 0x2BADB002)
    {
        init_gdt();
        idt_init();
        
        // Set up syscall interrupt (int 0x80)
        idt_interrupt_set(0x80, &(idt_entry_data_t){
            .isr        = (uint32_t)isr_stub80,
            .attributes = IDT_ATTR_SYSCALL
        });
        
        // Remap PIC to use interrupts 0x20-0x2F (32-47)
        // This avoids conflicts with CPU exceptions (0-31)
        PIC_remap(0x20, 0x28);
        
        // Set up keyboard interrupt (IRQ1 -> interrupt 0x21)
        idt_interrupt_set(0x21, &(idt_entry_data_t){
            .isr        = (uint32_t)isr_irq1_keyboard,
            .attributes = IDT_ATTR_INTERRUPTS
        });
        
        idt_save();
        
        // Enable IRQ1 (keyboard) by unmasking it in PIC
        IRQ_clear_mask(1);
        
        // Simple keyboard initialization - just reset it
        // Flush any existing data
        while (md_inb(0x64) & 0x01) {
            md_inb(0x60);
        }
        
        // Reset keyboard
        md_outb(0x60, 0xFF);  // Reset command to keyboard
        // Wait a bit for reset
        for (volatile int i = 0; i < 100000; i++);
        
        // Drain buffer after reset
        while (md_inb(0x64) & 0x01) {
            md_inb(0x60);
        }
        
        // // Enable interrupts
        // __asm__ volatile("sti");
        
        kernel();

    }
    else
    {
        return ;
    }
}