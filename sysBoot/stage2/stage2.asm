org 0x500
bits 16

jmp main

%include "sysBoot/stage2/stdio.h"
%include "sysBoot/stage2/gdt.h"
%include "sysBoot/stage2/A20.h"

loadingMsg db 0xa, "Searching for Operating System...", 0x0
msgFailure db 0xa, "*** FATAL: MISSING OR CURRUPT KERNEL.BIN", 0x0

;************************************************************
;
;	Stage 2 entry point
;
;		-Store BIOS information
;		-Load Kernel
;		-Install GDT, go into protected mode
;		-Jump to stage 3
;
;************************************************************
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

	call loadRoot  ; load root directory table

	;***************************************
	; load kernel
	;***************************************

	mov ebx, 0
	mov bp, IMAGE_RMODE_BASE
	mov si, imageName
	call loadFile
	mov dword [imageSize], ecx
	cmp ax, 0
	je enterStage3
	mov si, msgFailure
	call print16
	mov ah, 0
	int 0x16
	int 0x19
	cli
	hlt

enterStage3:
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

copyImage:
	mov eax, dword [imageSize]
	movzx ebx, word [bpbBytePerSector]
	mul ebx
	mov ebx, 4
	div ebx
	cld
	mov ecx, eax
	mov esi, IMAGE_RMODE_BASE
	mov edi, IMAGE_PMODE_BASE
	rep movsd

	jmp CODE_DESC:IMAGE_PMODE_BASE ; jump to kernel

; stop execution
	cli
	hlt
