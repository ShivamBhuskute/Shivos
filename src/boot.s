.set ALIGN, 1 << 0 				/*align loaded moduoes on page boundries */
.set MEMINFO, 1 << 1 			/* rovides memory map */
.set FLAGS, ALIGN | MEMINFO 	/*MULTIBOOT FLAG FIELD */
.set MAGIC, 0X1BADB002 			/*AGIC NUMBER LETS bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above to prove we are multiboot */


/* declares a multiboot header and marks our prigram as kernel
the boot loader will search for these values in first 8 kiB of file which is aligned
at a 32-bit boundry */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM


/*
The multiboot standard does not define the value of the stack pointer register
(esp) and it is up to the kernel to provide a stack. This allocates room for a
small stack by creating a symbol at the bottom of it, then allocating 16384
bytes for it, and finally creating a symbol at the top. The stack grows
downwards on x86. The stack is in its own section so it can be marked nobits,
which means the kernel file is smaller because it does not contain an
uninitialized stack. The stack on x86 must be 16-byte aligned according to the
System V ABI standard and de-facto extensions. The compiler will assume the
stack is properly aligned and failure to align the stack will result in
undefined behavior.
*/
.section .bss
.align 16

stack_bottom:
.skip 16384 /* 16 KB */
stack_top:


/* start as the entry point */
.section .text
.global _start
.type _start, @function
_start:
	/* after the bootloader has loaded into 32-bit protected mode, majority of functions are disabled. the kernel has the absoulte power over the machine */

	mov $stack_top, %esp   /* to  setup stack and point esp to the top of the stack (as it grows downwards)*/

	call kernel_main

	cli
1:	hlt
	jmp 1b


.size _start, . - _start
