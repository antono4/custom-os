# 🖥️ Custom-OS Project

<p align="center">
  <img src="https://img.shields.io/badge/Status-Active-brightgreen?style=flat-square" alt="Status">
  <img src="https://img.shields.io/badge/License-MIT-blue?style=flat-square" alt="License">
  <img src="https://img.shields.io/github/stars/antono4/custom-os?style=flat-square" alt="Stars">
  <img src="https://img.shields.io/github/forks/antono4/custom-os?style=flat-square" alt="Forks">
</p>

> Projek educativo untuk membangun sistem operasi dari nol dan distribusi Linux kustom menggunakan metodologi Linux From Scratch (LFS). Tersedia versi CLI dan GUI!

## 📖 Daftar Isi

- [Tentang Projek](#tentang-projek)
- [Bare-metal OS](#-bare-metal-os-os-dari-nol)
  - [CLI Version](#-cli-version)
  - [GUI Version](#-gui-version)
- [Custom Linux Distribution](#-custom-linux-distribution)
  - [CLI Version](#-cli-version-1)
  - [GUI Version](#-gui-version-1)
- [Mulai Cepat](#-mulai-cepat)
- [Fitur](#-fitur)
- [Struktur Direktori](#-struktur-direktori)
- [Kontribusi](#-kontribusi)
- [Lisensi](#-lisensi)

---

## 🎯 Tentang Projek

Projek **Custom-OS** berisi implementasi untuk memahami dan membangun sistem operasi:

| Proyek | Deskripsi | Kompleksitas | Cocok Untuk |
|--------|-----------|--------------|-------------|
| **Bare-metal OS** | OS dari nol tanpa kernel Linux | ⭐⭐ | Pembelajaran |
| **Custom Linux Distro** | Distro Linux menggunakan LFS | ⭐⭐⭐⭐⭐ | Produksi |

Setiap projek tersedia dalam **versi CLI** dan **versi GUI**!

---

## 💻 Bare-metal OS (OS dari Nol)

OS sederhana **NanoOS** yang dibuat dari awal tanpa menggunakan kernel Linux. Tersedia dalam versi CLI dan GUI.

### 🖥️ CLI Version

Versi command-line interface untuk pembelajaran konsep OS dasar.

### 🎨 GUI Version

Versi graphical user interface dengan window manager dan aplikasi desktop.

### Arsitektur

```
┌─────────────────────────────────────────────────────────────┐
│                    NanoOS Architecture                      │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│   ┌─────────────────────────────────────────────────────┐   │
│   │                   User Space                        │   │
│   │   ┌───────────────────┐  ┌────────────────────┐     │   │
│   │   │      Shell        │  │    Programs       │     │   │
│   │   └─────────┬─────────┘  └─────────┬─────────┘     │   │
│   │             └──────────┬──────────┘               │   │
│   │                    ┌────┴────┐                     │   │
│   │                    │  libc   │                     │   │
│   └────────────────────┴────┬────┴─────────────────────┘   │
│                             │                              │
│   ┌─────────────────────────┴─────────────────────────────┐ │
│   │                   Kernel Space                        │ │
│   │   ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌──────────┐  │ │
│   │   │   VFS   │ │   MMU   │ │   IPC   │ │ Syscalls │  │ │
│   │   ├─────────┤ ├─────────┤ ├─────────┤ ├──────────┤  │ │
│   │   │ Process │ │ Device  │ │  File   │ │  Task    │  │ │
│   │   │   Mgr   │ │   Drv   │ │  System │ │  Sched   │  │ │
│   └──────────────────────────────────────────────────────┘ │
│                             │                              │
│   ┌─────────────────────────┴─────────────────────────────┐ │
│   │                  Hardware (x86/ARM)                  │ │
│   └──────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────┘
```

### Fitur

| Fitur | Status | Deskripsi |
|-------|--------|-----------|
| 🔧 Bootloader | ✅ | Stage 1 & 2 bootloader dengan GRUB multiboot |
| 🖥️ Kernel 32-bit | ✅ | Kernel mode protegido i386 |
| 📋 GDT/IDT | ✅ | Global & Interrupt Descriptor Tables |
| 🧠 Memory Mgmt | ✅ | Paging & memory management |
| 📁 Virtual File System | ✅ | Abstraction layer untuk filesystem |
| ⌨️ Keyboard Driver | ✅ | Interrupt-driven keyboard handler |
| ⏱️ Timer Driver | ✅ | Programmable Interval Timer (PIT) |
| 🐚 Interactive Shell | ✅ | Command-line interface |
| 📊 Task Management | ✅ | Multitasking & scheduling |
| 🔔 System Calls | ✅ | Userspace to kernel interface |
| 💾 FAT12 Support | ✅ | Basic filesystem reading |

### Program Tersedia

| Program | File | Deskripsi |
|---------|------|----------|
| Hello World | `programs/hello.c` | Program demonstrasi sederhana |
| Echo | `programs/echo.c` | Echo input ke output |
| Calculator | `programs/calculator.c` | Kalkulator sederhana |
| Memory Info | `programs/meminfo.c` | Informasi memori sistem |
| Process List | `programs/ps.c` | Daftar proses berjalan |
| Date/Time | `programs/date.c` | Tampilkan tanggal & waktu |
| Help | `programs/help.c` | Bantuan perintah shell |

### Build & Run

```bash
# Clone repository
git clone https://github.com/antono4/custom-os.git
cd custom-os/baremetal-os

# Install dependencies (Ubuntu/Debian)
sudo apt install build-essential binutils gcc make grub-common xorriso qemu-system-x86

# Build
make clean
make all

# Run di QEMU
make run

# Build ISO image
make iso
```

---

## 🐧 Custom Linux Distribution

Distro Linux kustom menggunakan metodologi **Linux From Scratch (LFS)**. Projek ini membangun distribusi Linux dari source code secara manual.

### Arsitektur Build

```
┌─────────────────────────────────────────────────────────────┐
│              Linux From Scratch Build Process               │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌──────────────────────────────────────────────────────┐   │
│  │ Phase 1: System Preparation                          │   │
│  │ • Partition & Filesystem Creation                   │   │
│  │ • Download Source Packages                          │   │
│  │ • Set Up Build Environment                         │   │
│  └──────────────────────────────────────────────────────┘   │
│                            │                                │
│                            ▼                                │
│  ┌──────────────────────────────────────────────────────┐   │
│  │ Phase 2: Temporary System Build                      │   │
│  │ • Binutils 2.41  • GCC 13.2.0  • Glibc 2.38        │   │
│  │ • GMP, MPFR, MPC • Linux API Headers               │   │
│  │ • Tcl, Expect, DejaGNU (Test Suite)                 │   │
│  └──────────────────────────────────────────────────────┘   │
│                            │                                │
│                            ▼                                │
│  ┌──────────────────────────────────────────────────────┐   │
│  │ Phase 3: Final System Build                          │   │
│  │ • Coreutils • Bash • File • Gawk • Grep • Gzip     │   │
│  │ • Make • Patch • Sed • Tar • Xz                     │   │
│  │ • Python 3.11 • Vim 9.0 • Perl 5.36                │   │
│  └──────────────────────────────────────────────────────┘   │
│                            │                                │
│                            ▼                                │
│  ┌──────────────────────────────────────────────────────┐   │
│  │ Phase 4: System Configuration                         │   │
│  │ • /etc/passwd, /etc/group, /etc/fstab               │   │
│  │ • Network Setup • Systemd Configuration              │   │
│  └──────────────────────────────────────────────────────┘   │
│                            │                                │
│                            ▼                                │
│  ┌──────────────────────────────────────────────────────┐   │
│  │ Phase 5: Bootloader & Kernel                         │   │
│  │ • GRUB 2.06 Installation                            │   │
│  │ • Linux Kernel 6.4 Configuration & Build            │   │
│  └──────────────────────────────────────────────────────┘   │
│                            │                                │
│                            ▼                                │
│              🎉 YOUR CUSTOM LINUX DISTRO! 🎉                │
└─────────────────────────────────────────────────────────────┘
```

### Fitur

| Komponen | Versi | Fungsi |
|----------|-------|--------|
| 🔨 GCC | 13.2.0 | GNU Compiler Collection |
| 📚 Glibc | 2.38 | GNU C Library |
| 🐧 Linux Kernel | 6.4 | Operating System Kernel |
| 🐚 Bash | 5.2 | Bourne Again Shell |
| 🐍 Python | 3.11 | Python Interpreter |
| 📝 Vim | 9.0 | Text Editor |
| 🖥️ GRUB | 2.06 | Bootloader |
| ⚙️ Systemd | 253 | System & Service Manager |

### Build Steps

```bash
cd custom-linux-distro

# Set permissions
chmod +x scripts/*.sh

# 1. Download packages
./scripts/download-packages.sh

# 2. Persiapan sistem
sudo ./scripts/01-prepare-system.sh

# 3. Build temporary system
sudo ./scripts/02-temp-system.sh

# 4. Build final system
sudo ./scripts/03-final-system.sh

# 5. Konfigurasi
sudo ./scripts/04-config-system.sh

# 6. Bootloader
sudo ./scripts/05-bootloader.sh

# Test system
sudo ./scripts/test-system.sh
```

---

## 🚀 Mulai Cepat

### Prasyarat Sistem

| Komponen | Bare-metal OS | Custom Linux Distro |
|----------|--------------|---------------------|
| CPU | x86 32-bit | x86/x86_64 |
| RAM | 256 MB | 4 GB (minimum) |
| Disk | 1 GB | 30 GB+ |
| OS Host | Linux/Unix | Linux |

### Install Dependencies

```bash
# Ubuntu/Debian
sudo apt update
sudo apt install -y \
    build-essential \
    binutils \
    gcc \
    g++ \
    make \
    grub-common \
    xorriso \
    qemu-system-x86 \
    wget \
    xz-utils \
    git
```

---

## ⭐ Fitur

### Bare-metal OS
- ✅ Zero-dependency kernel
- ✅ Bootloader multiboot
- ✅ Memory management dengan paging
- ✅ Interrupt handling
- ✅ Virtual File System
- ✅ Task scheduling
- ✅ System calls
- ✅ Interactive shell
- ✅ Device drivers (keyboard, timer)
- ✅ FAT12 filesystem support

### Custom Linux Distribution
- ✅ Full Linux From Scratch build
- ✅ Latest stable packages
- ✅ Systemd init system
- ✅ Python interpreter
- ✅ Vim text editor
- ✅ Complete toolchain
- ✅ Bootable ISO
- ✅ Ext4 filesystem support

---

## 📁 Struktur Direktori

```
custom-os/
│
├── baremetal-os/
│   ├── bootloader/           # Stage 1 & 2 bootloader
│   │   ├── stage1.S
│   │   ├── stage2.S
│   │   ├── linker.ld
│   │   └── idt_flush.S
│   ├── kernel/               # Kernel source
│   │   ├── main.c
│   │   ├── gdt.c/h
│   │   ├── idt.c/h
│   │   ├── memory.c/h
│   │   ├── vfs.c/h
│   │   ├── task.c/h
│   │   ├── syscall.c/h
│   │   └── devices/
│   │       ├── keyboard.c
│   │       └── timer.c
│   ├── libc/                # C library
│   │   ├── stdio.c
│   │   └── string.c
│   ├── shell/                # Shell
│   │   └── shell.c
│   ├── programs/             # User programs
│   │   ├── hello.c
│   │   ├── echo.c
│   │   ├── calculator.c
│   │   ├── meminfo.c
│   │   ├── ps.c
│   │   ├── date.c
│   │   └── help.c
│   ├── Makefile
│   └── README.md
│
├── custom-linux-distro/
│   ├── scripts/              # Build scripts
│   │   ├── 01-prepare-system.sh
│   │   ├── 02-temp-system.sh
│   │   ├── 03-final-system.sh
│   │   ├── 04-config-system.sh
│   │   ├── 05-bootloader.sh
│   │   ├── download-packages.sh
│   │   └── test-system.sh
│   ├── packages/             # Package list
│   │   └── list.txt
│   ├── config/               # Configuration
│   │   ├── kernel-config
│   │   ├── fstab
│   │   ├── hostname
│   │   ├── hosts
│   │   ├── locale.gen
│   │   ├── network
│   │   ├── profile
│   │   └── bashrc
│   └── docs/                 # Documentation
│       ├── BUILD.md
│       └── TROUBLESHOOT.md
│
├── .github/
│   └── workflows/
│       └── ci.yml            # CI/CD pipeline
│
├── CONTRIBUTING.md            # Contribution guidelines
├── README.md                  # This file
└── LICENSE                    # MIT License
```

---

## 🤝 Kontribusi

Kontribusi sangat diterima! Silakan baca [CONTRIBUTING.md](CONTRIBUTING.md) untuk detail.

### Langkah Kontribusi

1. Fork repository
2. Buat branch fitur (`git checkout -b feature/amazing-feature`)
3. Commit perubahan (`git commit -m 'feat: add amazing feature'`)
4. Push ke branch (`git push origin feature/amazing-feature`)
5. Buka Pull Request

---

## 📚 Referensi

- [Linux From Scratch (LFS)](https://www.linuxfromscratch.org/)
- [OSDev Wiki](https://wiki.osdev.org/)
- [Writing a Simple Operating System from Scratch](https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf)
- [The little book about OS development](https://littleosbook.github.io/)
- [OSDev.org Bare Metal Tutorials](https://wiki.osdev.org/Bare_Bones)

---

## ⚠️ Catatan

1. **Bare-metal OS** - Cocok untuk pembelajaran cara kerja OS
2. **Custom Linux Distro** - Membutuhkan Linux host dengan minimal 30GB storage dan 4GB RAM

---

## 📝 Lisensi

Projek ini dilisensikan di bawah MIT License - lihat file [LICENSE](LICENSE) untuk detail.

---

<p align="center">
  Dibuat dengan ❤️ untuk komunitas open-source
</p>
