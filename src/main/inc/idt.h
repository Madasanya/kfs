#ifndef IDT_H
#define IDT_H

#include "inttype.h"

#define IDT_PRESENT         0x80

#define GATE_32BIT_INT      0x0E
#define GATE_32BIT_TRAP     0x0F

#define DPL_0               0x00
#define DPL_3               0x60    // 11 << 5

// The only 3 you will ever use:
#define KERNEL_INT_GATE     (IDT_PRESENT | DPL_0 | GATE_32BIT_INT)   // 0x8E
#define KERNEL_TRAP_GATE    (IDT_PRESENT | DPL_0 | GATE_32BIT_TRAP)  // 0x8F (rare)
#define USER_TRAP_GATE      (IDT_PRESENT | DPL_3 | GATE_32BIT_TRAP)  // 0xEE ← syscall

#define IDT_ATTR_INTERRUPTS KERNEL_INT_GATE
#define IDT_ATTR_SYSCALL    USER_TRAP_GATE

typedef struct {
    uint32_t isr;
    uint8_t  attributes;
} idt_entry_data_t;


void idt_save(void);

void idt_init(void);
void idt_interrupt_set(uint8_t num, const idt_entry_data_t *isr_data);

#endif