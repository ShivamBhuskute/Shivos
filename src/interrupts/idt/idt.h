#ifndef IDT_H
#define IDT_H

#include <stdint.h>
#include <stdbool.h>

// This defines exactly one interrupt gate (one entry in the IDT)
typedef struct {
    uint16_t isr_low;    // The lower 16 bits of the assembly stub's memory address
    uint16_t kernel_cs;  // The GDT segment selector for the kernel code (usually 0x08)
    uint8_t  reserved;   // The CPU requires this to be 0
    uint8_t  attributes; // Flags indicating privilege level (Ring 0 vs 3) and gate type
    uint16_t isr_high;   // The higher 16 bits of the assembly stub's memory address
} __attribute__((packed)) idt_entry_t;

// This structure is what we pass to the CPU to tell it where our table is
typedef struct {
    uint16_t limit;      // The size of the IDT in bytes, minus 1
    uint32_t base;       // The memory address of the first element in our IDT array
} __attribute__((packed)) idtr_t;

void idt_init(void);

#endif
