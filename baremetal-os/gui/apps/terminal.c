/* =============================================================================
 * NanoOS GUI Terminal Emulator Application
 * =============================================================================
 */

#include "../gui.h"

#define TERM_COLS 80
#define TERM_ROWS 24
#define CMD_HISTORY_SIZE 50

typedef struct {
    char screen[TERM_ROWS][TERM_COLS];
    char command[256];
    int cursor_x;
    int cursor_y;
    int scroll_y;
    char* history[CMD_HISTORY_SIZE];
    int history_count;
    int history_index;
} terminal_t;

static terminal_t* term = 0;

extern int strcmp(const char*, const char*);
extern void terminal_putchar(char c);

void terminal_clear(void) {
    for (int y = 0; y < TERM_ROWS; y++) {
        for (int x = 0; x < TERM_COLS; x++) {
            term->screen[y][x] = ' ';
        }
    }
}

void terminal_scroll(void) {
    for (int y = 1; y < TERM_ROWS; y++) {
        for (int x = 0; x < TERM_COLS; x++) {
            term->screen[y - 1][x] = term->screen[y][x];
        }
    }
    for (int x = 0; x < TERM_COLS; x++) {
        term->screen[TERM_ROWS - 1][x] = ' ';
    }
}

void terminal_print(const char* str) {
    while (*str) {
        if (*str == '\n') {
            term->cursor_x = 0;
            term->cursor_y++;
            if (term->cursor_y >= TERM_ROWS) {
                terminal_scroll();
                term->cursor_y = TERM_ROWS - 1;
            }
        } else if (*str == '\r') {
            term->cursor_x = 0;
        } else {
            term->screen[term->cursor_y][term->cursor_x] = *str;
            term->cursor_x++;
            if (term->cursor_x >= TERM_COLS) {
                term->cursor_x = 0;
                term->cursor_y++;
                if (term->cursor_y >= TERM_ROWS) {
                    terminal_scroll();
                    term->cursor_y = TERM_ROWS - 1;
                }
            }
        }
        str++;
    }
}

void terminal_execute(const char* cmd) {
    terminal_print("\r\n");
    
    if (strcmp(cmd, "help") == 0) {
        terminal_print("Available commands:\r\n");
        terminal_print("  help     - Show this help\r\n");
        terminal_print("  clear    - Clear screen\r\n");
        terminal_print("  info     - System information\r\n");
        terminal_print("  memory   - Memory status\r\n");
        terminal_print("  date     - Show date/time\r\n");
        terminal_print("  echo     - Echo text\r\n");
        terminal_print("  whoami   - Show current user\r\n");
    }
    else if (strcmp(cmd, "clear") == 0) {
        terminal_clear();
        term->cursor_x = term->cursor_y = 0;
    }
    else if (strcmp(cmd, "info") == 0) {
        terminal_print("NanoOS GUI Terminal v1.0\r\n");
        terminal_print("Kernel: NanoOS 0.1\r\n");
        terminal_print("Architecture: i386\r\n");
    }
    else if (strcmp(cmd, "memory") == 0) {
        terminal_print("Total Memory: 32 MB\r\n");
        terminal_print("Used: 8 MB\r\n");
        terminal_print("Free: 24 MB\r\n");
    }
    else if (strcmp(cmd, "date") == 0) {
        terminal_print("2024-01-15 12:00:00\r\n");
    }
    else if (strcmp(cmd, "whoami") == 0) {
        terminal_print("root@NanoOS\r\n");
    }
    else if (strncmp(cmd, "echo ", 5) == 0) {
        terminal_print(cmd + 5);
        terminal_print("\r\n");
    }
    else if (cmd[0] != '\0') {
        terminal_print("Unknown command: ");
        terminal_print(cmd);
        terminal_print("\r\n");
        terminal_print("Type 'help' for available commands.\r\n");
    }
}

void terminal_draw(window_t* win) {
    if (!term) return;
    
    int content_x = win->x + BORDER_WIDTH + 2;
    int content_y = win->y + BORDER_WIDTH + TITLE_BAR_HEIGHT + 4;
    int content_w = win->width - BORDER_WIDTH * 2 - 4;
    int content_h = win->height - TITLE_BAR_HEIGHT - BORDER_WIDTH * 2 - 8;
    
    // Draw terminal background (black)
    draw_rect(content_x, content_y, content_w, content_h, COLOR_BLACK);
    
    // Draw terminal content
    for (int y = 0; y < TERM_ROWS; y++) {
        for (int x = 0; x < TERM_COLS; x++) {
            if (term->screen[y][x] != ' ') {
                draw_text(content_x + x * 6, content_y + y * 12, 
                         (char[]){term->screen[y][x], '\0'}, COLOR_GREEN);
            }
        }
    }
    
    // Draw cursor
    int cursor_draw_x = content_x + term->cursor_x * 6;
    int cursor_draw_y = content_y + term->cursor_y * 12;
    draw_rect(cursor_draw_x, cursor_draw_y, 6, 12, COLOR_GREEN);
}

void terminal_handle_event(window_t* win, event_t* event) {
    if (!term) return;
    
    if (event->type == EVENT_KEY_PRESS) {
        if (event->key == 0x0D) { // Enter
            term->command[term->cursor_x] = '\0';
            terminal_execute(term->command);
            terminal_print("\r\nNanoOS> ");
            term->cursor_x = 0;
        }
        else if (event->key == 0x08) { // Backspace
            if (term->cursor_x > 0) {
                term->cursor_x--;
                term->command[term->cursor_x] = '\0';
            }
        }
        else if (event->key >= 0x20 && event->key <= 0x7E) {
            if (term->cursor_x < 255) {
                term->command[term->cursor_x] = (char)event->key;
                term->cursor_x++;
                term->screen[term->cursor_y][term->cursor_x - 1] = (char)event->key;
            }
        }
    }
}

void terminal_open(window_t* win) {
    if (!term) {
        term = (terminal_t*)kmalloc(sizeof(terminal_t));
        terminal_clear();
        term->cursor_x = 0;
        term->cursor_y = 0;
        term->scroll_y = 0;
        term->history_count = 0;
        term->history_index = 0;
        
        terminal_print("NanoOS Terminal v1.0\r\n");
        terminal_print("Type 'help' for available commands.\r\n\r\n");
        terminal_print("NanoOS> ");
    }
    
    win->draw = terminal_draw;
    win->handle_event = terminal_handle_event;
    win->user_data = term;
}

int strncmp(const char* s1, const char* s2, int n) {
    for (int i = 0; i < n; i++) {
        if (s1[i] != s2[i]) return 1;
        if (s1[i] == '\0') return 0;
    }
    return 0;
}