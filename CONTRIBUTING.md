# 🤝 Contributing to Custom-OS

Terima kasih atas minat Anda untuk berkontribusi pada proyek Custom-OS! Dokument ini menyediakan panduan dan prosedur untuk berkontribusi.

## 📋 Daftar Isi

- [Panduan Umum](#panduan-umum)
- [Cara Berkontribusi](#cara-berkontribusi)
- [Struktur Proyek](#struktur-proyek)
- [Standar Koding](#standar-koding)
- [Pull Request](#pull-request)
- [Reporting Issues](#reporting-issues)

## 🎯 Panduan Umum

### Jenis Kontribusi

Kami menyambut berbagai jenis kontribusi:

- 🐛 **Bug Fixes** - Perbaikan bug yang ada
- ✨ **Fitur Baru** - Penambahan fitur baru
- 📚 **Dokumentasi** - Perbaikan atau penulisan dokumentasi
- 🎨 **Refactoring** - Perbaikan kode tanpa mengubah fungsionalitas
- ⚡ **Optimasi** - Perbaikan performa
- 🧪 **Tes** - Penambahan atau perbaikan test

### Langkah Sebelum Berkontribusi

1. **Fork** repository ini
2. **Clone** fork Anda ke komputer lokal
3. **Buat branch** baru untuk fitur atau perbaikan Anda
4. Ikuti standar koding proyek
5. **Test** perubahan Anda
6. **Commit** perubahan dengan pesan yang jelas
7. **Push** ke fork Anda
8. Buat **Pull Request**

## 🔀 Cara Berkontribusi

### 1. Clone Repository

```bash
# Clone fork Anda
git clone https://github.com/YOUR_USERNAME/custom-os.git
cd custom-os

# Tambahkan upstream sebagai remote
git remote add upstream https://github.com/antono4/custom-os.git
```

### 2. Buat Branch Baru

```bash
# Untuk fitur baru
git checkout -b feature/nama-fitur

# Untuk perbaikan bug
git checkout -b fix/deskripsi-bug

# Untuk dokumentasi
git checkout -b docs/perbaikan-dokumentasi
```

### 3. Buat Perubahan

Lakukan perubahan kode sesuai dengan standar koding proyek.

### 4. Commit Perubahan

```bash
# Add perubahan
git add .

# Commit dengan pesan yang jelas
git commit -m "feat: menambahkan fitur multitasking"
```

### 5. Push ke Fork Anda

```bash
git push origin feature/nama-fitur
```

### 6. Buat Pull Request

1. Buka repository asli di GitHub
2. Klik "New Pull Request"
3. Pilih branch Anda dari fork
4. Isi judul dan deskripsi
5. Klik "Create Pull Request"

## 📁 Struktur Proyek

```
custom-os/
├── baremetal-os/           # OS dari nol
│   ├── bootloader/        # Stage 1 & 2 bootloader
│   ├── kernel/             # Kernel source
│   │   └── devices/        # Device drivers
│   ├── libc/               # C library
│   ├── shell/              # Shell interaktif
│   ├── programs/           # Program contoh
│   └── Makefile
│
├── custom-linux-distro/     # Custom Linux Distribution
│   ├── scripts/            # Build scripts
│   ├── packages/           # Package list
│   ├── config/             # Configuration files
│   └── docs/               # Documentation
│
├── .github/
│   └── workflows/          # CI/CD pipelines
│
├── README.md
├── CONTRIBUTING.md
└── LICENSE
```

## 📝 Standar Koding

### C Coding Standards

```c
// Header dengan guard
#ifndef FEATURE_H
#define FEATURE_H

// Fungsi dengan dokumentasi
/**
 * @brief Fungsi untuk melakukan sesuatu
 * @param param1 Deskripsi parameter pertama
 * @return Status hasil operasi
 */
int do_something(int param1);

#endif

// Naming conventions
int calculate_sum(int a, int b);     // camelCase untuk fungsi
int g_total_count;                    // g_ prefix untuk global
#define MAX_BUFFER_SIZE 1024          // UPPER_CASE untuk macro
```

### Shell Script Standards

```bash
#!/bin/bash
# Script description
# Usage: ./script.sh [options]

set -e  # Exit on error
set -u  # Exit on undefined variable

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

# Constants
readonly MAX_RETRIES=3
readonly LOG_FILE="/var/log/app.log"

# Functions
log() {
    echo -e "${GREEN}[INFO]${NC} $1"
}
```

### Commit Messages

Gunakan format berikut:

```
<type>(<scope>): <description>

[optional body]

[optional footer]
```

Contoh:

```
feat(kernel): add multitasking support

- Implement round-robin scheduler
- Add task structure and task management
- Add context switching mechanism

Closes #123
```

**Types:**
- `feat` - Fitur baru
- `fix` - Perbaikan bug
- `docs` - Dokumentasi
- `style` - Format kode
- `refactor` - Refactoring
- `test` - Penambahan tes
- `chore` - Tugas maintenance

### Git Branch Naming

```
feature/add-multitasking
fix/keyboard-driver-crash
docs/update-readme
refactor/improve-memory-management
hotfix/urgent-security-patch
```

## 🔍 Pull Request Guidelines

### Checklist PR

- [ ] Branch di-update dengan perubahan terbaru dari main
- [ ] Kode mengikuti standar koding proyek
- [ ] Perubahan sudah di-test
- [ ] Dokumentasi di-update jika perlu
- [ ] Commit messages jelas dan mengikuti format

### Proses Review

1. Maintainer akan me-review PR Anda
2. Mungkin ada beberapa feedback atau perubahan yang diminta
3. Responds terhadap feedback dengan commit baru
4. Setelah approved, PR akan di-merge

## 🐛 Reporting Issues

### Bug Report Template

```markdown
**Deskripsi Bug**
Penjelasan jelas tentang bug.

**Langkah Reproduksi**
1. Buka aplikasi
2. Klik pada X
3. Scroll ke Y
4. Lihat error

**Expected Behavior**
Apa yang seharusnya terjadi.

**Actual Behavior**
Apa yang sebenarnya terjadi.

**Screenshots**
Tambahkan screenshot jika applicable.

**Environment**
- OS: [e.g., Ubuntu 22.04]
- Version: [e.g., 1.0.0]
```

### Feature Request Template

```markdown
**Fitur yang Diminta**
Deskripsi jelas fitur yang diinginkan.

**Alasan**
 Mengapa fitur ini diperlukan.

**Alternative Solutions**
Solusi alternatif yang sudah dipertimbangkan.

**Additional Context**
Konteks tambahan atau screenshot.
```

## 📜 License

Dengan berkontribusi, Anda setuju bahwa kontribusi Anda akan dilisensikan di bawah MIT License.

## 🙏 Terima Kasih

Setiap kontribusi sangat dihargai! Terima kasih atas waktu dan tenaga Anda.

---

Dibuat dengan ❤️ untuk komunitas open-source