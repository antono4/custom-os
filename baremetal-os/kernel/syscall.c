/* =============================================================================
 * NanoOS System Calls Implementation
 * =============================================================================
 */

#include "syscall.h"
#include "kernel.h"
#include "memory.h"
#include "task.h"

extern void print_string(const char* str);
extern void terminal_putchar(char c);

static uint32_t syscall_write(const char* str) {
    print_string(str);
    return 0;
}

static uint32_t syscall_malloc(uint32_t size) {
    return (uint32_t)kmalloc(size);
}

static uint32_t syscall_free(uint32_t addr) {
    kfree((void*)addr);
    return 0;
}

static uint32_t syscall_getpid(void) {
    task_t* t = task_get_current();
    return t ? t->id : 0;
}

static uint32_t syscall_yield(void) {
    task_yield();
    return 0;
}

static uint32_t syscall_sleep(uint32_t ticks) {
    // Simple sleep implementation
    for (volatile uint32_t i = 0; i < ticks * 100000; i++);
    return 0;
}

static uint32_t syscall_gettime(void) {
    return 0;  // Placeholder
}

void syscall_init(void) {
    // Register interrupt handler for syscalls (interrupt 0x80)
}

void syscall_handler(registers_t* regs) {
    uint32_t syscall_num = regs->eax;
    uint32_t arg1 = regs->ebx;
    uint32_t arg2 = regs->ecx;
    uint32_t arg3 = regs->edx;
    uint32_t result = 0;
    
    switch (syscall_num) {
        case SYS_WRITE:
            result = syscall_write((const char*)arg1);
            break;
        case SYS_MALLOC:
            result = syscall_malloc(arg1);
            break;
        case SYS_FREE:
            result = syscall_free(arg1);
            break;
        case SYS_GETPID:
            result = syscall_getpid();
            break;
        case SYS_YIELD:
            result = syscall_yield();
            break;
        case SYS_SLEEP:
            result = syscall_sleep(arg1);
            break;
        case SYS_GETTIME:
            result = syscall_gettime();
            break;
        default:
            result = (uint32_t)-1;
    }
    
    regs->eax = result;
}

uint32_t syscall(uint32_t num, uint32_t arg1, uint32_t arg2, uint32_t arg3) {
    uint32_t result;
    __asm__ volatile (
        "mov %1, %%eax\n\t"
        "mov %2, %%ebx\n\t"
        "mov %3, %%ecx\n\t"
        "mov %4, %%edx\n\t"
        "int $0x80\n\t"
        "mov %%eax, %0"
        : "=r"(result)
        : "r"(num), "r"(arg1), "r"(arg2), "r"(arg3)
    );
    return result;
}