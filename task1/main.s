global main
extern strlen        ;declaring helper function from util.c

section .rodata
    newline: db 10   ; 10 = "\n"

section .data
    Infile dd 0 ; std in
    Outfile dd 1 ; std out
    EncoderString db "+VBCDEF", 0
    CurrentEncodeP dd EncoderString+2
    KeyStart dd EncoderString+2 ; for user input encoding key
    char_buffer db 0

section .text
main:
    ; --- initiatilizing Stack Frame ---
    push    ebp
    mov     ebp, esp

    ; --- retrieve arguments from stack---
    mov     esi, [ebp+8]   ; esi = argc (the arguments counter)
    mov     edi, [ebp+12]  ; edi = argv (pointer to the string array)

print_loop:
    cmp     esi, 0         ; cmp is like "if", by substructing. Are there arguments left to print?
    jz      read_loop       ;if argc == 0, jump to end_prog

    ; --- 1.Calling strlen to get string length---
    mov     eax, [edi]     ; eax = pointer to current string (argv[i])

    ; ====================================================================
    ; --- check if argument is a key ---
    cmp     word [eax], '+'+(256*'V')  ; does it start with v+?
    jnz     check_infile               ; if not, check if it is infile
    ; if we reached here, the argument is an encoding key
    mov     ebx, eax                    ; take the argument's adresses
    add     ebx, 2                      ; skip the +V chars
    mov     dword [CurrentEncodeP], ebx ; update the encoding key to be the new one
    mov     dword [KeyStart], ebx       ; 

check_infile:
    ; --- check -i flag ---
    cmp     word [eax], '-'+(256*'i')
    jnz     check_outfile              ; if not -i, check -o

    ; --- (sys_open) ---
    mov     ebx, eax
    add     ebx, 2          ; skip -i
    mov     eax, 5          ; 5 = open
    mov     ecx, 0          ; 0 = read only
    mov     edx, 0777q      ; file premissions
    int     0x80
    mov     dword [Infile], eax ; update the program's Infile
    jmp     continue_print

check_outfile:
    ; ---check -o flag ---
    cmp     word [eax], '-'+(256*'o')
    jnz     continue_print             ;

    ; --- (sys_open) ---
    mov     ebx, eax
    add     ebx, 2          ; skip '-o'
    mov     eax, 5          ; 5= open
    mov     ecx, 65         ;1 (writing) + 64 (create new file if doesnt exist)
    mov     edx, 0777q      ; file premissions
    int     0x80
    mov     dword [Outfile], eax ; update outfile variable
continue_print:
    ; ====================================================================
    mov     eax, [edi] ;revert value in eax to be the string adress
    push    eax            ; push the argument to stack for strlen function
    call    strlen         ; after strlen is done, the result will be stored in eax
    add     esp, 4         ; clean the stack from the argument we pushed

    ; --- 2. system call (sys_write) to print the argument ---
    mov     edx, eax       ; edx = string length (returned from strlen to eax)
    mov     ecx, [edi]     ; ecx = adress of the string (argv[i])
    mov     ebx, 1         ; ebx = printing target (1 = stdout)
    mov     eax, 4         ; 4 for write system call
    int     0x80           ; calling the system call

    ; --- 3. system call to print new line---
    mov     edx, 1         ; length is 1
    mov     ecx, newline   ; adress of the newline symbol
    mov     ebx, 1         ; stdout
    mov     eax, 4         ; sys_write
    int     0x80

    ; --- 4. advance to next string in loop---
    add     edi, 4         ;advancing the pointer argv in 4 bytes (to point next argument)
    dec     esi            ; decrement the arguments counter(argc--)
    jmp     print_loop     ; go back to beginning of the loop

read_loop:
    mov eax, 3             ; 3 = sys_read symbol
    mov ebx, [Infile]      ; Infile is just "0", for stdin
    mov ecx, char_buffer   ; char_buffer is the adress where the char is stored
    mov edx, 1             ; length of reading: 1 
    int 0x80               ; call the system call 
    cmp eax, 0             ; have we reached end of file?
    jle end_prog           ; if so- end
    mov al, byte [char_buffer] ;put the char in 'al' for encode function
    call encode                 ;after encode is finished, the encoded char is in 'al'

    ;---printing the encoded char---
    ; 1. save the encoded char back to actual memory
    mov byte [char_buffer], al 

    ; 2. preparing for sys_write
    mov eax, 4             ; 4 for write
    mov ebx, [Outfile]     ; Outfile is 1 => stdout
    mov ecx, char_buffer   ; where to read the char from
    mov edx, 1             ; length of print
    int 0x80               

    
    jmp read_loop          ; jump back to the beginning of the loop to read next char


end_prog:
    ; --- 5. exit the program (sys_exit) ---
    mov     ebx, 0         ; exit code (0= success)
    mov     eax, 1         ; 1 is the code for exit system call
    int     0x80           ; execute the system call


;---for 1b---
encode:
    mov ecx, [CurrentEncodeP]
    mov dl, byte[ecx]
    sub dl, 'A' ; dl stores the "shift"
    cmp al, 'a' ; al stores the current char
    jb skip_encode ; instead of jl because of sign
    cmp al, 'z'
    ja skip_encode ; instea of jg
    
    ;---encoding---
    add al, dl
    cmp al, 'z'
    jbe update_ptr
    sub al, 26
    
update_ptr:
    inc dword [CurrentEncodeP]
    mov ecx, [CurrentEncodeP]
    mov dl, byte[ecx] ; get the actual letter
    cmp dl, 0 ; check if we reached end of encoding key
    jz update_ecoding_key
    ret
    

update_ecoding_key: ;reset the pointer back to the beginning of the key
   mov ecx, [KeyStart] 
    mov dword [CurrentEncodeP], ecx
    ret

skip_encode:
   ret