#ifndef GDT_H
#define GDT_H

#include <stdint.h>
#include "../../drivers/vga/vga.h"

// struct for gdt
struct GDT {
    uint32_t base;
    uint32_t limit;
    uint8_t access_byte;
    uint8_t flags;
};

void gdt_init(void);
void encodeGdtEntry(uint8_t *target, struct GDT source);

// assembly func
extern void setGdt(uint16_t limit, uint32_t base);
extern void reloadSegments(void);

#endif
