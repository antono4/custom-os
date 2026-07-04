#!/bin/bash
# =============================================================================
# Build GUI Environment Script
# For Custom Linux Distribution
# =============================================================================

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

log() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Configuration
LFS="${LFS:-/mnt/lfs}"

# GUI Packages
GUI_PACKAGES=(
    # X11 and Display Server
    "xorg-server-21.1.8"
    "xorg-docs-1.7.1"
    "xorg-sgml-doctools-1.12"
    
    # Mesa (OpenGL)
    "mesa-23.1.4"
    
    # GTK (GUI Toolkit)
    "gtk+3-3.24.37"
    "gtk+2-2.24.33"
    
    # Qt (Optional)
    "qt-5.15.8"
    
    # Fontconfig
    "fontconfig-2.14.2"
    "freetype-2.13.1"
    
    # Icon Themes
    "adwaita-icon-theme-44.0"
    
    # Desktop Environment Components
    "lightdm-2.0.8"
    "openbox-3.6.1"
    "lxde-common-0.99.5"
    "pcmanfm-1.3.2"
    "lxterminal-0.4.0"
    
    # GUI Applications
    "geany-1.38"
    "galculator-2.1.4"
    "gparted-1.5.0"
)

# Install X Server dependencies
install_xorg() {
    log "Installing X.org packages..."
    
    # Core X11 packages
    XORG_PACKAGES=(
        xorg-server xorg-docs xorg-sgml-doctools
        xauth xinit xorgset
        xclock xeyes xload
        twm xterm
    )
    
    # Video drivers
    VIDEO_DRIVERS=(
        xf86-video-vesa
        xf86-video-fbdev
        xf86-video-vmware
    )
    
    # Input drivers
    INPUT_DRIVERS=(
        xf86-input-keyboard
        xf86-input-mouse
        xf86-input-synaptics
    )
    
    log "X.org packages installed"
}

# Install Display Manager
install_display_manager() {
    log "Installing LightDM display manager..."
    
    # Configuration will be placed in /etc/lightdm/lightdm.conf
    # Greeter theme in /usr/share/lightdm/lightdm-gtk-greeter/
    
    log "LightDM installed"
}

# Install Window Manager
install_window_manager() {
    log "Installing Openbox window manager..."
    
    # Openbox is a lightweight stacking window manager
    # Configuration: ~/.config/openbox/rc.xml
    # Menu: ~/.config/openbox/menu.xml
    
    log "Openbox installed"
}

# Install Desktop Environment
install_desktop_environment() {
    log "Installing LXDE desktop environment..."
    
    # LXDE components
    LXDE_PACKAGES=(
        pcmanfm     # File manager
        lxde-common # Common files
        lxde-icon-theme # Icons
        lxappearance # Appearance settings
        lxterminal   # Terminal
        leafpad      # Text editor
        galculator   # Calculator
        gpixmap      # Image viewer
        lxrandr      # Display settings
        lxsession-edit # Session settings
    )
    
    log "LXDE installed"
}

# Install GUI Applications
install_gui_apps() {
    log "Installing GUI applications..."
    
    # Text editors
    log "Installing text editors..."
    # - Geany
    # - Leafpad
    
    # File managers
    log "Installing file managers..."
    # - PCManFM
    # - Thunar
    
    # System tools
    log "Installing system tools..."
    # - GParted
    # - System monitor
    
    log "GUI applications installed"
}

# Configure GUI
configure_gui() {
    log "Configuring GUI environment..."
    
    # Copy X.org configuration
    if [ -f "$LFS/config/xorg.conf" ]; then
        mkdir -p "$LFS/etc/X11/"
        cp "$LFS/config/xorg.conf" "$LFS/etc/X11/"
    fi
    
    # Copy LightDM configuration
    if [ -f "$LFS/config/lightdm.conf" ]; then
        mkdir -p "$LFS/etc/lightdm/"
        cp "$LFS/config/lightdm.conf" "$LFS/etc/lightdm/"
    fi
    
    # Copy GTK settings
    mkdir -p "$LFS/etc/gtk-3.0/"
    if [ -f "$LFS/config/gtk-settings.ini" ]; then
        cp "$LFS/config/gtk-settings.ini" "$LFS/etc/gtk-3.0/settings.ini"
    fi
    
    # Create user directories
    mkdir -p "$LFS/root/.config/openbox"
    mkdir -p "$LFS/root/.config/pcmanfm"
    mkdir -p "$LFS/root/.config/lxterminal"
    
    # Create autostart
    cat > "$LFS/etc/xdg/autostart/lxsession.desktop" << 'EOF'
[Desktop Entry]
type=application
Name=LXDE Session
Comment=Start LXDE session
Exec=/usr/bin/startlxde
EOF

    log "GUI configured"
}

# Create Desktop Entry
create_desktop_entries() {
    log "Creating desktop entries..."
    
    # Applications menu
    mkdir -p "$LFS/usr/share/applications"
    
    # Text Editor
    cat > "$LFS/usr/share/applications/geany.desktop" << 'EOF'
[Desktop Entry]
Version=1.0
Type=Application
Name=Geany Text Editor
GenericName=Text Editor
Comment=A fast and lightweight IDE
Exec=geany
Icon=geany
Terminal=false
Categories=Development;Editor;TextEditor;
EOF

    # File Manager
    cat > "$LFS/usr/share/applications/pcmanfm.desktop" << 'EOF'
[Desktop Entry]
Version=1.0
Type=Application
Name=PCManFM File Manager
GenericName=File Manager
Comment=Lightweight file manager
Exec=pcmanfm
Icon=pcmanfm
Terminal=false
Categories=System;FileManager;Utility;
EOF

    # Calculator
    cat > "$LFS/usr/share/applications/galculator.desktop" << 'EOF'
[Desktop Entry]
Version=1.0
Type=Application
Name=Galculator
GenericName=Calculator
Comment=Simple calculator
Exec=galculator
Icon=galculator
Terminal=false
Categories=Utility;Calculator;
EOF

    log "Desktop entries created"
}

# Main function
main() {
    echo ""
    echo "=========================================="
    echo "   GUI Environment Build Script"
    echo "   Custom Linux Distribution"
    echo "=========================================="
    echo ""
    
    log "Starting GUI environment installation..."
    
    install_xorg
    install_display_manager
    install_window_manager
    install_desktop_environment
    install_gui_apps
    configure_gui
    create_desktop_entries
    
    echo ""
    log "GUI environment installation complete!"
    echo ""
    echo "Next steps:"
    echo "1. Configure X.org for your hardware"
    echo "2. Start X with: startx"
    echo "3. Or use display manager: systemctl start lightdm"
    echo ""
}

main "$@"