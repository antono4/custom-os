/* =============================================================================
 * NanoOS GUI System Header
 * =============================================================================
 */

#ifndef GUI_H
#define GUI_H

#include <stdint.h>

// GUI Constants
#define GUI_WIDTH  800
#define GUI_HEIGHT 600
#define MAX_WINDOWS 16
#define TITLE_BAR_HEIGHT 24
#define BORDER_WIDTH 2

// Colors
#define COLOR_BLACK       0x000000
#define COLOR_WHITE       0xFFFFFF
#define COLOR_RED         0xFF0000
#define COLOR_GREEN       0x00FF00
#define COLOR_BLUE        0x0000FF
#define COLOR_YELLOW      0xFFFF00
#define COLOR_CYAN       0x00FFFF
#define COLOR_MAGENTA    0xFF00FF
#define COLOR_GRAY       0x808080
#define COLOR_LIGHT_GRAY 0xC0C0C0
#define COLOR_DARK_GRAY  0x404040
#define COLOR_TITLE_BAR  0x2B579A
#define COLOR_TITLE_TEXT 0xFFFFFF

typedef enum {
    WINDOW_NORMAL,
    WINDOW_DIALOG,
    WINDOW_TOOLBAR,
    WINDOW_POPUP
} window_type_t;

typedef enum {
    EVENT_NONE,
    EVENT_MOUSE_MOVE,
    EVENT_MOUSE_DOWN,
    EVENT_MOUSE_UP,
    EVENT_KEY_PRESS,
    EVENT_KEY_RELEASE,
    EVENT_EXPOSE,
    EVENT_CLOSE,
    EVENT_RESIZE
} event_type_t;

typedef struct {
    event_type_t type;
    int x, y;
    int button;
    uint32_t key;
} event_t;

typedef struct window {
    uint32_t id;
    char title[64];
    int x, y;
    int width, height;
    uint32_t bg_color;
    window_type_t type;
    uint8_t focused;
    uint8_t visible;
    void (*draw)(struct window*);
    void (*handle_event)(struct window*, event_t*);
    void* user_data;
} window_t;

// GUI Functions
void gui_init(void);
void gui_draw_framebuffer(void);

window_t* window_create(const char* title, int x, int y, int w, int h);
void window_destroy(window_t* win);
void window_draw(window_t* win);
void window_set_focus(window_t* win);
void window_move(window_t* win, int x, int y);
void window_resize(window_t* win, int w, int h);
void window_minimize(window_t* win);
void window_maximize(window_t* win);

void draw_pixel(int x, int y, uint32_t color);
void draw_rect(int x, int y, int w, int h, uint32_t color);
void draw_line(int x1, int y1, int x2, int y2, uint32_t color);
void draw_text(int x, int y, const char* text, uint32_t color);
void draw_button(int x, int y, int w, int h, const char* label, uint32_t bg, uint32_t fg);

int mouse_get_x(void);
int mouse_get_y(void);
int mouse_get_buttons(void);
uint32_t mouse_get_event(void);

#endif