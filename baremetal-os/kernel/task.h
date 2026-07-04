/* =============================================================================
 * NanoOS Task Management Header
 * =============================================================================
 */

#ifndef TASK_H
#define TASK_H

#include <stdint.h>

#define MAX_TASKS 16
#define KERNEL_STACK_SIZE 4096

typedef struct task {
    uint32_t id;
    char name[32];
    uint32_t esp;          // Stack pointer
    uint32_t ebp;          // Base pointer
    uint32_t eip;          // Instruction pointer
    uint32_t priority;
    uint32_t state;        // 0=running, 1=ready, 2=blocked, 3=exited
    uint32_t time_slice;
    uint32_t ticks_used;
} task_t;

void task_init(void);
int task_create(const char* name, void (*entry)(void), uint32_t priority);
void task_switch(void);
void task_exit(void);
void task_schedule(void);
task_t* task_get_current(void);
void task_yield(void);

#endif