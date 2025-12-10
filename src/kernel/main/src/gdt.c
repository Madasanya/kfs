#include "gdt.h"

#define GDTBASE    0x00000800
#define NUM_GDT_ENTRIES 8

// Each define here is for a specific flag in the descriptor.
// Refer to the intel documentation for a description of what each one does.
#define SEG_DESCTYPE(x)  ((x) << 0x04u)  // Descriptor type (0 for system, 1 for code/data)
#define SEG_PRES(x)      ((x) << 0x07u) // Present
#define SEG_SAVL(x)      ((x) << 0x0Cu) // Available for system use
#define SEG_LONG(x)      ((x) << 0x0Du) // Long mode
#define SEG_SIZE(x)      ((x) << 0x0Eu) // Size (0 for 16-bit, 1 for 32)
#define SEG_GRAN(x)      ((x) << 0x0Fu) // Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)
#define SEG_PRIV(x)     (((x) &  0x03u) << 0x05u)   // Set privilege level (0 - 3)
 
#define SEG_DATA_RD        0x00u // Read-Only
#define SEG_DATA_RDA       0x01u // Read-Only, accessed
#define SEG_DATA_RDWR      0x02u // Read/Write
#define SEG_DATA_RDWRA     0x03u // Read/Write, accessed
#define SEG_DATA_RDEXPD    0x04u // Read-Only, expand-down
#define SEG_DATA_RDEXPDA   0x05u // Read-Only, expand-down, accessed
#define SEG_DATA_RDWREXPD  0x06u // Read/Write, expand-down
#define SEG_DATA_RDWREXPDA 0x07u // Read/Write, expand-down, accessed
#define SEG_CODE_EX        0x08u // Execute-Only
#define SEG_CODE_EXA       0x09u // Execute-Only, accessed
#define SEG_CODE_EXRD      0x0Au // Execute/Read
#define SEG_CODE_EXRDA     0x0Bu // Execute/Read, accessed
#define SEG_CODE_EXC       0x0Cu // Execute-Only, conforming
#define SEG_CODE_EXCA      0x0Du // Execute-Only, conforming, accessed
#define SEG_CODE_EXRDC     0x0Eu // Execute/Read, conforming
#define SEG_CODE_EXRDCA    0x0Fu // Execute/Read, conforming, accessed

#define SEG_TSS_AVAIL      0x09u // Available 32-bit TSS with busy flag 0
 
#define GDT_CODE_PL0 SEG_DESCTYPE(1u) | SEG_PRES(1u) | SEG_SAVL(0u) | \
                     SEG_LONG(0u)     | SEG_SIZE(1u) | SEG_GRAN(1u) | \
                     SEG_PRIV(0u)     | SEG_CODE_EXRD
 
#define GDT_DATA_PL0 SEG_DESCTYPE(1u) | SEG_PRES(1u) | SEG_SAVL(0u) | \
                     SEG_LONG(0u)     | SEG_SIZE(1u) | SEG_GRAN(1u) | \
                     SEG_PRIV(0u)     | SEG_DATA_RDWR
 
#define GDT_CODE_PL3 SEG_DESCTYPE(1u) | SEG_PRES(1u) | SEG_SAVL(0u) | \
                     SEG_LONG(0u)     | SEG_SIZE(1u) | SEG_GRAN(1u) | \
                     SEG_PRIV(3u)     | SEG_CODE_EXRD
 
#define GDT_DATA_PL3 SEG_DESCTYPE(1u) | SEG_PRES(1u) | SEG_SAVL(0u) | \
                     SEG_LONG(0u)     | SEG_SIZE(1u) | SEG_GRAN(1u) | \
                     SEG_PRIV(3u)     | SEG_DATA_RDWR
                     
#define GDT_BSS_PL0 SEG_DESCTYPE(1u) | SEG_PRES(1u) | SEG_SAVL(0u) | \
                    SEG_LONG(0u)     | SEG_SIZE(1u) | SEG_GRAN(1u) | \
                    SEG_PRIV(0u)     | SEG_DATA_RDWR

#define GDT_BSS_PL3 SEG_DESCTYPE(1u) | SEG_PRES(1u) | SEG_SAVL(0u) | \
                    SEG_LONG(0u)     | SEG_SIZE(1u) | SEG_GRAN(1u) | \
                    SEG_PRIV(3u)     | SEG_DATA_RDWR

#define GDT_TSS     SEG_DESCTYPE(0u) | SEG_PRES(1u) | SEG_SAVL(0u) | \
                    SEG_LONG(0u)     | SEG_SIZE(0u) | SEG_GRAN(0u) | \
                    SEG_PRIV(0u)     | SEG_TSS_AVAIL

                    
                    
#define GRANULARITY_PAGE_DIV 4096u
#define GRANULARITY_BYTE_DIV 1u  

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__ ((packed)) gdt_reg_t;

typedef struct {
    uint16_t lim0_15;
    uint16_t base0_15;
    uint8_t base16_23;
    uint8_t flags_access;
    uint8_t lim16_19:4;
    uint8_t flags_others:4;
    uint8_t base24_31;
}  __attribute__ ((packed)) gdt_entry_t;

typedef struct {
    uint32_t base;
    uint32_t limit;
    uint16_t flags;
} gdt_segment_data_t;

typedef struct {
	uint32_t prev_tss; // The previous TSS - with hardware task switching these form a kind of backward linked list.
	uint32_t esp0;     // The stack pointer to load when changing to kernel mode.
	uint32_t ss0;      // The stack segment to load when changing to kernel mode.
	// Everything below here is unused.
	uint32_t esp1; // esp and ss 1 and 2 would be used when switching to rings 1 or 2.
	uint32_t ss1;
	uint32_t esp2;
	uint32_t ss2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;
	uint32_t cs;
	uint32_t ss;
	uint32_t ds;
	uint32_t fs;
	uint32_t gs;
	uint32_t ldt;
	uint16_t trap;
	uint16_t iomap_base;
} __attribute__ ((packed)) tss_entry_t;

gdt_reg_t gdt_reg = {0};

tss_entry_t tss __attribute__((section(".tss")));

extern uint32_t _user_stack_start[];
extern uint32_t _user_stack_end[];
extern uint32_t _user_data_start[];
extern uint32_t _user_data_end[];
extern uint32_t _user_text_start[];
extern uint32_t _user_text_end[];
extern uint32_t _kernel_stack_start[];
extern uint32_t _kernel_stack_end[];
extern uint32_t _kernel_data_start[];
extern uint32_t _kernel_data_end[];
extern uint32_t _kernel_text_start[];
extern uint32_t _kernel_text_end[];

extern uint32_t kernel_stack_top[];

extern void gdt_flush(gdt_reg_t *gdt);
extern void gdt_save(gdt_reg_t *gdt);

extern void tss_flush(void);


/**
 * @brief Create a GDT entry from segment data.
 *
 * @details Populates a gdt_entry_t structure with base address, limit, and
 *          access flags from the provided segment data. Splits the 32-bit base
 *          and 20-bit limit across multiple fields as required by the GDT format.
 *
 * @param[out] entry    Pointer to the GDT entry structure to populate.
 * @param[in]  seg_data Pointer to the segment configuration data.
 */
static void gdt_entry_create(gdt_entry_t* entry, const gdt_segment_data_t *seg_data)
{
    entry->lim0_15      = (seg_data->limit & 0xFFFF);
    entry->base0_15     = (seg_data->base & 0xFFFF);
    entry->base16_23    = (seg_data->base >> 16) & 0xFF;
    entry->flags_access = (seg_data->flags & 0xFF);
    entry->lim16_19     = (seg_data->limit >> 16) & 0x0F;
    entry->flags_others = (seg_data->flags >> 0xC) & 0x0F;
    entry->base24_31    = (seg_data->base >> 24) & 0xFF;
}

void gdt_init(void)
{
    gdt_entry_t* gdt = (gdt_entry_t*)GDTBASE;

    const gdt_segment_data_t null_seg = {0, 0, 0};
    const gdt_segment_data_t code_seg_pl0 = {0x00000000, 0xFFFFF, GDT_CODE_PL0};
    const gdt_segment_data_t data_seg_pl0 = {0x00000000, 0xFFFFF, GDT_DATA_PL0};
    const gdt_segment_data_t stack_seg_pl0 = {0x00000000, 0xFFFFF, GDT_BSS_PL0};
    const gdt_segment_data_t code_seg_pl3 = {0x00200000, 0xFFFFF, GDT_CODE_PL3};
    const gdt_segment_data_t data_seg_pl3 = {0x00200000, 0xFFFFF, GDT_DATA_PL3};
    const gdt_segment_data_t stack_seg_pl3 = {0x00200000, 0xFFFFF, GDT_BSS_PL3};
    // TSS segment
    const gdt_segment_data_t tss_seg = {(uint32_t)&tss, 0x00000067, GDT_TSS};
    
    gdt_entry_create(&gdt[0], &null_seg);
    gdt_entry_create(&gdt[1], &code_seg_pl0);
    gdt_entry_create(&gdt[2], &data_seg_pl0);
    gdt_entry_create(&gdt[3], &stack_seg_pl0);
    gdt_entry_create(&gdt[4], &code_seg_pl3);
    gdt_entry_create(&gdt[5], &data_seg_pl3);
    gdt_entry_create(&gdt[6], &stack_seg_pl3);
    gdt_entry_create(&gdt[7], &tss_seg);
    
    gdt_save(&gdt_reg);

    gdt_entry_t* gdt_old = (gdt_entry_t*)gdt_reg.base;
    gdt_old = gdt_old;

    gdt_reg.limit = (NUM_GDT_ENTRIES * sizeof(gdt_entry_t)) - 1;
    gdt_reg.base  = GDTBASE;

    // Load the new GDT FIRST
    gdt_flush(&gdt_reg);
    

    //Initialize TSS
    tss_entry_t* tss_ptr = &tss;
    for (uint32_t i = 0; i < sizeof(tss_entry_t); i++)
    {
        ((uint8_t*)tss_ptr)[i] = 0; // Zero out the TSS
    }
    tss_ptr->ss0 = 0x18; // Kernel data segment selector
    tss_ptr->esp0 = (uint32_t)kernel_stack_top; // Stack pointer for kernel mode
    tss_flush();

    
}
