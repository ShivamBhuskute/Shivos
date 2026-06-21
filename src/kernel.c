#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "drivers/vga/vga.h"
#include "interrupts/idt/idt.h"
#include "interrupts/gdt/gdt.h"

#if defined(__linux__)
#error "Wrong cross compiler"
#endif

// working is only possiblee in i386-elf compiler
#if !defined(__i386__)
#error "Needs to be compiled eith ix86-elf compiler"
#endif

void delay(uint32_t count) {
  for(volatile uint32_t i = 0; i < count; i ++  ) {
	__asm__("nop");
  }
}


void kernel_main(void) 
{
	/* Initialize terminal interface */
	terminal_init();
	enable_cursor(0, 14

				  );
	terminal_writestring("Hello, kernel World!\n");
	terminal_writestring("This is the second line\n");
	
	idt_init();
	gdt_init();

	terminal_writestring("GDT abd IDT initialized");
	
	/* Newline support is left as an exercise. */
	delay(1000000000); // apparently this is 2 seconds
	clear_screen();
	update_cursor(0, 0);

	terminal_writestring("Welcome!!!");
	//overflow();
}
