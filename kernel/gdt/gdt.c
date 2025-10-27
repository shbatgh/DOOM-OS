#include <stdint.h>
#include "gdt.h"

// Structure for GDT entry
struct GDT {
    uint32_t base;
    uint32_t limit;
    uint8_t access_byte;
    uint8_t flags;
};

void encodeGdtEntry(uint8_t *target, struct GDT source) {
    // Check the limit to make sure that it can be encoded
    if (source.limit > 0xFFFFF) {
        kerror("GDT cannot encode limits larger than 0xFFFFF");
    }

    // Encode the limit
    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] = (source.limit >> 16) & 0x0F;

    // Encode the base
    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;

    // Encode the access byte
    target[5] = source.access_byte;

    // Encode the flags
    target[6] |= (source.flags << 4);
}

void gdt_install() {
    struct GDT gdt_entries[3];
    uint8_t gdt_table[24];

    // Null descriptor
    gdt_entries[0] = (struct GDT){0, 0, 0, 0};

    // Code segment descriptor
    gdt_entries[1] = (struct GDT){0, 0x000FFFFF, GDT_CODE_PL0 & 0xFF, (GDT_CODE_PL0 >> 8) & 0xF};

    // Data segment descriptor
    gdt_entries[2] = (struct GDT){0, 0x000FFFFF, GDT_DATA_PL0 & 0xFF, (GDT_DATA_PL0 >> 8) & 0xF};

    for (int i = 0; i < 3; i++) {
        encodeGdtEntry(&gdt_table[i * 8], gdt_entries[i]);
    }

    // Load the GDT using assembly
    extern void setGdt(uint16_t limit, uint32_t base);
    setGdt(sizeof(gdt_table) - 1, (uint32_t)gdt_table);
}
