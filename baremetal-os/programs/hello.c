/* =============================================================================
 * NanoOS Hello World Program
 * =============================================================================
 */

#include "kernel.h"

void main(void) {
    print_string("=================================\n");
    print_string("     Hello from NanoOS!          \n");
    print_string("=================================\n");
    print_string("\nThis is a user program running\n");
    print_string("on our custom operating system.\n");
    print_string("\n");
}