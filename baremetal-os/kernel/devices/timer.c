/* =============================================================================
 * Timer Driver (PIT - Programmable Interval Timer)
 * =============================================================================
 */

#include "kernel.h"

#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40

static uint32_t tick_count = 0;

void timer_init(uint32_t frequency) {
    uint32_t divisor = 1193180 / frequency;
    
    outb(PIT_COMMAND_PORT, 0x36);
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL0_PORT, (divisor >> 8) & 0xFF);
}

void timer_handler(void) {
    tick_count++;
}

uint32_t timer_get_ticks(void) {
    return tick_count;
}
