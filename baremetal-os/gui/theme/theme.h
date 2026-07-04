/* =============================================================================
 * NanoOS GUI Theme Header
 * =============================================================================
 */

#ifndef THEME_H
#define THEME_H

#include <stdint.h>

// Theme structure
typedef struct {
    uint32_t window_bg;
    uint32_t window_border;
    uint32_t window_border_focused;
    uint32_t title_bar;
    uint32_t title_bar_inactive;
    uint32_t title_text;
    uint32_t button_bg;
    uint32_t button_hover;
    uint32_t button_pressed;
    uint32_t button_text;
    uint32_t text_normal;
    uint32_t text_disabled;
    uint32_t selection_bg;
    uint32_t selection_text;
    uint32_t scrollbar_bg;
    uint32_t scrollbar_thumb;
    uint32_t desktop_bg;
    uint32_t taskbar_bg;
    uint32_t menu_bg;
} theme_t;

// Predefined themes
extern theme_t theme_default;
extern theme_t theme_dark;
extern theme_t theme_light;

// Theme functions
void theme_init(void);
void theme_set(theme_t* theme);
theme_t* theme_get_current(void);

#endif