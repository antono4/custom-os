# 🖥️ Custom Linux Distribution - GUI Version

Panduan untuk membangun versi GUI (Graphical User Interface) dari Custom Linux Distribution.

## 📦 Komponen GUI

### Display Server
- **X.org (X11)** - X Window System
- **Wayland** - Modern display server (optional)
- **VESA Driver** - Generic video driver
- **Framebuffer** - Direct framebuffer access

### Display Manager
- **LightDM** - Cross-desktop display manager
- GTK Greeter - GTK-based login screen

### Window Manager
- **Openbox** - Lightweight stacking window manager
- Configurable keybindings and themes

### Desktop Environment
- **LXDE** - Lightweight X11 Desktop Environment
  - PCManFM - File manager
  - LXTerminal - Terminal emulator
  - Leafpad - Text editor
  - And more...

### GUI Applications
| Aplikasi | Package | Fungsi |
|----------|---------|--------|
| Web Browser | Firefox/Chromium | Web browsing |
| File Manager | PCManFM/Thunar | File management |
| Text Editor | Geany/Mousepad | Text editing |
| Terminal | LXTerminal | Command line |
| Calculator | Galculator | Calculations |
| System Monitor | System Monitor | Process monitoring |
| Partition Editor | GParted | Disk partitioning |

## 🏗️ Build Steps

### 1. Build Base System
First build the base LFS system:
```bash
cd custom-linux-distro
sudo ./scripts/01-prepare-system.sh
sudo ./scripts/02-temp-system.sh
sudo ./scripts/03-final-system.sh
sudo ./scripts/04-config-system.sh
sudo ./scripts/05-bootloader.sh
```

### 2. Build GUI Environment
Then add the GUI layer:
```bash
cd gui/scripts
chmod +x build-gui.sh
sudo ./build-gui.sh
```

## 📁 Struktur Direktori GUI

```
custom-linux-distro/gui/
├── config/                 # GUI Configuration files
│   ├── xorg.conf         # X.org configuration
│   ├── lightdm.conf      # LightDM configuration
│   └── gtk-settings.ini  # GTK settings
├── apps/                  # Application package definitions
├── scripts/              # Build scripts
│   └── build-gui.sh     # Main GUI build script
└── README.md             # This file
```

## ⚙️ Konfigurasi

### X.org Configuration
The X server configuration is in `config/xorg.conf`. Key settings:
- Display resolution and refresh rates
- Input device configuration
- Video driver selection

### Display Manager
LightDM configuration in `config/lightdm.conf`:
- Auto-login settings
- Session types
- Greeter theme

### Window Manager
Openbox configuration files:
- `~/.config/openbox/rc.xml` - Main config
- `~/.config/openbox/menu.xml` - Application menu
- `~/.config/openbox/autostart` - Autostart apps

## 🎨 Theming

### GTK Themes
Supported GTK themes:
- Adwaita (default)
- Numix
- Arc
- Materia

### Icon Themes
- Adwaita (default)
- Faenza
- Papirus

### Window Manager Themes
Openbox themes available at:
- [Openbox.org Themes](http://openbox.org/wiki/Openbox_Themes)

## 🚀 Starting GUI

### Via Display Manager
```bash
systemctl start lightdm
```

### Via Startx
Add to `~/.xinitrc`:
```bash
exec openbox-session
# or
exec startlxde
```

Then run:
```bash
startx
```

## 📋 Package List

### Core GUI Packages
| Package | Version | Size | Purpose |
|---------|---------|------|---------|
| Xorg Server | 21.1.8 | ~15MB | X Window System |
| Mesa | 23.1.4 | ~30MB | OpenGL |
| GTK+3 | 3.24.37 | ~20MB | GUI Toolkit |
| LightDM | 2.0.8 | ~2MB | Display Manager |
| Openbox | 3.6.1 | ~1MB | Window Manager |
| LXDE | 0.99.5 | ~5MB | Desktop Env |

### Application Packages
| Package | Version | Size | Purpose |
|---------|---------|------|---------|
| Geany | 1.38 | ~10MB | Text Editor |
| PCManFM | 1.3.2 | ~2MB | File Manager |
| LXTerminal | 0.4.0 | ~500KB | Terminal |
| Galculator | 2.1.4 | ~200KB | Calculator |

## 🔧 Troubleshooting

### X Server won't start
```bash
# Check Xorg log
cat /var/log/Xorg.0.log

# Common fixes:
# 1. Check /var/log/Xorg.0.log for errors
# 2. Ensure correct video driver installed
# 3. Verify xorg.conf settings
```

### Resolution issues
```bash
# Generate new xorg.conf
Xorg -configure

# Or use xrandr
xrandr --output VGA1 --mode 1920x1080
```

### Display manager login loop
```bash
# Check .xsession-errors
cat ~/.xsession-errors

# Try starting with different session
exec openbox-session
```

## 📚 Referensi

- [X.org Documentation](https://www.x.org/wiki/)
- [Openbox Manual](http://openbox.org/wiki/Help:Contents)
- [LXDE Wiki](https://wiki.lxde.org/)
- [LightDM Documentation](https://github.com/canonical/lightdm)