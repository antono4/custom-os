/* =============================================================================
 * NanoOS Task Management Implementation
 * =============================================================================
 */

#include "task.h"
#include "kernel.h"

static task_t tasks[MAX_TASKS];
static int task_count = 0;
static int current_task = 0;
static uint32_t task_ticks = 0;

extern void context_switch(uint32_t* old_esp, uint32_t new_esp);

void task_init(void) {
    task_count = 0;
    current_task = 0;
    task_ticks = 0;
    
    // Create idle task
    task_create("idle", 0, 0);
}

int task_create(const char* name, void (*entry)(void), uint32_t priority) {
    if (task_count >= MAX_TASKS) return -1;
    
    task_t* t = &tasks[task_count];
    t->id = task_count;
    t->priority = priority;
    t->state = 1;  // Ready
    t->time_slice = 10;
    t->ticks_used = 0;
    
    // Copy name safely
    int i = 0;
    while (name[i] && i < 31) {
        t->name[i] = name[i];
        i++;
    }
    t->name[i] = '\0';
    
    task_count++;
    return t->id;
}

task_t* task_get_current(void) {
    return &tasks[current_task];
}

void task_yield(void) {
    task_schedule();
}

void task_schedule(void) {
    // Simple round-robin scheduler
    int old_task = current_task;
    
    do {
        current_task = (current_task + 1) % task_count;
        if (tasks[current_task].state == 1) break;
    } while (current_task != old_task);
    
    if (current_task != old_task) {
        task_ticks = 0;
    }
}

void task_exit(void) {
    tasks[current_task].state = 3;  // Exited
    task_schedule();
}

void task_switch(void) {
    task_ticks++;
    
    if (task_ticks >= tasks[current_task].time_slice) {
        task_schedule();
    }
}