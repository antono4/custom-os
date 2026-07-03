# =============================================================================
# NanoOS Build System
# =============================================================================

# Toolchain
CC = gcc
AS = nasm
LD = ld
OBJCOPY = objcopy

# Flags
CFLAGS = -m32 -nostdinc -fno-pie -ffreestanding -Wall -Wextra -Werror
ASFLAGS16 = -f bin
LDFLAGS = -T bootloader/linker.ld -nostdlib -Map=kernel.map

# Directories
BOOT_DIR = bootloader
KERNEL_DIR = kernel
LIB_DIR = libc
SHELL_DIR = shell
ISO_DIR = iso
GRUB_DIR = $(ISO_DIR)/boot/grub

# Output
ISO = NanoOS.iso
KERNEL_BIN = kernel.bin
OSIMG = nanoos.img

# Source files
BOOT_SOURCES = $(BOOT_DIR)/stage1.S $(BOOT_DIR)/stage2.S
KERNEL_SOURCES = $(KERNEL_DIR)/main.c $(KERNEL_DIR)/gdt.c $(KERNEL_DIR)/idt.c \
                 $(KERNEL_DIR)/memory.c $(KERNEL_DIR)/vfs.c \
                 $(KERNEL_DIR)/devices/keyboard.c $(KERNEL_DIR)/devices/timer.c
LIB_SOURCES = $(LIB_DIR)/stdio.c $(LIB_DIR)/string.c
SHELL_SOURCES = $(SHELL_DIR)/shell.c

# Objects
BOOT_OBJECTS = $(BOOT_SOURCES:.S=.bin)
KERNEL_OBJECTS = $(KERNEL_SOURCES:.c=.o)
LIB_OBJECTS = $(LIB_SOURCES:.c=.o)
SHELL_OBJECTS = $(SHELL_SOURCES:.c=.o)
ALL_OBJECTS = $(KERNEL_OBJECTS) $(LIB_OBJECTS) $(SHELL_OBJECTS)

.PHONY: all clean run iso img kernel

all: iso

# Build bootloader (binary format for stage1)
$(BOOT_DIR)/stage1.bin: $(BOOT_DIR)/stage1.S
	@echo "  AS      $<"
	$(AS) $(ASFLAGS16) $< -o $@

# Build stage2 bootloader (ELF format to link with kernel)
$(BOOT_DIR)/stage2.o: $(BOOT_DIR)/stage2.S
	@echo "  AS      $<"
	$(AS) -f elf32 $< -o $@

$(BOOT_DIR)/idt_flush.o: $(BOOT_DIR)/idt_flush.S
	@echo "  AS      $<"
	$(AS) -f elf32 $< -o $@

# Build kernel
$(KERNEL_DIR)/%.o: $(KERNEL_DIR)/%.c $(KERNEL_DIR)/kernel.h
	@echo "  CC      $<"
	$(CC) $(CFLAGS) -I. -I$(KERNEL_DIR) -c $< -o $@

# Build libc (use system headers)
$(LIB_DIR)/%.o: $(LIB_DIR)/%.c
	@echo "  CC      $<"
	$(CC) -m32 -nostdlib -fno-pie -Wall -I$(KERNEL_DIR) -c $< -o $@

# Build shell
$(SHELL_DIR)/%.o: $(SHELL_DIR)/%.c
	@echo "  CC      $<"
	$(CC) $(CFLAGS) -I. -I$(KERNEL_DIR) -I$(LIB_DIR) -c $< -o $@

# Link everything (include stage2 bootloader)
kernel.elf: $(BOOT_DIR)/stage2.o $(BOOT_DIR)/idt_flush.o $(ALL_OBJECTS)
	@echo "  LD      $@"
	$(LD) -m elf_i386 $(LDFLAGS) $^ -o $@

# Create flat binary
$(KERNEL_BIN): kernel.elf
	@echo "  OBJCOPY $@"
	$(OBJCOPY) -O binary -R .note -R .comment -S $< $@

# Create ISO directory structure
$(GRUB_DIR):
	@mkdir -p $(GRUB_DIR)

# Create GRUB configuration
$(GRUB_DIR)/grub.cfg: $(GRUB_DIR)
	@echo "  GEN     $@"
	@printf '%s\n' '# NanoOS GRUB Configuration' 'set timeout=5' 'set default=0' '' 'menuentry "NanoOS v0.1" {' '    echo "Loading NanoOS..."' '    multiboot /boot/kernel.bin' '    boot' '}' '' 'menuentry "NanoOS v0.1 (Debug Mode)" {' '    echo "Loading NanoOS (Debug)..."' '    multiboot /boot/kernel.bin debug' '    boot' '}' > $@

# Copy kernel to ISO directory
$(ISO_DIR)/boot/kernel.bin: $(KERNEL_BIN) | $(GRUB_DIR)
	@cp $< $@

# Create GRUB PC image
$(ISO_DIR)/boot/grub/i386-pc: $(GRUB_DIR)
	@mkdir -p $@
	@grub-mkimage -O i386-pc -o $@/core.img normal part_msdos boot linux multiboot 2>/dev/null || true
	@echo "  GRUB    core.img created"

# Build ISO
iso: $(KERNEL_BIN) $(GRUB_DIR)/grub.cfg $(ISO_DIR)/boot/kernel.bin
	@echo "  ISO     $(ISO)"
	@sudo grub-mkrescue -o $(ISO) $(ISO_DIR) 2>/dev/null || \
	grub-mkrescue -o $(ISO) $(ISO_DIR)

# Create floppy disk image
$(OSIMG): $(BOOT_DIR)/stage1.bin $(KERNEL_BIN)
	@echo "  IMG     $@"
	@dd if=/dev/zero of=$@ bs=1024 count=1440 2>/dev/null
	@dd if=$(BOOT_DIR)/stage1.bin of=$@ conv=notrunc 2>/dev/null
	@dd if=$(KERNEL_BIN) of=$@ seek=1 bs=512 conv=notrunc 2>/dev/null

# Run in QEMU
run: iso
	@echo "Starting NanoOS in QEMU..."
	@qemu-system-i386 -cdrom $(ISO) -m 64 -display curses 2>/dev/null || \
	qemu-system-i386 -cdrom $(ISO) -m 64

# Run with GUI
run-gui: iso
	@qemu-system-i386 -cdrom $(ISO) -m 64

# Run floppy image
run-floppy: $(OSIMG)
	@qemu-system-i386 -fda $(OSIMG) -m 64

# Clean
clean:
	rm -f $(KERNEL_OBJECTS) $(LIB_OBJECTS) $(SHELL_OBJECTS) $(BOOT_DIR)/*.bin kernel.elf $(KERNEL_BIN) kernel.map
	rm -f $(ISO) $(OSIMG)
	rm -rf $(ISO_DIR)/boot

.PRECIOUS: $(KERNEL_OBJECTS) $(LIB_OBJECTS) $(SHELL_OBJECTS)
