org 0x500
bits 16

jmp main

%include "sysBoot/stage2/stdio.h"
%include "sysBoot/stage2/gdt.h"

loadingMsg db 0xd, 0xa, "Searching for Operating System...", 0x0

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
	cli
	hlt

