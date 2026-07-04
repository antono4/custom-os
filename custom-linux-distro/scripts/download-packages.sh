#!/bin/bash
# =============================================================================
# Download Packages Script for Custom Linux Distribution
# =============================================================================

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Configuration
LFS="${LFS:-/mnt/lfs}"
SOURCES_DIR="$LFS/sources"
LOG_DIR="$SOURCES_DIR/logs"

# URLs
MIRROR_BASE="https://ftp.gnu.org/gnu"
MIRROR_KERNEL="https://cdn.kernel.org/pub/linux/kernel"
MIRROR_OTHER="https://anduin.linuxfromscratch.org/LFS"

# Package list with URLs
declare -A PACKAGES=(
    # Toolchain
    ["binutils-2.41.tar.xz"]="$MIRROR_BASE/binutils/binutils-2.41.tar.xz"
    ["gcc-13.2.0.tar.xz"]="$MIRROR_BASE/gcc/gcc-13.2.0/gcc-13.2.0.tar.xz"
    ["glibc-2.38.tar.xz"]="$MIRROR_BASE/glibc/glibc-2.38.tar.xz"
    ["linux-6.4.tar.xz"]="$MIRROR_KERNEL/v6.4/linux-6.4.tar.xz"
    
    # Core utilities
    ["bash-5.2.tar.gz"]="$MIRROR_BASE/bash/bash-5.2/bash-5.2.tar.gz"
    ["coreutils-9.3.tar.xz"]="$MIRROR_BASE/coreutils/coreutils-9.3.tar.xz"
    ["diffutils-3.10.tar.xz"]="$MIRROR_BASE/diffutils/diffutils-3.10.tar.xz"
    ["file-5.45.tar.gz"]="$MIRROR_BASE/file/file-5.45.tar.gz"
    ["findutils-4.9.0.tar.xz"]="$MIRROR_BASE/findutils/findutils-4.9.0.tar.xz"
    ["gawk-5.2.2.tar.xz"]="$MIRROR_BASE/gawk/gawk-5.2.2.tar.xz"
    ["grep-3.11.tar.xz"]="$MIRROR_BASE/grep/grep-3.11.tar.xz"
    ["gzip-1.12.tar.xz"]="$MIRROR_BASE/gzip/gzip-1.12.tar.xz"
    ["m4-1.4.19.tar.xz"]="$MIRROR_BASE/m4/m4-1.4.19.tar.xz"
    ["make-4.4.1.tar.gz"]="$MIRROR_BASE/make/make-4.4.1.tar.gz"
    ["patch-2.7.6.tar.xz"]="$MIRROR_BASE/patch/patch-2.7.6.tar.xz"
    ["sed-4.9.tar.xz"]="$MIRROR_BASE/sed/sed-4.9.tar.xz"
    ["tar-1.34.tar.xz"]="$MIRROR_BASE/tar/tar-1.34.tar.xz"
    ["texinfo-7.0.3.tar.xz"]="$MIRROR_BASE/texinfo/texinfo-7.0.3.tar.xz"
    ["xz-5.4.4.tar.xz"]="$MIRROR_BASE/xz/xz-5.4.4.tar.xz"
    
    # Additional libraries
    ["gmp-6.3.0.tar.xz"]="$MIRROR_OTHER/LFS/temp-system-deps/gmp-6.3.0.tar.xz"
    ["mpfr-4.2.0.tar.xz"]="$MIRROR_OTHER/LFS/temp-system-deps/mpfr-4.2.0.tar.xz"
    ["mpc-1.3.1.tar.gz"]="$MIRROR_OTHER/LFS/temp-system-deps/mpc-1.3.1.tar.gz"
    ["isl-0.26.tar.bz2"]="$MIRROR_OTHER/LFS/temp-system-deps/isl-0.26.tar.bz2"
    
    # Test suites
    ["tcl8.6.13-src.tar.gz"]="$MIRROR_OTHER/LFS/temp-system-deps/tcl8.6.13-src.tar.gz"
    ["expect5.45.4.tar.gz"]="$MIRROR_OTHER/LFS/temp-system-deps/expect5.45.4.tar.gz"
    ["dejagnu-1.6.3.tar.gz"]="$MIRROR_OTHER/LFS/temp-system-deps/dejagnu-1.6.3.tar.gz"
    
    # Final system
    ["bzip2-1.0.8.tar.gz"]="$MIRROR_BASE/bzip2/bzip2-1.0.8.tar.gz"
    ["e2fsprogs-1.47.0.tar.gz"]="$MIRROR_BASE/e2fsprogs/e2fsprogs-1.47.0.tar.gz"
    ["kbd-2.5.1.tar.xz"]="$MIRROR_BASE/kbd/kbd-2.5.1.tar.xz"
    ["libcap-2.69.tar.xz"]="$MIRROR_BASE/libcap/libcap-2.69.tar.xz"
    ["openssl-3.1.2.tar.gz"]="$MIRROR_BASE/openssl/openssl-3.1.2.tar.gz"
    ["perl-5.36.1.tar.gz"]="$MIRROR_BASE/perl/perl-5.36.1.tar.gz"
    ["python-3.11.4.tar.xz"]="$MIRROR_BASE/python/python-3.11.4/python-3.11.4.tar.xz"
    ["shadow-4.13.tar.xz"]="$MIRROR_BASE/shadow/shadow-4.13.tar.xz"
    ["systemd-253.tar.gz"]="$MIRROR_BASE/systemd/systemd-253.tar.gz"
    ["man-db-2.11.3.tar.xz"]="$MIRROR_BASE/man-db/man-db-2.11.3.tar.xz"
    ["procps-ng-4.0.3.tar.xz"]="$MIRROR_OTHER/LFS/temp-system-deps/procps-ng-4.0.3.tar.xz"
    ["util-linux-2.38.1.tar.xz"]="$MIRROR_BASE/util-linux/util-linux-2.38.1.tar.xz"
    ["vim-9.0.1670.tar.gz"]="$MIRROR_BASE/vim/vim-9.0.1670.tar.gz"
    ["grub-2.06.tar.xz"]="$MIRROR_BASE/grub/grub-2.06.tar.xz"
    ["ncurses-6.4.tar.gz"]="$MIRROR_BASE/ncurses/ncurses-6.4.tar.gz"
    ["gettext-0.22.tar.xz"]="$MIRROR_BASE/gettext/gettext-0.22.tar.xz"
    ["readline-8.2.tar.gz"]="$MIRROR_BASE/readline/readline-8.2.tar.gz"
)

log() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Create directories
mkdir -p "$SOURCES_DIR"
mkdir -p "$LOG_DIR"

# Download a single package
download_package() {
    local pkg="$1"
    local url="$2"
    local dest="$SOURCES_DIR/$pkg"
    
    if [ -f "$dest" ]; then
        log "Package $pkg already exists, skipping..."
        return 0
    fi
    
    log "Downloading $pkg..."
    
    # Try wget first, fallback to curl
    if command -v wget &> /dev/null; then
        wget --no-check-certificate -O "$dest" "$url" 2>&1 | tee "$LOG_DIR/${pkg%.tar.*}.log"
    else
        curl -L -o "$dest" "$url" 2>&1 | tee "$LOG_DIR/${pkg%.tar.*}.log"
    fi
    
    if [ $? -eq 0 ]; then
        log "Successfully downloaded $pkg"
    else
        error "Failed to download $pkg"
        rm -f "$dest"
        return 1
    fi
}

# Main download process
main() {
    log "Starting package download for Custom Linux Distribution"
    log "Sources directory: $SOURCES_DIR"
    
    # Check for existing list
    if [ -f "$SOURCES_DIR/packages.txt" ]; then
        warn "Using existing package list"
    fi
    
    # Download all packages
    local failed=0
    for pkg in "${!PACKAGES[@]}"; do
        if ! download_package "$pkg" "${PACKAGES[$pkg]}"; then
            ((failed++))
        fi
    done
    
    # Report results
    log "Download complete!"
    log "Downloaded: $((${#PACKAGES[@]} - failed)) packages"
    
    if [ $failed -gt 0 ]; then
        error "Failed downloads: $failed packages"
        error "Check $LOG_DIR for error logs"
        return 1
    fi
    
    return 0
}

# Run main
main "$@"