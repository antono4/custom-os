/* =============================================================================
 * Interrupt Descriptor Table (IDT)
 * =============================================================================
 */

#include "kernel.h"

#define IDT_SIZE 256

typedef struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

static idt_entry_t idt[IDT_SIZE];
static idt_ptr_t idt_ptr;

extern void idt_flush(uint32_t);

void idt_set_gate(int num, uint32_t base, uint16_t selector, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector = selector;
    idt[num].flags = flags;
    idt[num].zero = 0;
}

void idt_init(void) {
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base = (uint32_t)&idt;
    
    /* Fill all entries with null */
    for (int i = 0; i < IDT_SIZE; i++) {
        idt_set_gate(i, 0, 0, 0);
    }
    
    /* Remap PIC */
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x00);
    outb(0xA1, 0x00);
    
    /* Load IDT */
    idt_flush((uint32_t)&idt_ptr);
}
