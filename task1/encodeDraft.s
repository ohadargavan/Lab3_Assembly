section .data
    Infile dd 0 ; std in
    Outfile dd 1 ; std out
    EncoderString db "+VA", 0
    CurrentEncodeP dd EncoderString+2

section .text

encode:
    mov ecx, [CurrentEncodeP]
    mov dl, byte[ecx]
    sub dl, 'A' ; dl stores the "shift"
    cmp al, 'a' ; al stores the current char
    jl skip_encode
    cmp al, 'z'
    ret
    ;---encoding---
    add al, dl
    cmp al, 'z'
    jle update_ptr
    sub al, 26
    
update_ptr:
    inc dword [CurrentEncodeP]
    mov ecx, [CurrentEncodeP]
    mov dl, byte[ecx] ; get the actual letter
    cmp dl, 0 ; check if we reached end of encoding key
    jz update_ecoding_key
    ret
    

update_ecoding_key:
    mov dword [CurrentEncodeP], EncoderString
    ret

    