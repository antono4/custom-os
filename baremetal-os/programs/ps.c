/* =============================================================================
 * NanoOS Process List Program (ps)
 * =============================================================================
 */

#include "kernel.h"

extern int strcmp(const char*, const char*);

void main(void) {
    print_string("=================================\n");
    print_string("     Process List (ps)           \n");
    print_string("=================================\n");
    print_string("\n");
    print_string("  PID   NAME          STATE\n");
    print_string("------  ------------  --------\n");
    print_string("  0     idle          running\n");
    print_string("  1     init          ready\n");
    print_string("  2     shell         ready\n");
    print_string("  3     timer         blocked\n");
    print_string("\n");
}