; =============================================================================
; NanoOS Bootloader - Stage 2
; =============================================================================
; Stage 2 bootloader: Switches to protected mode and loads kernel
; Uses GRUB multiboot specification
; Syntax: NASM (ELF format for linking)

extern kernel_main

section .text

stage2_start:
    ; Set up GDT
    cli
    lgdt [gdt_descriptor]
    
    ; Enable A20 line
    in al, 0x92
    or al, 2
    out 0x92, al
    
    ; Switch to protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    ; Far jump to set CS
    jmp 0x08:protected_start

; GDT
gdt_start:
    dq 0x0000000000000000
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x9A
    db 0xCF
    db 0x00
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x92
    db 0xCF
    db 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

protected_start:
    ; Set up segment registers
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Set up stack
    mov esp, 0x90000
    
    ; Point to video memory
    mov ax, 0xB800
    mov gs, ax
    
    ; Print message
    mov esi, msg_loaded
    call print_pm
    
    ; Jump to kernel
    jmp kernel_main

print_pm:
    lodsb
    test al, al
    jz done_pm
    ; VGA text mode
    xor ebx, ebx
    mov [gs:ebx], al
    inc ebx
    mov byte [gs:ebx], 0x07
    inc ebx
    jmp print_pm
done_pm:
    ret

msg_loaded: db "NanoOS Stage 2: Protected Mode Enabled", 0

times 8192-($-$$) db 0
