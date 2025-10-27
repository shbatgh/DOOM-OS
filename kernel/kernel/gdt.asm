section .data
    gdtr dw 0
    dd 0

section .text
global setGdt

setGdt:
    mov ax, [esp + 4]
    mov [gdtr], ax
    mov eax, [esp + 8]
    mov [gdtr + 2], eax
    lgdt [gdtr]
    ret
