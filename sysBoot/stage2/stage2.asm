org 0x500
bits 16

jmp main

%include "sysBoot/stage2/stdio.h"
%include "sysBoot/stage2/gdt.h"
%include "sysBoot/stage2/A20.h"

loadingMsg db 0xa, "Searching for Operating System...", 0x0

main:
	cli
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ax, 0x9000
	mov ss, ax
	mov sp, 0xffff	;stack begins at 0x9000:0xffff
	sti
	
	mov si, loadingMsg
	call print16

	call installGDT
	call enableA20_kbrd
	
	cli		;entering p-mode
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	jmp CODE_DESC:stage3	;far jump to load CS

bits 32
stage3:
	mov ax, DATA_DESC
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov esp, 0x90000
; clear screen
	call clrScr32
	mov ebx, msg
	call print32
; stop execution
	cli
	hlt
msg db 0xA, 0xA, 0xA, "          <[ OS Development Series Tutorial 10 ]>"
    db 0xA, 0xA,         "        Basic 32 bit graphics demo in Assembly Language", 0
