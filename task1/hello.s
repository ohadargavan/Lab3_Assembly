global _start       ;Defining the entarance point so it's visible to Linker

section .rodata     ;memory area for reading-only data (like constant strings)
    hello_str: db "hello bgu", 10  ; db: alocate bytes for the string. 10 = "\n"
    dummy_str:                       ; empty string for calculating length

section .text       ; memory area for code (proccesor instructions)

_start:
    ; --- sys_write ---
    mov edx, dummy_str - hello_str   ; Calculte string length by substructing adresses
    mov ecx, hello_str               ;pointer to the beginning of the string
    mov ebx, 1                       ; File Descriptor 1 = stdout
    mov eax, 4                       ;4 = sys_write
    int 0x80                         ;calling the system call via the kernel

    ; --- sys_exit ---
    mov ebx, 0                       ; exit code 0 (everything is fine)
    mov eax, 1                       ; 1= sys_exit
    int 0x80                         ; calling the system call