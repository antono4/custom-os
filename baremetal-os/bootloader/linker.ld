/* =============================================================================
 * NanoOS Linker Script
 * =============================================================================
 * Defines memory layout for the kernel
 */

OUTPUT_FORMAT("elf32-i386")
ENTRY(stage2_start)

SECTIONS
{
    . = 0x10000;
    
    .text : {
        *(.text)
    }
    
    .rodata : {
        *(.rodata)
    }
    
    .data : {
        *(.data)
    }
    
    .bss : {
        *(.bss)
    }
    
    /DISCARD/ : {
        *(.comment)
        *(.eh_frame)
    }
}
