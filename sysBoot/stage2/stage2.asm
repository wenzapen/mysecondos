org 0x0
bits 16

jmp main

print:
    lodsb
    or al, al
    jz printDone
    mov ah, 0xe
    int 0x10
    jmp print

printDone:
    ret

main:
    cli
    push cs
    pop ds
    mov si, msg
    call print
    cli
    hlt

msg db "Preparing to load operating system...",13,10,0
