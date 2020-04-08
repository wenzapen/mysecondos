org 0x7c00
bits 16

starts:
    jmp loader

msg db  "Welcome to My Operating System!",0

print:
    lodsb
    or al, al
    jz printDone
    mov ah, 0xe
    int 0x10
    jmp print

printDone:
    ret

loader:
    mov ax, 0xcafe
    xor ax, ax
    mov ds, ax
    mov es, ax
    
    mov si, msg
    call print

    mov ax, 0xcafe
    cli
    hlt
times 510 - ($-$$) db 0
dw 0xaa55

