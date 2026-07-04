/* =============================================================================
 * NanoOS GUI Text Editor Application
 * =============================================================================
 */

#include "../gui.h"

#define MAX_LINES 100
#define LINE_HEIGHT 16

typedef struct {
    char lines[MAX_LINES][256];
    int line_count;
    int cursor_x;
    int cursor_y;
    int scroll_offset;
} text_editor_t;

static text_editor_t* editor = 0;

void text_editor_draw(window_t* win) {
    if (!editor) return;
    
    int content_x = win->x + BORDER_WIDTH + 2;
    int content_y = win->y + BORDER_WIDTH + TITLE_BAR_HEIGHT + 4;
    int content_w = win->width - BORDER_WIDTH * 2 - 4;
    int content_h = win->height - TITLE_BAR_HEIGHT - BORDER_WIDTH * 2 - 8;
    
    // Draw text area background
    draw_rect(content_x, content_y, content_w, content_h, COLOR_WHITE);
    
    // Draw line numbers
    for (int i = 0; i < editor->line_count && i < content_h / LINE_HEIGHT; i++) {
        char num[16];
        int_to_string(i + 1, num);
        draw_text(content_x + 4, content_y + i * LINE_HEIGHT, num, COLOR_GRAY);
    }
    
    // Draw text
    for (int i = 0; i < editor->line_count && i < content_h / LINE_HEIGHT; i++) {
        int line_idx = i + editor->scroll_offset;
        if (line_idx < editor->line_count) {
            draw_text(content_x + 40, content_y + i * LINE_HEIGHT, 
                     editor->lines[line_idx], COLOR_BLACK);
        }
    }
    
    // Draw cursor
    int cursor_draw_x = content_x + 40 + editor->cursor_x * 8;
    int cursor_draw_y = content_y + editor->cursor_y * LINE_HEIGHT;
    draw_rect(cursor_draw_x, cursor_draw_y, 8, LINE_HEIGHT, COLOR_BLUE);
}

void text_editor_handle_event(window_t* win, event_t* event) {
    if (!editor) return;
    
    if (event->type == EVENT_KEY_PRESS) {
        switch (event->key) {
            case 0x08: // Backspace
                if (editor->cursor_x > 0) {
                    editor->cursor_x--;
                    // Remove character from line
                }
                break;
            case 0x0D: // Enter
                if (editor->line_count < MAX_LINES - 1) {
                    editor->line_count++;
                    editor->cursor_y++;
                    editor->cursor_x = 0;
                }
                break;
            case 0x25: // Left arrow
                if (editor->cursor_x > 0) editor->cursor_x--;
                break;
            case 0x27: // Right arrow
                if (editor->cursor_x < 255) editor->cursor_x++;
                break;
            case 0x26: // Up arrow
                if (editor->cursor_y > 0) {
                    editor->cursor_y--;
                } else if (editor->scroll_offset > 0) {
                    editor->scroll_offset--;
                }
                break;
            case 0x28: // Down arrow
                if (editor->cursor_y < 20 && editor->cursor_y < editor->line_count - 1) {
                    editor->cursor_y++;
                } else if (editor->scroll_offset < editor->line_count - 20) {
                    editor->scroll_offset++;
                }
                break;
            default:
                // Add character
                if (event->key >= 0x20 && event->key <= 0x7E && editor->cursor_x < 255) {
                    editor->lines[editor->cursor_y][editor->cursor_x] = (char)event->key;
                    editor->cursor_x++;
                }
                break;
        }
    }
}

void text_editor_open(window_t* win) {
    if (!editor) {
        editor = (text_editor_t*)kmalloc(sizeof(text_editor_t));
        editor->line_count = 1;
        editor->cursor_x = 0;
        editor->cursor_y = 0;
        editor->scroll_offset = 0;
        editor->lines[0][0] = '\0';
    }
    
    win->draw = text_editor_draw;
    win->handle_event = text_editor_handle_event;
    win->user_data = editor;
}

void int_to_string(int num, char* str) {
    char temp[16];
    int i = 0;
    int j = 0;
    
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    while (num > 0) {
        temp[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    // Reverse
    for (j = 0; j < i; j++) {
        str[j] = temp[i - 1 - j];
    }
    str[j] = '\0';
}