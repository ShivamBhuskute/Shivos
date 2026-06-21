#include "idt.h"

#define IDT_MAX_DESCRIPTORS 256

// Allocate the table itself. Aligned for better CPU performance.
__attribute__((aligned(0x10))) static idt_entry_t idt[IDT_MAX_DESCRIPTORS];
static idtr_t idtr;

// Import the table of assembly stubs we made in isr.s
extern void* isr_stub_table[];

// this is tha handler that ive called in idt.s
__attribute__((noreturn)) void exception_handler(void) {
    // for now, we just hang the machine entirely if an exception occurs
    __asm__ volatile ("cli; hlt");
    while(1); 
}

// Helper to fill out a single entry in the table
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];
    
    descriptor->isr_low    = (uint32_t)isr & 0xFFFF; // Extract bottom 16 bits
    descriptor->kernel_cs  = 0x08;                   // Kernel Code Segment offset in GDT
    descriptor->attributes = flags;                  // e.g. 0x8E for 32-bit interrupt gate
    descriptor->isr_high   = (uint32_t)isr >> 16;    // Extract top 16 bits
    descriptor->reserved   = 0;
}

// will call this in kernel file
void idt_init(void) {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    // We only mapped the first 32 (CPU exceptions) in assembly so far
    for (uint8_t vector = 0; vector < 32; vector++) {
        // 0x8E means: Present (1), DPL Ring 0 (00), 32-bit Interrupt Gate (01110)
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    }

    // Load the IDT into the CPU
    __asm__ volatile ("lidt %0" : : "m"(idtr)); 
    
    // Set the interrupt flag to start allowing hardware interrupts
	// __asm__ volatile ("sti"); 
}
