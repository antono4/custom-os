/* =============================================================================
 * NanoOS Standard C Library - stdio
 * =============================================================================
 */

#include "kernel.h"

#define VGA_BASE 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static size_t terminal_row = 0;
static size_t terminal_col = 0;
static uint8_t terminal_color = 0x07;

static inline uint16_t make_vga_char(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

void terminal_init(void) {
    terminal_row = 0;
    terminal_col = 0;
    terminal_color = 0x07;
    uint16_t* vga = (uint16_t*)VGA_BASE;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga[i] = make_vga_char(' ', terminal_color);
    }
}

void terminal_putchar(char c) {
    uint16_t* vga = (uint16_t*)VGA_BASE;
    
    if (c == '\n') {
        terminal_col = 0;
        terminal_row++;
        if (terminal_row >= VGA_HEIGHT) terminal_row = VGA_HEIGHT - 1;
        return;
    }
    
    if (c == '\r') {
        terminal_col = 0;
        return;
    }
    
    size_t index = terminal_row * VGA_WIDTH + terminal_col;
    vga[index] = make_vga_char(c, terminal_color);
    
    terminal_col++;
    if (terminal_col >= VGA_WIDTH) {
        terminal_col = 0;
        terminal_row++;
        if (terminal_row >= VGA_HEIGHT) terminal_row = VGA_HEIGHT - 1;
    }
}

void print_string(const char* str) {
    while (*str) {
        terminal_putchar(*str++);
    }
}

void print_int(int num) {
    if (num < 0) {
        terminal_putchar('-');
        num = -num;
    }
    char buffer[32];
    int i = 0;
    if (num == 0) {
        terminal_putchar('0');
        return;
    }
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    while (i > 0) terminal_putchar(buffer[--i]);
}

void print_hex(uint32_t num) {
    terminal_putchar('0');
    terminal_putchar('x');
    char buffer[16];
    int i = 0;
    if (num == 0) {
        terminal_putchar('0');
        return;
    }
    while (num > 0) {
        int digit = num % 16;
        buffer[i++] = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
        num /= 16;
    }
    while (i > 0) terminal_putchar(buffer[--i]);
}
