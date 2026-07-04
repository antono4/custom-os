#!/bin/bash
# =============================================================================
# Test System Script - Validate the Custom Linux Distribution
# =============================================================================

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

PASSED=0
FAILED=0

pass() {
    echo -e "${GREEN}[PASS]${NC} $1"
    ((PASSED++))
}

fail() {
    echo -e "${RED}[FAIL]${NC} $1"
    ((FAILED++))
}

info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

# Test executables
test_executables() {
    info "Testing executables..."
    
    local cmds=("bash" "sh" "ls" "cat" "echo" "grep" "sed" "awk" "find" "tar" "gzip" "xz")
    
    for cmd in "${cmds[@]}"; do
        if command -v "$cmd" &> /dev/null; then
            pass "Executable: $cmd"
        else
            fail "Executable: $cmd (not found)"
        fi
    done
}

# Test libraries
test_libraries() {
    info "Testing libraries..."
    
    local libs=("libc.so.6" "libm.so.6" "libpthread.so.0" "libdl.so.2")
    
    for lib in "${libs[@]}"; do
        if ldconfig -p | grep -q "$lib"; then
            pass "Library: $lib"
        else
            fail "Library: $lib (not found)"
        fi
    done
}

# Test filesystem
test_filesystem() {
    info "Testing filesystem..."
    
    local dirs=("/bin" "/usr/bin" "/lib" "/usr/lib" "/etc" "/var" "/home")
    
    for dir in "${dirs[@]}"; do
        if [ -d "$dir" ]; then
            pass "Directory: $dir"
        else
            fail "Directory: $dir (not found)"
        fi
    done
}

# Test configuration files
test_config() {
    info "Testing configuration files..."
    
    local configs=("/etc/passwd" "/etc/group" "/etc/fstab" "/etc/hostname")
    
    for cfg in "${configs[@]}"; do
        if [ -f "$cfg" ]; then
            pass "Config: $cfg"
        else
            fail "Config: $cfg (not found)"
        fi
    done
}

# Test kernel modules
test_modules() {
    info "Testing kernel modules..."
    
    if [ -d "/lib/modules" ]; then
        pass "Kernel modules directory exists"
        
        local modules=$(ls /lib/modules 2>/dev/null | wc -l)
        info "Found $modules kernel module directories"
    else
        fail "Kernel modules directory not found"
    fi
}

# Test network
test_network() {
    info "Testing network..."
    
    if [ -f "/etc/resolv.conf" ]; then
        pass "Network configuration exists"
        
        if grep -q "nameserver" /etc/resolv.conf; then
            pass "DNS nameservers configured"
        else
            fail "DNS nameservers not configured"
        fi
    else
        fail "Network configuration not found"
    fi
}

# Summary
summary() {
    echo ""
    echo "=========================================="
    echo "           TEST SUMMARY"
    echo "=========================================="
    echo -e "Passed: ${GREEN}$PASSED${NC}"
    echo -e "Failed: ${RED}$FAILED${NC}"
    echo "=========================================="
    
    if [ $FAILED -eq 0 ]; then
        echo -e "${GREEN}All tests passed!${NC}"
        return 0
    else
        echo -e "${RED}Some tests failed.${NC}"
        return 1
    fi
}

# Main
main() {
    echo ""
    echo "=========================================="
    echo "  Custom Linux Distribution Test Suite"
    echo "=========================================="
    echo ""
    
    test_executables
    test_libraries
    test_filesystem
    test_config
    test_modules
    test_network
    
    summary
}

main "$@"