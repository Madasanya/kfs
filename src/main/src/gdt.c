#include "gdt.h"

#define GDTBASE    0x00000800

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

static void gdt_entry_create(gdt_entry_t* entry, const gdt_segment_data_t *seg_data)
{
    entry->lim0_15      = (seg_data->limit & 0xFFFF);
    entry->base0_15     = (seg_data->base & 0xFFFF);
    entry->base16_23    = (seg_data->base >> 16) & 0xFF;
    entry->flags_access = (seg_data->flags & 0xFF);
    entry->lim16_19     = (seg_data->limit >> 16) & 0x0F;
    entry->flags_others = (seg_data->flags >> 8) & 0x0F;
    entry->base24_31    = (seg_data->base >> 24) & 0xFF;
}

void init_gdt(void)
{
    gdt_entry_t* gdt = (gdt_entry_t*)GDTBASE;

    const gdt_segment_data_t null_seg = {0, 0, 0};
    // Non-flat memory layout with 6MB total image
    // Kernel code: 0x00100000 - 0x002FFFFF (2MB starting at 1MB)
    const gdt_segment_data_t code_seg_pl0 = {0x00100000, 0x000001FF, GDT_CODE_PL0};
    // Kernel data: 0x00300000 - 0x004FFFFF (2MB starting at 3MB)
    const gdt_segment_data_t data_seg_pl0 = {0x00300000, 0x000001FF, GDT_DATA_PL0};
    // User code: 0x00500000 - 0x005FFFFF (1MB starting at 5MB)
    const gdt_segment_data_t code_seg_pl3 = {0x00500000, 0x000000FF, GDT_CODE_PL3};
    // User data: 0x00600000 - 0x006FFFFF (1MB starting at 6MB)
    const gdt_segment_data_t data_seg_pl3 = {0x00600000, 0x000000FF, GDT_DATA_PL3};
    // Kernel BSS: 0x00700000 - 0x007FFFFF (1MB starting at 7MB)
    const gdt_segment_data_t bss_seg_pl0 = {0x00700000, 0x000000FF, GDT_BSS_PL0};
    // User BSS: 0x00800000 - 0x008FFFFF (1MB starting at 8MB)
    const gdt_segment_data_t bss_seg_pl3 = {0x00800000, 0x000000FF, GDT_BSS_PL3};
    
    gdt_entry_create(&gdt[0], &null_seg);
    gdt_entry_create(&gdt[1], &code_seg_pl0);
    gdt_entry_create(&gdt[2], &data_seg_pl0);
    gdt_entry_create(&gdt[3], &code_seg_pl3);
    gdt_entry_create(&gdt[4], &data_seg_pl3);
    gdt_entry_create(&gdt[5], &bss_seg_pl0);
    gdt_entry_create(&gdt[6], &bss_seg_pl3);

    gdt_reg_t gdt_reg;
    gdt_reg.limit = (sizeof(gdt_entry_t) * 7) - 1;
    gdt_reg.base  = GDTBASE;

    asm volatile ("lgdt %0" : : "m"(gdt_reg));
}
