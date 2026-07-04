; =============================================================================
; NanoOS Bootloader - Stage 1 (MBR)
; =============================================================================
; Stage 1 bootloader: Loads stage 2 from disk and jumps to it
; Uses BIOS interrupts for disk access
; Syntax: NASM

[BITS 16]
[ORG 0x7C00]

_start:
    ; Save drive number
    mov [drive_num], dl
    
    ; Set up segments
    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov es, ax
    
    ; Set stack
    mov sp, 0x7C00
    
    ; Clear screen
    mov ah, 0x03
    int 0x10
    
    ; Print loading message
    mov si, msg_loading
    call print_string
    
    ; Load stage 2 (15 sectors = 7680 bytes)
    mov ax, 0x1000
    mov es, ax
    xor bx, bx
    
    mov ah, 0x02          ; Function: read sectors
    mov al, 0x0E          ; Sector count
    mov ch, 0x00          ; Cylinder 0
    mov cl, 0x02          ; Start from sector 2
    mov dl, [drive_num]   ; Drive number
    mov dh, 0x00          ; Head 0
    int 0x13
    jc disk_error
    
    ; Jump to stage 2
    jmp 0x0000:0x1000

disk_error:
    mov si, msg_error
    call print_string
    jmp hang

print_string:
    lodsb
    or al, al
    jz done
    mov ah, 0x0E
    int 0x10
    jmp print_string
done:
    ret

hang:
    jmp hang

drive_num: db 0

msg_loading: db "NanoOS: Loading...", 13, 10, 0
msg_error: db "Disk read error!", 13, 10, 0

; Padding and boot signature
times 510-($-$$) db 0
dw 0xAA55
