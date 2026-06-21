#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
// kfs grip
#include "drivers/vga/vga.c"

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

	/* Newline support is left as an exercise. */
	terminal_writestring("Hello, kernel World!\n");
	enable_cursor(0, 14);
	terminal_writestring("This is the second line");
	delay(1000000000); // apparently this is 2 seconds
	clear_screen();
	update_cursor(0, 0);
}
