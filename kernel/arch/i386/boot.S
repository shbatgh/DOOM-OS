# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The kernel entry point.
.section .text
.global _start
.type _start, @function
_start:
	movl $stack_top, %esp

	# Call the function to set up the GDT
    call setup_gdt

	# Call the global constructors.
	call _init

	# Transfer control to the main kernel.
	call kernel_main

	# Hang if kernel_main unexpectedly returns.
	cli
1:	hlt
	jmp 1b
.size _start, . - _start

# Function to set up the GDT
setup_gdt:
    lgdt gdtr
    mov $0x10, %ax      # Data segment selector
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    ljmp $0x08, $.flush # Code segment selector

.flush:
    ret

# GDT descriptor
.section .data
.align 8
gdt:
    .quad 0x0000000000000000  # Null segment
    .quad 0x00CF9A000000FFFF  # Code segment
    .quad 0x00CF92000000FFFF  # Data segment
    .quad 0x00CFFA000000FFFF  # User mode code segment
    .quad 0x00CFF2000000FFFF  # User mode data segment

gdtr:
    .word gdt_end - gdt - 1   # Limit
    .long gdt                 # Base

gdt_end:
