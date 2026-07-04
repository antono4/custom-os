/* =============================================================================
 * NanoOS GUI System Implementation
 * =============================================================================
 */

#include "gui.h"
#include "../kernel/kernel.h"

static window_t* windows[MAX_WINDOWS];
static int window_count = 0;
static window_t* focused_window = 0;
static int mouse_x = 0;
static int mouse_y = 0;
static int mouse_buttons = 0;

// Framebuffer for GUI rendering
static uint32_t framebuffer[GUI_WIDTH * GUI_HEIGHT];

void gui_init(void) {
    // Clear framebuffer
    for (int i = 0; i < GUI_WIDTH * GUI_HEIGHT; i++) {
        framebuffer[i] = COLOR_WHITE;
    }
    
    // Initialize windows
    for (int i = 0; i < MAX_WINDOWS; i++) {
        windows[i] = 0;
    }
    window_count = 0;
    
    // Draw desktop background
    draw_rect(0, 0, GUI_WIDTH, GUI_HEIGHT, COLOR_LIGHT_GRAY);
}

void gui_draw_framebuffer(void) {
    // Copy framebuffer to video memory
    volatile uint32_t* vga = (uint32_t*)0xA0000;
    for (int i = 0; i < GUI_WIDTH * GUI_HEIGHT && i < 320 * 200; i++) {
        vga[i] = framebuffer[i];
    }
}

window_t* window_create(const char* title, int x, int y, int w, int h) {
    if (window_count >= MAX_WINDOWS) return 0;
    
    window_t* win = (window_t*)kmalloc(sizeof(window_t));
    if (!win) return 0;
    
    win->id = window_count;
    
    // Copy title safely
    int i = 0;
    while (title[i] && i < 63) {
        win->title[i] = title[i];
        i++;
    }
    win->title[i] = '\0';
    
    win->x = x;
    win->y = y;
    win->width = w;
    win->height = h;
    win->bg_color = COLOR_WHITE;
    win->type = WINDOW_NORMAL;
    win->focused = 0;
    win->visible = 1;
    win->draw = 0;
    win->handle_event = 0;
    win->user_data = 0;
    
    windows[window_count++] = win;
    return win;
}

void window_destroy(window_t* win) {
    if (!win) return;
    
    for (int i = 0; i < window_count; i++) {
        if (windows[i] == win) {
            for (int j = i; j < window_count - 1; j++) {
                windows[j] = windows[j + 1];
            }
            window_count--;
            kfree(win);
            break;
        }
    }
}

void window_draw(window_t* win) {
    if (!win || !win->visible) return;
    
    uint32_t border_color = (win->focused) ? COLOR_BLUE : COLOR_GRAY;
    
    // Draw window background
    draw_rect(win->x, win->y, win->width, win->height, win->bg_color);
    
    // Draw border
    draw_rect(win->x, win->y, win->width, BORDER_WIDTH, border_color);
    draw_rect(win->x, win->y + win->height - BORDER_WIDTH, win->width, BORDER_WIDTH, border_color);
    draw_rect(win->x, win->y, BORDER_WIDTH, win->height, border_color);
    draw_rect(win->x + win->width - BORDER_WIDTH, win->y, BORDER_WIDTH, win->height, border_color);
    
    // Draw title bar
    draw_rect(win->x + BORDER_WIDTH, win->y + BORDER_WIDTH, 
              win->width - 2 * BORDER_WIDTH, TITLE_BAR_HEIGHT, COLOR_TITLE_BAR);
    
    // Draw title text
    draw_text(win->x + BORDER_WIDTH + 4, win->y + BORDER_WIDTH + 4, 
              win->title, COLOR_TITLE_TEXT);
    
    // Draw close button (X)
    int btn_x = win->x + win->width - BORDER_WIDTH - 18;
    int btn_y = win->y + BORDER_WIDTH + 3;
    draw_rect(btn_x, btn_y, 16, 16, COLOR_RED);
    draw_text(btn_x + 4, btn_y + 3, "X", COLOR_WHITE);
    
    // Draw resize handle
    draw_rect(win->x + win->width - 10, win->y + win->height - 10, 
              10, 10, COLOR_DARK_GRAY);
    
    // Call custom draw function if set
    if (win->draw) {
        win->draw(win);
    }
}

void window_set_focus(window_t* win) {
    // Remove focus from all windows
    for (int i = 0; i < window_count; i++) {
        windows[i]->focused = 0;
    }
    
    if (win) {
        win->focused = 1;
        focused_window = win;
    }
}

void window_move(window_t* win, int x, int y) {
    if (!win) return;
    win->x = x;
    win->y = y;
}

void window_resize(window_t* win, int w, int h) {
    if (!win) return;
    if (w < 100) w = 100;
    if (h < 50) h = 50;
    win->width = w;
    win->height = h;
}

void window_minimize(window_t* win) {
    if (!win) return;
    win->visible = 0;
}

void window_maximize(window_t* win) {
    if (!win) return;
    win->x = 0;
    win->y = 0;
    win->width = GUI_WIDTH;
    win->height = GUI_HEIGHT;
}

void draw_pixel(int x, int y, uint32_t color) {
    if (x < 0 || x >= GUI_WIDTH || y < 0 || y >= GUI_HEIGHT) return;
    framebuffer[y * GUI_WIDTH + x] = color;
}

void draw_rect(int x, int y, int w, int h, uint32_t color) {
    for (int py = y; py < y + h && py < GUI_HEIGHT; py++) {
        for (int px = x; px < x + w && px < GUI_WIDTH; px++) {
            if (px >= 0 && py >= 0) {
                framebuffer[py * GUI_WIDTH + px] = color;
            }
        }
    }
}

void draw_line(int x1, int y1, int x2, int y2, uint32_t color) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = (dx > dy) ? dx : dy;
    
    if (steps == 0) {
        draw_pixel(x1, y1, color);
        return;
    }
    
    float x_inc = (float)dx / steps;
    float y_inc = (float)dy / steps;
    
    float x = x1;
    float y = y1;
    
    for (int i = 0; i <= steps; i++) {
        draw_pixel((int)x, (int)y, color);
        x += x_inc;
        y += y_inc;
    }
}

void draw_text(int x, int y, const char* text, uint32_t color) {
    // Simple bitmap font renderer
    extern void terminal_putchar(char c);
    
    int orig_x = x;
    while (*text) {
        if (*text == '\n') {
            x = orig_x;
            y += 16;
        } else {
            // Draw character at position
            // Using simple 8x16 bitmap font
            draw_char_8x16(x, y, *text, color);
            x += 8;
        }
        text++;
    }
}

void draw_char_8x16(int x, int y, char c, uint32_t color) {
    // Simple 8x16 font data
    static const uint8_t font[256][16] = {
        // Space
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        // More font data would be here...
    };
    
    // Default: draw as rectangle for unknown chars
    for (int row = 0; row < 16; row++) {
        uint8_t data = font[(unsigned char)c][row];
        for (int col = 0; col < 8; col++) {
            if (data & (0x80 >> col)) {
                draw_pixel(x + col, y + row, color);
            }
        }
    }
}

void draw_button(int x, int y, int w, int h, const char* label, uint32_t bg, uint32_t fg) {
    // Draw button background
    draw_rect(x, y, w, h, bg);
    
    // Draw button border (3D effect)
    draw_rect(x, y, w, 2, COLOR_WHITE);
    draw_rect(x, y, 2, h, COLOR_WHITE);
    draw_rect(x + w - 2, y, 2, h, COLOR_DARK_GRAY);
    draw_rect(x, y + h - 2, w, 2, COLOR_DARK_GRAY);
    
    // Draw label
    int text_x = x + (w - strlen(label) * 8) / 2;
    int text_y = y + (h - 16) / 2;
    draw_text(text_x, text_y, label, fg);
}

int mouse_get_x(void) {
    return mouse_x;
}

int mouse_get_y(void) {
    return mouse_y;
}

int mouse_get_buttons(void) {
    return mouse_buttons;
}

uint32_t mouse_get_event(void) {
    // Placeholder - would be implemented with interrupt handling
    return EVENT_NONE;
}