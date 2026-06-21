#!/bin/bash

# for kernel
i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

# for vga
i686-elf-gcc -c drivers/vga/vga.c -o vga.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

#for gdt
i686-elf-gcc -c interrupts/gdt/gdt.c -o gdt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-as interrupts/gdt/gdt.s -o gdt_asm.o

i686-elf-gcc -c interrupts/idt/idt.c -o idt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-as interrupts/idt/idt.s -o isr.o

i686-elf-gcc -T linker.ld -o myos -ffreestanding -O2 -nostdlib boot.o kernel.o vga.o idt.o isr.o io.o gdt.o gdt_asm.o -lgcc


qemu-system-i386 -kernel myos 
