/* =============================================================================
 * NanoOS GUI Theme Implementation
 * =============================================================================
 */

#include "theme.h"

// Default theme (Windows-like)
theme_t theme_default = {
    .window_bg = 0xF0F0F0,
    .window_border = 0x808080,
    .window_border_focused = 0x000080,
    .title_bar = 0x000080,
    .title_bar_inactive = 0x808080,
    .title_text = 0xFFFFFF,
    .button_bg = 0xC0C0C0,
    .button_hover = 0xD0D0D0,
    .button_pressed = 0xA0A0A0,
    .button_text = 0x000000,
    .text_normal = 0x000000,
    .text_disabled = 0x808080,
    .selection_bg = 0x000080,
    .selection_text = 0xFFFFFF,
    .scrollbar_bg = 0xE0E0E0,
    .scrollbar_thumb = 0xA0A0A0,
    .desktop_bg = 0x008080,
    .taskbar_bg = 0xC0C0C0,
    .menu_bg = 0xF0F0F0
};

// Dark theme
theme_t theme_dark = {
    .window_bg = 0x2D2D2D,
    .window_border = 0x555555,
    .window_border_focused = 0x007ACC,
    .title_bar = 0x3C3C3C,
    .title_bar_inactive = 0x555555,
    .title_text = 0xFFFFFF,
    .button_bg = 0x3C3C3C,
    .button_hover = 0x4A4A4A,
    .button_pressed = 0x2D2D2D,
    .button_text = 0xFFFFFF,
    .text_normal = 0xFFFFFF,
    .text_disabled = 0x808080,
    .selection_bg = 0x007ACC,
    .selection_text = 0xFFFFFF,
    .scrollbar_bg = 0x3C3C3C,
    .scrollbar_thumb = 0x5A5A5A,
    .desktop_bg = 0x1E1E1E,
    .taskbar_bg = 0x3C3C3C,
    .menu_bg = 0x2D2D2D
};

// Light theme
theme_t theme_light = {
    .window_bg = 0xFFFFFF,
    .window_border = 0xCCCCCC,
    .window_border_focused = 0x2196F3,
    .title_bar = 0x2196F3,
    .title_bar_inactive = 0xCCCCCC,
    .title_text = 0xFFFFFF,
    .button_bg = 0xF5F5F5,
    .button_hover = 0xE0E0E0,
    .button_pressed = 0xCCCCCC,
    .button_text = 0x333333,
    .text_normal = 0x333333,
    .text_disabled = 0x999999,
    .selection_bg = 0x2196F3,
    .selection_text = 0xFFFFFF,
    .scrollbar_bg = 0xF0F0F0,
    .scrollbar_thumb = 0xBDBDBD,
    .desktop_bg = 0xE8E8E8,
    .taskbar_bg = 0xF5F5F5,
    .menu_bg = 0xFFFFFF
};

// Current theme
static theme_t* current_theme = &theme_default;

void theme_init(void) {
    current_theme = &theme_default;
}

void theme_set(theme_t* theme) {
    if (theme) {
        current_theme = theme;
    }
}

theme_t* theme_get_current(void) {
    return current_theme;
}