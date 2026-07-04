/* =============================================================================
 * NanoOS GUI File Manager Application
 * =============================================================================
 */

#include "../gui.h"

#define MAX_FILES 100
#define FILE_ICON_SIZE 48

typedef struct {
    char files[MAX_FILES][256];
    char current_path[256];
    int file_count;
    int selected_index;
    int scroll_offset;
    int view_mode;  // 0 = list, 1 = icon
} file_manager_t;

static file_manager_t* fm = 0;

// File icons
typedef enum {
    FILE_TYPE_UNKNOWN,
    FILE_TYPE_FOLDER,
    FILE_TYPE_TEXT,
    FILE_TYPE_IMAGE,
    FILE_TYPE_AUDIO,
    FILE_TYPE_VIDEO
} file_type_t;

const char* file_type_names[] = {
    "Unknown", "Folder", "Text File", "Image", "Audio", "Video"
};

file_type_t get_file_type(const char* filename) {
    const char* ext = filename;
    while (*filename) {
        if (*filename == '.') ext = filename + 1;
        filename++;
    }
    
    // Folders
    if (ext == filename) return FILE_TYPE_FOLDER;
    
    // Text files
    if (strcmp(ext, "txt") == 0 || strcmp(ext, "md") == 0 ||
        strcmp(ext, "c") == 0 || strcmp(ext, "h") == 0 ||
        strcmp(ext, "sh") == 0) return FILE_TYPE_TEXT;
    
    // Images
    if (strcmp(ext, "bmp") == 0 || strcmp(ext, "png") == 0 ||
        strcmp(ext, "jpg") == 0 || strcmp(ext, "gif") == 0) return FILE_TYPE_IMAGE;
    
    return FILE_TYPE_UNKNOWN;
}

void file_manager_draw_icon(int x, int y, const char* name, file_type_t type) {
    uint32_t bg_color;
    
    switch (type) {
        case FILE_TYPE_FOLDER: bg_color = 0xFFD700; break;  // Gold
        case FILE_TYPE_TEXT: bg_color = 0xFFFFFF; break;     // White
        case FILE_TYPE_IMAGE: bg_color = 0x90EE90; break;    // Light green
        case FILE_TYPE_AUDIO: bg_color = 0xFFB6C1; break;   // Light pink
        case FILE_TYPE_VIDEO: bg_color = 0xADD8E6; break;    // Light blue
        default: bg_color = COLOR_LIGHT_GRAY;
    }
    
    // Draw icon background
    draw_rect(x, y, FILE_ICON_SIZE, FILE_ICON_SIZE, bg_color);
    draw_rect(x, y, FILE_ICON_SIZE, 2, COLOR_DARK_GRAY);
    draw_rect(x, y, 2, FILE_ICON_SIZE, COLOR_DARK_GRAY);
    draw_rect(x + FILE_ICON_SIZE - 2, y, 2, FILE_ICON_SIZE, COLOR_DARK_GRAY);
    draw_rect(x, y + FILE_ICON_SIZE - 2, FILE_ICON_SIZE, 2, COLOR_DARK_GRAY);
    
    // Draw icon symbol
    if (type == FILE_TYPE_FOLDER) {
        draw_text(x + 12, y + 8, "[D]", COLOR_BLACK);
    } else {
        draw_text(x + 12, y + 8, "[F]", COLOR_BLACK);
    }
    
    // Draw filename (truncated)
    char display_name[13];
    int i = 0;
    while (name[i] && i < 12) {
        display_name[i] = name[i];
        i++;
    }
    display_name[i] = '\0';
    
    draw_text(x, y + FILE_ICON_SIZE + 4, display_name, COLOR_BLACK);
}

void file_manager_draw(window_t* win) {
    if (!fm) return;
    
    int content_x = win->x + BORDER_WIDTH + 2;
    int content_y = win->y + BORDER_WIDTH + TITLE_BAR_HEIGHT + 4;
    int content_w = win->width - BORDER_WIDTH * 2 - 4;
    int content_h = win->height - TITLE_BAR_HEIGHT - BORDER_WIDTH * 2 - 8;
    
    // Draw toolbar
    draw_rect(content_x, content_y, content_w, 28, COLOR_LIGHT_GRAY);
    draw_button(content_x + 4, content_y + 4, 60, 20, "Back", COLOR_DARK_GRAY, COLOR_WHITE);
    draw_button(content_x + 68, content_y + 4, 60, 20, "Up", COLOR_DARK_GRAY, COLOR_WHITE);
    
    // Draw path bar
    int path_y = content_y + 32;
    draw_rect(content_x, path_y, content_w, 24, COLOR_WHITE);
    draw_rect(content_x, path_y, content_w, 1, COLOR_GRAY);
    draw_text(content_x + 4, path_y + 4, fm->current_path, COLOR_BLACK);
    
    // Draw file list
    int list_y = path_y + 28;
    int list_h = content_h - (list_y - content_y) - 24;
    
    // Draw selection highlight
    if (fm->selected_index >= 0 && fm->selected_index < fm->file_count) {
        int sel_y = list_y + fm->selected_index * 20 - fm->scroll_offset * 20;
        if (sel_y >= list_y && sel_y < list_y + list_h) {
            draw_rect(content_x, sel_y, content_w - 24, 20, 0xDDEEFF);
        }
    }
    
    // Draw files
    for (int i = fm->scroll_offset; i < fm->file_count && i < fm->scroll_offset + list_h / 20; i++) {
        int item_y = list_y + (i - fm->scroll_offset) * 20;
        
        file_type_t type = get_file_type(fm->files[i]);
        
        // Draw type indicator
        const char* type_name = file_type_names[type];
        draw_text(content_x + 4, item_y + 4, type_name, COLOR_GRAY);
        
        // Draw filename
        draw_text(content_x + 80, item_y + 4, fm->files[i], COLOR_BLACK);
    }
    
    // Draw status bar
    int status_y = content_y + content_h - 20;
    char status[128];
    // Format: "X items, Y selected"
    // This would need a sprintf-like function
    
    draw_rect(content_x, status_y, content_w, 20, COLOR_LIGHT_GRAY);
    char item_count[32] = "X items";
    draw_text(content_x + 4, status_y + 4, item_count, COLOR_BLACK);
}

void file_manager_handle_event(window_t* win, event_t* event) {
    if (!fm) return;
    
    if (event->type == EVENT_KEY_PRESS) {
        switch (event->key) {
            case 0x26: // Up arrow
                if (fm->selected_index > 0) fm->selected_index--;
                break;
            case 0x28: // Down arrow
                if (fm->selected_index < fm->file_count - 1) fm->selected_index++;
                break;
            case 0x0D: // Enter
                // Open file/folder
                break;
        }
    }
}

void file_manager_open(window_t* win) {
    if (!fm) {
        fm = (file_manager_t*)kmalloc(sizeof(file_manager_t));
        
        // Set default path
        strcpy(fm->current_path, "/");
        fm->file_count = 5;
        fm->selected_index = 0;
        fm->scroll_offset = 0;
        fm->view_mode = 0;
        
        // Sample files
        strcpy(fm->files[0], "Documents");
        strcpy(fm->files[1], "Downloads");
        strcpy(fm->files[2], "readme.txt");
        strcpy(fm->files[3], "photo.png");
        strcpy(fm->files[4], "music.mp3");
    }
    
    win->draw = file_manager_draw;
    win->handle_event = file_manager_handle_event;
    win->user_data = fm;
}

int strcmp(const char* s1, const char* s2);
int strcpy(char* dest, const char* src);
int strlen(const char* s);

int strcmp(const char* s1, const char* s2) {
    while (*s1 && *s2) {
        if (*s1 != *s2) return *s1 - *s2;
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

int strcpy(char* dest, const char* src) {
    int i = 0;
    while (src[i]) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return i;
}

int strlen(const char* s) {
    int len = 0;
    while (s[len]) len++;
    return len;
}