#ifndef MEMORY_H
#define MEMORY_H

void memory_init(void);
void* kmalloc(size_t size);
void kfree(void* ptr);
uint32_t get_total_memory(void);

#endif
