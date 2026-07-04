/* =============================================================================
 * NanoOS System Calls Header
 * =============================================================================
 */

#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

// System call numbers
#define SYS_WRITE      0
#define SYS_READ       1
#define SYS_MALLOC     2
#define SYS_FREE       3
#define SYS_EXIT       4
#define SYS_GETPID     5
#define SYS_YIELD      6
#define SYS_SLEEP      7
#define SYS_OPEN       8
#define SYS_CLOSE      9
#define SYS_READFILE   10
#define SYS_WRITEFILE  11
#define SYS_GETTIME    12

typedef struct registers {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

void syscall_init(void);
void syscall_handler(registers_t* regs);
uint32_t syscall(uint32_t num, uint32_t arg1, uint32_t arg2, uint32_t arg3);

#endif