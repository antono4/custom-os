/* =============================================================================
 * NanoOS Simple Window Manager
 * =============================================================================
 */

#include "wm.h"
#include "../gui.h"

// Window manager instance
static wm_t wm = {0};

void wm_init(void) {
    wm.state = WM_NORMAL;
    wm.active_window = 0;
    wm.desktop_bg = COLOR_LIGHT_GRAY;
}

void wm_handle_event(event_t* event) {
    switch (event->type) {
        case EVENT_MOUSE_MOVE:
            wm_handle_mouse_move(event);
            break;
        case EVENT_MOUSE_DOWN:
            wm_handle_mouse_down(event);
            break;
        case EVENT_MOUSE_UP:
            wm_handle_mouse_up(event);
            break;
        case EVENT_KEY_PRESS:
            wm_handle_key_press(event);
            break;
        default:
            break;
    }
}

void wm_handle_mouse_down(event_t* event) {
    window_t* clicked = wm_get_window_at(event->x, event->y);
    
    if (clicked) {
        wm_raise_window(clicked);
        window_set_focus(clicked);
        
        // Check if clicking on title bar (move)
        if (event->y >= clicked->y && 
            event->y < clicked->y + TITLE_BAR_HEIGHT + BORDER_WIDTH) {
            wm.state = WM_MOVING;
            wm.start_x = event->x - clicked->x;
            wm.start_y = event->y - clicked->y;
        }
        
        // Check if clicking on resize handle
        int resize = wm_get_resize_handle(clicked, event->x, event->y);
        if (resize) {
            wm.state = WM_RESIZING;
            wm.prev_x = event->x;
            wm.prev_y = event->y;
        }
        
        // Check if clicking close button
        int btn_x = clicked->x + clicked->width - BORDER_WIDTH - 18;
        int btn_y = clicked->y + BORDER_WIDTH + 3;
        if (event->x >= btn_x && event->x < btn_x + 16 &&
            event->y >= btn_y && event->y < btn_y + 16) {
            window_destroy(clicked);
        }
    } else {
        // Clicked on desktop - deselect
        window_set_focus(0);
    }
}

void wm_handle_mouse_up(event_t* event) {
    wm.state = WM_NORMAL;
}

void wm_handle_mouse_move(event_t* event) {
    if (wm.state == WM_MOVING && wm.active_window) {
        int new_x = event->x - wm.start_x;
        int new_y = event->y - wm.start_y;
        window_move(wm.active_window, new_x, new_y);
    }
    
    if (wm.state == WM_RESIZING && wm.active_window) {
        int delta_x = event->x - wm.prev_x;
        int delta_y = event->y - wm.prev_y;
        window_resize(wm.active_window, 
                     wm.active_window->width + delta_x,
                     wm.active_window->height + delta_y);
        wm.prev_x = event->x;
        wm.prev_y = event->y;
    }
}

void wm_handle_key_press(event_t* event) {
    // Handle keyboard shortcuts
    switch (event->key) {
        case 0x1B: // ESC - close focused window
            if (wm.active_window) {
                window_destroy(wm.active_window);
            }
            break;
        default:
            break;
    }
}

void wm_draw_desktop(void) {
    // Draw desktop background
    draw_rect(0, 0, GUI_WIDTH, GUI_HEIGHT, wm.desktop_bg);
    
    // Draw taskbar at bottom
    draw_rect(0, GUI_HEIGHT - 32, GUI_WIDTH, 32, COLOR_DARK_GRAY);
    
    // Draw start button
    draw_button(4, GUI_HEIGHT - 28, 80, 24, "Start", COLOR_TITLE_BAR, COLOR_WHITE);
    
    // Draw clock area
    draw_text(GUI_WIDTH - 100, GUI_HEIGHT - 24, "00:00", COLOR_WHITE);
}

void wm_add_window(window_t* win) {
    // Windows are added when created via gui.c
}

void wm_remove_window(window_t* win) {
    if (wm.active_window == win) {
        wm.active_window = 0;
    }
}

void wm_raise_window(window_t* win) {
    wm.active_window = win;
}

void wm_lower_window(window_t* win) {
    if (wm.active_window == win) {
        wm.active_window = 0;
    }
}

window_t* wm_get_window_at(int x, int y) {
    // Find topmost window at position
    // Search in reverse order (top to bottom)
    for (int i = 0; i < window_count; i++) {
        window_t* win = windows[window_count - 1 - i];
        if (!win || !win->visible) continue;
        
        if (x >= win->x && x < win->x + win->width &&
            y >= win->y && y < win->y + win->height) {
            return win;
        }
    }
    return 0;
}

int wm_get_resize_handle(window_t* win, int x, int y) {
    // Check bottom-right corner for resize handle
    if (x >= win->x + win->width - 10 &&
        y >= win->y + win->height - 10 &&
        x < win->x + win->width &&
        y < win->y + win->height) {
        return 1;
    }
    return 0;
}