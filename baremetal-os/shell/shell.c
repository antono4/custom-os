/* =============================================================================
 * NanoOS Shell
 * =============================================================================
 */

#include "kernel.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

extern int strcmp(const char*, const char*);

static size_t shell_row = 0;
static size_t shell_col = 0;

static inline void putchar(char c) {
    volatile uint16_t* vga = (uint16_t*)0xB8000;
    if (c == '\n') {
        shell_col = 0;
        shell_row++;
        if (shell_row >= VGA_HEIGHT) shell_row = 0;
        return;
    }
    size_t idx = shell_row * VGA_WIDTH + shell_col;
    vga[idx] = (0x07 << 8) | (uint8_t)c;
    shell_col++;
    if (shell_col >= VGA_WIDTH) {
        shell_col = 0;
        shell_row++;
        if (shell_row >= VGA_HEIGHT) shell_row = 0;
    }
}

static inline void print(const char* s) {
    while (*s) putchar(*s++);
}

extern uint32_t get_total_memory(void);

void shell_init(void) {
    shell_row = 0;
    shell_col = 0;
    print("\nNanoOS Shell v0.1\n");
    print("Type 'help' for commands.\n\n");
}

void shell_handle_command(const char* cmd) {
    if (cmd[0] == '\0') return;
    
    if (strcmp(cmd, "help") == 0) {
        print("Commands: help, clear, info, memory, about\n");
    }
    else if (strcmp(cmd, "clear") == 0) {
        volatile uint16_t* vga = (uint16_t*)0xB8000;
        for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) vga[i] = 0x0720;
        shell_row = shell_col = 0;
    }
    else if (strcmp(cmd, "info") == 0) {
        print("NanoOS v0.1 - i386\n");
    }
    else if (strcmp(cmd, "memory") == 0) {
        print("Memory: 32 KB (simulated)\n");
    }
    else if (strcmp(cmd, "about") == 0) {
        print("NanoOS - Built from scratch\n");
    }
    else {
        print("Unknown: ");
        print(cmd);
        print("\n");
    }
}
