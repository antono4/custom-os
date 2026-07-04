# 🖥️ NanoOS GUI Version

Versi graphical user interface (GUI) untuk NanoOS dengan window manager dan aplikasi desktop.

## 📦 Komponen GUI

### Window Manager (`wm/`)
- Simple stacking window manager
- Window focus management
- Drag-and-drop window movement
- Window resize handles
- Desktop dengan taskbar

### GUI System (`gui.c/h`)
- Framebuffer rendering
- Window management
- Event handling (mouse, keyboard)
- Drawing primitives (pixel, rectangle, line, text)
- Button widgets

### Applications (`apps/`)
| Aplikasi | File | Fungsi |
|----------|------|--------|
| Text Editor | `text_editor.c` | Editor teks asas |
| Terminal | `terminal.c` | Terminal emulator |
| File Manager | `file_manager.c` | File explorer |
| Calculator | `calculator.c` | Kalkulator desktop |

### Theme System (`theme/`)
- 3 predefined themes: Default, Dark, Light
- Consistent styling untuk semua komponen
- Customizable colors

### Icons (`icons/`)
- Computer icon
- Folder icon
- Dan lain-lain (extensible)

## 🏗️ Struktur GUI

```
gui/
├── gui.h/c             # Core GUI system
├── wm/
│   ├── wm.h            # Window manager header
│   └── wm.c            # Window manager implementation
├── apps/
│   ├── text_editor.c   # Text editor application
│   ├── terminal.c       # Terminal emulator
│   ├── file_manager.c   # File manager
│   └── calculator.c     # Calculator
├── theme/
│   ├── theme.h         # Theme system header
│   └── theme.c         # Theme implementation
├── icons/
│   ├── icons.h         # Icon system header
│   └── icons.c         # Icon data
└── README.md           # This file
```

## 🚀 Penggunaan

### Inisialisasi GUI
```c
#include "gui/gui.h"
#include "gui/wm/wm.h"

// Initialize GUI system
gui_init();
wm_init();
```

### Membuat Window
```c
window_t* win = window_create("My Window", 100, 100, 400, 300);
window_set_focus(win);
```

### Menambah Aplikasi
```c
// Open terminal
window_t* term_win = window_create("Terminal", 50, 50, 600, 400);
terminal_open(term_win);
```

## 🎨 Customization

### Mengganti Theme
```c
theme_t* new_theme = &theme_dark;  // or &theme_light
theme_set(new_theme);
```

### Custom Colors
```c
// Edit theme.c untuk menambah tema baru
theme_t my_custom_theme = {
    .window_bg = 0xF0F0F0,
    .title_bar = 0x2B579A,
    // ... customize colors
};
```

## 📋 Roadmap

- [x] Basic window manager
- [x] GUI applications (Terminal, Editor, File Manager, Calculator)
- [x] Theme system
- [ ] Icon system
- [ ] Menu system
- [ ] Drag-and-drop support
- [ ] Multi-monitor support
- [ ] Hardware acceleration
- [ ] Touch input support

## 📚 Referensi

- [NanoOS Main README](../README.md)
- [OSDev Wiki - Graphics](https://wiki.osdev.org/VGA_Hardware)
- [VBE (VESA BIOS Extensions)](https://wiki.osdev.org/VESA)