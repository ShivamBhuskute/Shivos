#ifndef VGA_H
#define VGA_H
#include "../../cpu/io.h"
#include <stddef.h>

void terminal_init();
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);

void terminal_writestring(const char *data);
void overflow();

void update_cursor(int x, int y);
void clear_screen();

void disable_cursor();

#endif
