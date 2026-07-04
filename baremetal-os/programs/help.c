/* =============================================================================
 * NanoOS Help Program
 * =============================================================================
 */

#include "kernel.h"

void main(void) {
    print_string("=================================\n");
    print_string("     NanoOS Help                 \n");
    print_string("=================================\n");
    print_string("\n");
    print_string("Available Commands:\n");
    print_string("-------------------\n");
    print_string("  help      - Show this help\n");
    print_string("  clear     - Clear screen\n");
    print_string("  info      - System information\n");
    print_string("  memory    - Memory status\n");
    print_string("  ps        - Process list\n");
    print_string("  date      - Show date/time\n");
    print_string("  hello     - Run hello program\n");
    print_string("  calc      - Run calculator\n");
    print_string("  about     - About NanoOS\n");
    print_string("\n");
}