#include "multiboot.h"
#include "start.h"
#include "gdt.h"
#include "idt.h"
#include "isr_dummy.h"
#include "isr_stub.h"
#include "pic.h"
#include "io.h"
#include "kernel.h"

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
        gdt_init();
        idt_init();
        
        // Set up syscall interrupt (int 0x80)
        idt_interrupt_set(0x80, &(idt_entry_data_t){
            .isr        = (uint32_t)isr_stub80,
            .attributes = IDT_ATTR_SYSCALL
        });
        
        // Set up keyboard interrupt (IRQ1 -> interrupt 0x21)
        idt_interrupt_set(0x21, &(idt_entry_data_t){
            .isr        = (uint32_t)isr_stub21,
            .attributes = IDT_ATTR_INTERRUPTS
        });
        
        idt_save();
        
        // Initialize PIC after IDT is fully configured
        // This remaps PIC vectors and unmasks IRQ1 (keyboard)
        pic_init();
        
        kernel();

    }
    else
    {
        return ;
    }
}