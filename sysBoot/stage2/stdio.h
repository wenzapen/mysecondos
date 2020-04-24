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
.loop_p16:
	lodsb
	or al, al
	jz print16Done
	mov ah, 0xe
	int 0x10
	jmp .loop_p16
print16Done:
	popa
	ret


;****************************************************
;	printch32()
;		-prints a character to sceen
;	bl => character to print
;****************************************************

bits 32

%define VIDMEM 0xB8000 ; video memory
%define COLS 80 ; width and height of screen
%define LINES 25
%define CHAR_ATTRIB 15

_curX db 0
_curY db 0

printch32:
	pusha
	mov edi, VIDMEM
	xor eax, eax ; clear eax

	mov ecx, COLS*2
	mov al, byte [_curY]
	mul ecx
	push eax ; eax now equals to 2*COLS*_curY

	mov al, byte [_curX]
	mov cl, 2
	mul cl
	pop ecx
	add eax, ecx  ; eax now equals to 2*COLS*_curY+2*_curX

	add edi, eax

	cmp bl, 0x0A ; to check if it's a '\n' character
	je .row

	mov dl, bl
	mov dh, CHAR_ATTRIB
	mov word [edi], dx ; write a char to video display

	inc byte [_curX] ;go to next character
	cmp byte [_curX], COLS
	je .row
	jmp .done_pch

.row:
	mov byte [_curX], 0
	inc byte [_curY]

.done_pch:
	popa
	ret


;*****************************************************
;	print32()
;		- prints a null terminated string
;	parameters: ebx = address of string to print
;*****************************************************

bits 32

print32:
	pusha
	push ebx
	pop edi
.loop_p32:
	mov bl, byte [edi] ; put the next character to bl
	cmp bl, 0
	je .done_p32

	call printch32
	inc edi
	jmp .loop_p32
.done_p32:
	mov bh, byte [_curY]
	mov bl, byte [_curX]
	call moveCur
	popa
	ret



;*****************************************************
;	moveCur()
;		- update hardware cursor 
;	parameters: bl=X position; bh=Y position
;*****************************************************

bits 32
moveCur:
	pusha
	xor eax, eax
	mov ecx, COLS ;
	mov al, bh ; set al=_curY
	mul ecx ; al=COLS * curY
	add al, bl
	mov ebx, eax ; ebx=x+COLS*y

; set low byte index to VGA register
	mov al, 0xf
	mov dx, 0x3D4
	out dx, al
	mov al, bl
	mov dx, 0x3D5
	out dx, al

; set high byte index to VGA register
	xor eax, eax
	mov al, 0xe
	mov dx, 0x3D4
	out dx, al
	mov al, bh
	mov dx, 0x3D5
	out dx, al

	popa
	ret

;*****************************************************
;	gotoXY()
;		- set current X/Y position
;	parameters: al=X position; ah=Y position
;*****************************************************

bits 32
gotoXY:
	pusha
	mov [_curX], al
	mov [_curY], ah
	popa
	ret

;*****************************************************
;	clrScr32()
;		- Clears screen
;*****************************************************

bits 32
clrScr32:
	pusha
	cld
	mov edi, VIDMEM
	mov ecx, 2000
	mov ah, CHAR_ATTRIB
	mov al, ' '
	rep stosw
	mov byte [_curX], 0
	mov byte [_curY], 0
	popa
	ret

%endif
