/* =============================================================================
 * NanoOS Window Manager Header
 * =============================================================================
 */

#ifndef WM_H
#define WM_H

#include "../gui.h"

// Window manager states
typedef enum {
    WM_NORMAL,
    WM_RESIZING,
    WM_MOVING,
    WM_DRAGGING
} wm_state_t;

// Window manager
typedef struct {
    wm_state_t state;
    window_t* active_window;
    int desktop_bg;
    int start_x, start_y;
    int prev_x, prev_y;
} wm_t;

void wm_init(void);
void wm_handle_event(event_t* event);
void wm_draw_desktop(void);
void wm_add_window(window_t* win);
void wm_remove_window(window_t* win);
void wm_raise_window(window_t* win);
void wm_lower_window(window_t* win);

window_t* wm_get_window_at(int x, int y);
int wm_get_resize_handle(window_t* win, int x, int y);

#endif