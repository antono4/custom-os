# 🔧 Troubleshooting Guide

Panduan troubleshooting untuk masalah yang sering terjadi selama build Custom Linux Distribution.

## 📑 Daftar Masalah Umum

### 1. Build Errors

#### Problem: GCC tidak dapat membuat executable
```
configure: error: in `/mnt/lfs/sources/gcc-13.2.0':
configure: error: C compiler cannot create executables
```

**Solusi:**
```bash
# Check GCC installation
gcc --version

# Verify prerequisites
make clean
./configure --prefix=/usr \
    --with-local-prefix=/usr/local \
    --with-native-system-header-dir=/usr/include
make
make install
```

#### Problem: Glibc build fails
```
make[2]: *** [Others] Error 1
```

**Solusi:**
```bash
# Ensure headers are correct
make install-headers
make -j1
make install
```

#### Problem: Binutils build error
```
configure: error: no native linker found
```

**Solusi:**
```bash
# Use system linker for initial build
./configure --prefix=/usr \
    --enable-shared \
    --disable-multilib \
    --with-lib-path=/usr/lib:/lib
```

### 2. Runtime Errors

#### Problem: Kernel Panic - VFS: Unable to mount root fs
```
VFS: Cannot open root device "sda2" or unknown-block(0,0)
```

**Solusi:**
1. Verifikasi `/etc/fstab`:
```bash
# Pastikan format benar
/dev/sda2     /               ext4    defaults        1       1
```

2. Rebuild kernel dengan filesystem support:
```bash
cd /usr/src/linux-6.4
make menuconfig
# Enable: File systems -> Ext4
make -j$(nproc)
make modules_install
cp arch/x86/boot/bzImage /boot/vmlinuz-6.4-custom
```

3. Update GRUB:
```bash
update-grub
# Atau manual:
grub-mkconfig -o /boot/grub/grub.cfg
```

#### Problem: Cannot login
```
login: Authentication failure
```

**Solusi:**
```bash
# Reset password
sudo chroot /mnt/lfs /bin/bash
passwd root

# Atau buat user baru
useradd -m -G wheel,audio,video -s /bin/bash username
passwd username
```

#### Problem: No network interface
```
eth0: error fetching interface information
```

**Solusi:**
```bash
# Check kernel modules
modprobe e1000  # Intel
modprobe r8169  # Realtek

# Add to /etc/modules
echo e1000 >> /etc/modules
```

### 3. Library Errors

#### Problem: libc.so.6: cannot open shared object file
```
/lib/ld-linux.so.2: error loading libc.so.6
```

**Solusi:**
```bash
# Rebuild Glibc
make clean
make -j$(nproc)
make install

# Update library cache
ldconfig
```

#### Problem: Segmentation fault saat boot
```
Segmentation fault
kernel panic - not syncing: Attempted to kill init!
```

**Solusi:**
1. Check memory:
```bash
memtest86+
```

2. Verify compiler flags:
```bash
# Use compatible flags di GCC
CFLAGS="-O2 -march=i686"
export CFLAGS
```

3. Rebuild dengan debugging:
```bash
make clean
CFLAGS="-g -O0" ./configure
make -j1 2>&1 | tee build.log
```

### 4. Filesystem Errors

#### Problem: Read-only filesystem
```
EXT4-fs error: remounting filesystem read-only
```

**Solusi:**
```bash
# Check filesystem
fsck.ext4 -f /dev/sda2

# Remount rw
mount -o remount,rw /dev/sda2
```

#### Problem: Disk full during build
```
No space left on device
```

**Solusi:**
```bash
# Clean sources
rm -rf /mnt/lfs/sources/*.tar.*

# Or extend partition
fdisk /dev/sda
partprobe
resize2fs /dev/sda2
```

### 5. Boot Problems

#### Problem: GRUB tidak muncul
```
GNU GRUB loading...
```

**Solusi:**
```bash
# Reinstall GRUB
grub-install /dev/sda
grub-mkconfig -o /boot/grub/grub.cfg
```

#### Problem: Blank screen after GRUB
```
Loading Linux 6.4-custom...
```

**Solusi:**
1. Check video driver di kernel config:
```bash
# Enable framebuffer
Device Drivers ->
    Graphics support ->
        <*> Support for frame buffer devices
        <*> EFI Framebuffer Support
```

2. Disable quiet boot:
```bash
# Edit GRUB config, hapus 'quiet'
grub.cfg:
linux /boot/vmlinuz-6.4-custom root=/dev/sda2 ro
```

### 6. Network Errors

#### Problem: DNS resolution failed
```
ping: google.com: Name or service not known
```

**Solusi:**
```bash
# Check /etc/resolv.conf
cat /etc/resolv.conf

# Add DNS
echo "nameserver 8.8.8.8" >> /etc/resolv.conf
echo "nameserver 8.8.4.4" >> /etc/resolv.conf
```

#### Problem: No eth0
```
Device not found
```

**Solusi:**
```bash
# Check kernel config
CONFIG_NETDEVICES=y
CONFIG_ETHERNET=y
CONFIG_NET_VENDOR_INTEL=y

# Or use DHCP
dhcpcd eth0
```

### 7. Package Specific Issues

#### Python: SSL module not available
```
pip is configured with locations that require TLS/SSL
```

**Solusi:**
```bash
# Install OpenSSL development
./configure --prefix=/usr \
    --openssldir=/etc/ssl \
    zlib \
    enable-cms
make -j$(nproc)
make install

# Rebuild Python
make clean
./configure --with-ensurepip=install
make -j$(nproc)
make install
```

#### Vim: GUI features not available
```
vim: command not found
```

**Solusi:**
```bash
# Install dependencies
apt install libncurses5-dev libgnome2-dev \
    libgtk2.0-dev libatk1.0-dev \
    libcairo2-dev libx11-dev

# Configure with GUI
./configure --enable-gui=auto
make -j$(nproc)
make install
```

## 🛠️ Diagnostic Commands

### Check System Status
```bash
# Mount status
mount

# Disk usage
df -h

# Memory
free -m

# Kernel messages
dmesg | less

# Running processes
ps aux

# Loaded modules
lsmod
```

### Check Build Logs
```bash
# Find error logs
find /mnt/lfs/sources -name "*.log" -mtime -1

# View Binutils log
cat /mnt/lfs/sources/logs/binutils-*.log

# View GCC log
cat /mnt/lfs/sources/logs/gcc-*.log
```

## 📞 Need More Help?

Jika masalah tidak terlist di atas:

1. Check [Linux From Scratch Forum](https://www.linuxfromscratch.org/lfs/lists.html)
2. Check [LFS Wiki](https://wiki.lfsdev.nl/)
3. Buka issue di repository ini