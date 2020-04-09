%ifndef STDIO_H
%define STDIO_H

;*************************************************
;	print16()
;		-prints a null terminated string
;	DS:SI 0 terminated string
;*************************************************

bits 16

print16:
	pusha
.loop:
	lodsb
	or al, al
	jz print16Done
	mov ah, 0xe
	int 0x10
	jmp .loop
print16Done:
	popa
	ret

%endif
