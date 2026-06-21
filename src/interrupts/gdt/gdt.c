#include "gdt.h"
#include "../../drivers/vga/vga.h"

// my actual GDT table (5 entries, 8 bytes each) ill include user and kernel data asweellas code segment and a null descriptor
uint8_t gdt_table[5][8];

// https://wiki.osdev.org/GDT_Tutorial#Flat_Setup
void encodeGdtEntry(uint8_t *target, struct GDT source)
{
    // Check the limit to make sure that it can be encoded
    if (source.limit > 0xFFFFF) {
	  terminal_writestring("GDT cannot encode limits larger than 0xFFFFF");
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

// calling this in kernel main
void gdt_init(void) {
    struct GDT descriptor;

    // Null Descriptor
    descriptor.base = 0; descriptor.limit = 0; 
    descriptor.access_byte = 0x00; descriptor.flags = 0x0;
    encodeGdtEntry(gdt_table[0], descriptor);

    // Kernel Code Segment
    descriptor.base = 0; descriptor.limit = 0xFFFFF; 
    descriptor.access_byte = 0x9A; descriptor.flags = 0xC;
    encodeGdtEntry(gdt_table[1], descriptor);

    // kernel Data Segment
    descriptor.base = 0; descriptor.limit = 0xFFFFF; 
    descriptor.access_byte = 0x92; descriptor.flags = 0xC;
    encodeGdtEntry(gdt_table[2], descriptor);

	// user code segment
	descriptor.base = 0; descriptor.limit = 0xFFFFF;
	descriptor.access_byte = 0xFA; descriptor.flags = 0xC;
	encodeGdtEntry(gdt_table[3], descriptor);

	// user dara segment
	descriptor.base =0 ; descriptor.limit = 0xFFFFF;
	descriptor.access_byte = 0xF2; descriptor.flags = 0xC;
	encodeGdtEntry(gdt_table[4], descriptor);


	// will implwmnt task state management later

    // 4. CALL YOUR ASSEMBLY FUNCTIONS!
    setGdt(sizeof(gdt_table) - 1, (uint32_t)&gdt_table);
    reloadSegments();
}
