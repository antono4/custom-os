/* =============================================================================
 * NanoOS Memory Information Program
 * =============================================================================
 */

#include "kernel.h"

extern uint32_t get_total_memory(void);

void main(void) {
    print_string("=================================\n");
    print_string("     Memory Information          \n");
    print_string("=================================\n");
    print_string("\n");
    print_string("Total Memory: ");
    print_int(32);  // Simulated value
    print_string(" MB\n");
    print_string("\n");
    print_string("Memory Regions:\n");
    print_string("  0x00000000 - 0x00100000: Reserved\n");
    print_string("  0x00100000 - 0x01000000: Kernel\n");
    print_string("  0x01000000 - 0x02000000: Heap\n");
    print_string("\n");
}