/* =============================================================================
 * NanoOS Simple Calculator Program
 * =============================================================================
 */

#include "kernel.h"

// Simple string to number conversion
int atoi(const char* str) {
    int result = 0;
    int sign = 1;
    
    if (*str == '-') {
        sign = -1;
        str++;
    }
    
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    
    return result * sign;
}

void main(void) {
    print_string("=================================\n");
    print_string("     NanoOS Calculator           \n");
    print_string("=================================\n");
    print_string("\nOperations supported:\n");
    print_string("  Addition (+)\n");
    print_string("  Subtraction (-)\n");
    print_string("  Multiplication (*)\n");
    print_string("  Division (/)\n");
    print_string("\nDemo calculations:\n");
    
    // Demo: 10 + 5 = 15
    print_string("  10 + 5 = ");
    print_int(10 + 5);
    print_string("\n");
    
    // Demo: 20 - 8 = 12
    print_string("  20 - 8 = ");
    print_int(20 - 8);
    print_string("\n");
    
    // Demo: 6 * 7 = 42
    print_string("  6 * 7 = ");
    print_int(6 * 7);
    print_string("\n");
    
    // Demo: 100 / 4 = 25
    print_string("  100 / 4 = ");
    print_int(100 / 4);
    print_string("\n");
    
    print_string("\n");
}