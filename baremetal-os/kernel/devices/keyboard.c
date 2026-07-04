/* =============================================================================
 * Keyboard Driver
 * =============================================================================
 */

#include "kernel.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

void keyboard_init(void) {
    /* Initialize keyboard */
}

uint8_t keyboard_read(void) {
    return inb(KEYBOARD_DATA_PORT);
}

int keyboard_available(void) {
    return (inb(KEYBOARD_STATUS_PORT) & 1);
}
