/* =============================================================================
 * NanoOS Kernel - Main Entry Point
 * =============================================================================
 */

#include "kernel.h"
#include "gdt.h"
#include "idt.h"
#include "memory.h"
#include "vfs.h"

extern void terminal_init(void);
extern void print_string(const char* str);
extern void print_int(int num);

void kernel_main(void) {
    /* Initialize subsystems */
    gdt_init();
    idt_init();
    memory_init();
    vfs_init();
    terminal_init();
    
    /* Print boot message */
    print_string("NanoOS Kernel v0.1\n");
    print_string("Initializing...\n");
    print_string("GDT: OK\n");
    print_string("IDT: OK\n");
    print_string("Memory: OK\n");
    print_string("VFS: OK\n");
    print_string("\nSystem ready!\n");
    
    /* Halt */
    while (1) {
        __asm__ volatile ("hlt");
    }
}
