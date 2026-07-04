/* =============================================================================
 * NanoOS GUI Calculator Application
 * =============================================================================
 */

#include "../gui.h"

#define CALC_DISPLAY_W 200
#define CALC_DISPLAY_H 40
#define CALC_BTN_SIZE 40

typedef struct {
    char display[64];
    double current_value;
    double previous_value;
    char operator;
    int new_number;
    int has_result;
} calculator_t;

static calculator_t* calc = 0;

void calculator_draw(window_t* win) {
    if (!calc) return;
    
    int content_x = win->x + BORDER_WIDTH + 2;
    int content_y = win->y + BORDER_WIDTH + TITLE_BAR_HEIGHT + 4;
    
    // Draw display
    int display_x = content_x;
    int display_y = content_y;
    draw_rect(display_x, display_y, CALC_DISPLAY_W, CALC_DISPLAY_H, COLOR_WHITE);
    draw_rect(display_x, display_y, CALC_DISPLAY_W, 2, COLOR_DARK_GRAY);
    draw_rect(display_x, display_y, 2, CALC_DISPLAY_H, COLOR_DARK_GRAY);
    draw_rect(display_x + CALC_DISPLAY_W - 2, display_y, 2, CALC_DISPLAY_H, COLOR_DARK_GRAY);
    draw_rect(display_x, display_y + CALC_DISPLAY_H - 2, CALC_DISPLAY_W, 2, COLOR_DARK_GRAY);
    
    // Draw display text (right-aligned)
    int text_x = display_x + CALC_DISPLAY_W - 8;
    int len = strlen(calc->display);
    text_x -= len * 10;
    draw_text(text_x, display_y + 12, calc->display, COLOR_BLACK);
    
    // Draw buttons
    const char* buttons[] = {
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "0", ".", "=", "+",
        "C", "(", ")", "^"
    };
    
    int btn_x = content_x;
    int btn_y = display_y + CALC_DISPLAY_H + 10;
    
    for (int i = 0; i < 20; i++) {
        uint32_t bg_color;
        const char* btn = buttons[i];
        
        // Style operator buttons differently
        if (strcmp(btn, "+") == 0 || strcmp(btn, "-") == 0 ||
            strcmp(btn, "*") == 0 || strcmp(btn, "/") == 0 ||
            strcmp(btn, "=") == 0 || strcmp(btn, "^") == 0) {
            bg_color = COLOR_TITLE_BAR;
        } else if (strcmp(btn, "C") == 0) {
            bg_color = 0xCC0000;  // Red for clear
        } else {
            bg_color = COLOR_LIGHT_GRAY;
        }
        
        // Draw button
        draw_button(btn_x, btn_y, CALC_BTN_SIZE, CALC_BTN_SIZE, btn, bg_color, 
                   (bg_color == COLOR_LIGHT_GRAY) ? COLOR_BLACK : COLOR_WHITE);
        
        // Move to next position
        btn_x += CALC_BTN_SIZE + 4;
        if ((i + 1) % 4 == 0) {
            btn_x = content_x;
            btn_y += CALC_BTN_SIZE + 4;
        }
    }
}

void calculator_handle_event(window_t* win, event_t* event) {
    if (!calc || event->type != EVENT_KEY_PRESS) return;
    
    int key = event->key;
    
    // Number keys
    if (key >= '0' && key <= '9') {
        if (calc->new_number || calc->has_result) {
            calc->display[0] = '\0';
            calc->new_number = 0;
            calc->has_result = 0;
        }
        int len = strlen(calc->display);
        if (len < 31) {
            calc->display[len] = (char)key;
            calc->display[len + 1] = '\0';
        }
    }
    
    // Decimal point
    else if (key == '.') {
        if (calc->new_number) {
            calc->display[0] = '0';
            calc->display[1] = '.';
            calc->display[2] = '\0';
            calc->new_number = 0;
        } else if (strchr(calc->display, '.') == 0) {
            int len = strlen(calc->display);
            if (len < 31) {
                calc->display[len] = '.';
                calc->display[len + 1] = '\0';
            }
        }
    }
    
    // Operators
    else if (key == '+' || key == '-' || key == '*' || key == '/' || key == '^') {
        calc->previous_value = atof(calc->display);
        calc->operator = (char)key;
        calc->new_number = 1;
    }
    
    // Equals
    else if (key == '=' || key == 0x0D) {
        double result = 0;
        double current = atof(calc->display);
        
        switch (calc->operator) {
            case '+': result = calc->previous_value + current; break;
            case '-': result = calc->previous_value - current; break;
            case '*': result = calc->previous_value * current; break;
            case '/': 
                if (current != 0) result = calc->previous_value / current;
                else strcpy(calc->display, "Error");
                break;
            case '^': result = pow(calc->previous_value, current); break;
        }
        
        // Convert result to string
        if (calc->operator != '/') {
            double_to_string(result, calc->display);
        }
        calc->has_result = 1;
        calc->new_number = 1;
    }
    
    // Clear
    else if (key == 'c' || key == 'C') {
        calc->display[0] = '0';
        calc->display[1] = '\0';
        calc->current_value = 0;
        calc->previous_value = 0;
        calc->operator = '+';
        calc->new_number = 1;
        calc->has_result = 0;
    }
}

double atof(const char* str) {
    double result = 0;
    double factor = 1;
    int in_decimal = 0;
    
    // Handle sign
    if (*str == '-') {
        factor = -1;
        str++;
    }
    
    while (*str) {
        if (*str == '.') {
            in_decimal = 1;
            str++;
            continue;
        }
        
        if (*str >= '0' && *str <= '9') {
            if (in_decimal) {
                factor /= 10;
                result += (*str - '0') * factor;
            } else {
                result = result * 10 + (*str - '0');
            }
        }
        str++;
    }
    
    return result;
}

void double_to_string(double num, char* str) {
    if (num < 0) {
        *str++ = '-';
        num = -num;
    }
    
    int int_part = (int)num;
    double frac_part = num - int_part;
    
    // Integer part
    char int_str[32];
    int i = 0;
    if (int_part == 0) {
        int_str[i++] = '0';
    } else {
        while (int_part > 0) {
            int_str[i++] = '0' + (int_part % 10);
            int_part /= 10;
        }
    }
    
    // Reverse and copy
    for (int j = i - 1; j >= 0; j--) {
        *str++ = int_str[j];
    }
    
    // Decimal part
    *str++ = '.';
    
    for (int j = 0; j < 6; j++) {
        frac_part *= 10;
        int digit = (int)frac_part;
        *str++ = '0' + digit;
        frac_part -= digit;
        if (frac_part < 0.0001) break;
    }
    
    *str = '\0';
}

double pow(double base, double exp) {
    double result = 1;
    if (exp >= 0) {
        for (int i = 0; i < (int)exp; i++) {
            result *= base;
        }
    }
    return result;
}

char* strchr(const char* str, char c) {
    while (*str) {
        if (*str == c) return (char*)str;
        str++;
    }
    return 0;
}

void calculator_open(window_t* win) {
    if (!calc) {
        calc = (calculator_t*)kmalloc(sizeof(calculator_t));
        calc->display[0] = '0';
        calc->display[1] = '\0';
        calc->current_value = 0;
        calc->previous_value = 0;
        calc->operator = '+';
        calc->new_number = 1;
        calc->has_result = 0;
    }
    
    win->draw = calculator_draw;
    win->handle_event = calculator_handle_event;
    win->user_data = calc;
}