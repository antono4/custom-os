/* =============================================================================
 * NanoOS Echo Program
 * =============================================================================
 */

#include "kernel.h"

extern int strcmp(const char*, const char*);
extern void terminal_putchar(char c);

void main(void) {
    print_string("Echo Program Ready.\n");
    print_string("Type some text (this is a demo).\n");
    print_string("\n");
}