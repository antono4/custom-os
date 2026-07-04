# 📚 Panduan Build Custom Linux Distribution

Panduan lengkap untuk membangun distribusi Linux kustom menggunakan metodologi Linux From Scratch (LFS).

## 📋 Prasyarat Sistem

### Hardware Minimum
| Komponen | Minimum | Disarankan |
|----------|---------|------------|
| CPU | Single Core 1GHz | Multi-core 2GHz+ |
| RAM | 4 GB | 8 GB+ |
| Disk | 30 GB | 100 GB+ |
| Network | Tidak wajib | Diperlukan untuk download packages |

### Software Prasyarat
- Linux host (disarankan Ubuntu 22.04+ atau Debian 12+)
- Akses root (sudo)
- Koneksi internet

### Paket yang Diperlukan
```bash
sudo apt update
sudo apt install -y \
    build-essential \
    binutils \
    bison \
    flex \
    gawk \
    gcc \
    g++ \
    make \
    patch \
    wget \
    xz-utils \
    tar \
    gzip \
    bc \
    rsync \
    git \
    vim
```

## 📁 Persiapan Direktori

### Variabel Lingkungan
```bash
export LFS=/mnt/lfs
export LFS_VERSION=6.4
```

### Buat Direktori
```bash
sudo mkdir -pv $LFS/{sources,tools,var/lib/lfs}
sudo mkdir -pv $LFS/{bin,etc,home,root}
sudo mkdir -pv $LFS/{usr/{bin,lib,src,sbin},var}
sudo mkdir -pv $LFS/{boot,dev,proc,sys,tmp}
sudo mkdir -pv $LFS/usr/local/{bin,lib,src}
```

## 📦 Download Packages

### Menggunakan Script
```bash
cd scripts
chmod +x download-packages.sh
sudo ./download-packages.sh
```

### Download Manual
```bash
cd $LFS/sources
wget --input-file=../packages/list.txt
```

## 🔨 Tahap 1: Build Temporary System

### Build Toolchain
```bash
cd scripts
sudo ./02-temp-system.sh
```

Script ini akan membangun:
1. **Binutils 2.41** - Binary utilities (linker, assembler)
2. **GCC 13.2.0** - GNU Compiler Collection
3. **Glibc 2.38** - GNU C Library
4. **Linux API Headers 6.4** - Kernel headers

### Build Temporary Tools
Script akan membangun semua utilitas sementara:
- Bash 5.2.15
- Coreutils 9.3
- Diffutils 3.10
- File 5.45
- Findutils 4.9.0
- Gawk 5.2.2
- Grep 3.11
- Gzip 1.12
- Make 4.4.1
- Patch 2.7.6
- Sed 4.9
- Tar 1.34
- Xz 5.4.4

## 🔧 Tahap 2: Build Final System

```bash
sudo ./03-final-system.sh
```

Script ini membangun sistem akhir dengan:
- Compiler (GCC 13.2.0)
- C Library (Glibc 2.38)
- System utilities (Coreutils, Shadow, etc.)
- Python 3.11.4
- Vim 9.0.1670
- Man-DB 2.11.3

## ⚙️ Tahap 3: Konfigurasi Sistem

```bash
sudo ./04-config-system.sh
```

### File Konfigurasi
- `/etc/passwd` - Database pengguna
- `/etc/group` - Database grup
- `/etc/fstab` - Filesystem mount table
- `/etc/hostname` - Nama host
- `/etc/hosts` - Resolusi hostname
- `/etc/locale.gen` - Locale settings
- `/etc/profile` - Environment variables
- `/etc/sysconfig/network` - Konfigurasi jaringan

### Buat Pengguna
```bash
sudo chroot "$LFS" /usr/bin/env -i \
    HOME=/root TERM="$TERM" PS1='\u:\w\$ ' \
    /bin/bash -c "/usr/sbin/useradd -m -G audio,video,wheel lfs"
```

## 🚀 Tahap 4: Bootloader

```bash
sudo ./05-bootloader.sh
```

### Install GRUB
```bash
sudo chroot "$LFS" /bin/bash -c "grub-install /dev/sda"
```

### Konfigurasi GRUB
```bash
sudo chroot "$LFS" /bin/bash -c "cat > /boot/grub/grub.cfg << 'EOF'
set default=0
set timeout=5

menuentry 'Custom LFS' {
    linux   /boot/vmlinuz-6.4-custom-lfs root=/dev/sda2 ro
}
EOF"
```

## 🧪 Testing

### Boot Test
1. Reboot sistem
2. Pilih "Custom LFS" dari menu GRUB
3. Verifikasi:
   - [ ] Kernel boots successfully
   - [ ] Filesystem mounts correctly
   - [ ] Login prompt appears
   - [ ] Basic commands work (ls, cat, etc.)

### Test Script
```bash
cd scripts
sudo ./test-system.sh
```

## 🔍 Troubleshooting

### Error: "gcc: command not found"
```bash
# Verify GCC installation
sudo chroot "$LFS" /usr/bin/env -i \
    HOME=/root PATH=/usr/bin:/bin \
    /usr/bin/gcc --version
```

### Error: "C compiler cannot create executables"
- Pastikan Binutils di-build sebelum GCC
- Check log files di `$LFS/sources/logs/`

### Error: "kernel panic - not syncing"
- Verifikasi filesystem di `/etc/fstab`
- Pastikan kernel image path benar di GRUB config

### Error: "cannot open shared object file"
- Jalankan `ldconfig` setelah build Glibc
- Check library paths di `/etc/ld.so.conf`

## 📊 Estimasi Waktu Build

| Tahap | Waktu |
|-------|-------|
| Persiapan | 30 menit |
| Temporary System | 2-3 jam |
| Final System | 3-4 jam |
| Konfigurasi | 30 menit |
| Bootloader | 15 menit |
| **Total** | **6-8 jam** |

## ✅ Checklist Pre-Build

- [ ] Hardware requirements met
- [ ] Software dependencies installed
- [ ] 30GB+ free disk space
- [ ] Backup data (recommended)
- [ ] Internet connection stable
- [ ] Root access available
- [ ] Variables set correctly

## 📚 Referensi

- [Linux From Scratch Official](https://www.linuxfromscratch.org/)
- [LFS Book](https://www.linuxfromscratch.org/lfs/view/stable/)
- [Beyond Linux From Scratch](https://www.linuxfromscratch.org/blfs/view/stable/)