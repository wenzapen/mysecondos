org 0x500
bits 16

jmp main

%include "sysBoot/stage2/stdio.h"
%include "sysBoot/stage2/gdt.h"
%include "sysBoot/stage2/A20.h"
%include "sysBoot/stage2/common.h"
%include "sysBoot/stage2/fat12.h"
%include "sysBoot/stage2/bootinfo.h"
%include "sysBoot/stage2/memory.h"

loadingMsg db 0xa, "Searching for Operating System...", 0x0
msgFailure db 0xa, "*** FATAL: MISSING OR CURRUPT KERNEL.BIN", 0x0
loadingKernel db 0xa, "Loading Kernel...",0x0

boot_info:
istruc multiboot_info
	at multiboot_info.flags,			dd 0
	at multiboot_info.memoryLo,			dd 0
	at multiboot_info.memoryHi,			dd 0
	at multiboot_info.bootDevice,		dd 0
	at multiboot_info.cmdLine,			dd 0
	at multiboot_info.mods_count,		dd 0
	at multiboot_info.mods_addr,		dd 0
	at multiboot_info.syms0,			dd 0
	at multiboot_info.syms1,			dd 0
	at multiboot_info.syms2,			dd 0
	at multiboot_info.mmap_length,		dd 0
	at multiboot_info.mmap_addr,		dd 0
	at multiboot_info.drives_length,	dd 0
	at multiboot_info.drives_addr,		dd 0
	at multiboot_info.config_table,		dd 0
	at multiboot_info.bootloader_name,	dd 0
	at multiboot_info.apm_table,		dd 0
	at multiboot_info.vbe_control_info,	dd 0
	at multiboot_info.vbe_mode_info,	dw 0
	at multiboot_info.vbe_interface_seg,dw 0
	at multiboot_info.vbe_interface_off,dw 0
	at multiboot_info.vbe_interface_len,dw 0
iend


; elf header
e_entry: dd 0
e_phoff: dd 0  ; program header offset: the 1st ph
e_phentsize: dw 0  ; size of program header
e_phnum: dw 0
ph_base: dd 0  ; base address of programe header: address of 1st ph

; program header
p_offset: dd 0  ; ph+4
p_paddr: dd 0 ; ph+12
p_filesz: dd 0 ; ph+16
p_memsz: dd 0 ; ph+20
p_segment_base: dd 0

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
	mov ax, 0x0
	mov ss, ax
	mov sp, 0xffff	;stack begins at 0x9000:0xffff
	sti
	
	mov si, loadingMsg
	call print16

	mov [boot_info+multiboot_info.bootDevice], dl

	call installGDT
	call _enableA20
;	call enableA20_kbrd

	xor eax, eax
	xor ebx, ebx
	call biosGetMemorySize64MB
	mov word [boot_info+multiboot_info.memoryHi],bx
	mov word [boot_info+multiboot_info.memoryLo], ax

	mov eax, 0x0
	mov ds, ax
	mov di, 0x1000
	call biosGetMemoryMap


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

%include "sysBoot/stage2/paging.h"

stage3:
	mov ax, DATA_DESC
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov esp, 0x90000
	mov ebx, loadingKernel
	call print32

	call enablePaging

; copy kernel to IMAGE_PMODE_BASE(0x100000)
; 

loadKernel:
	mov eax, IMAGE_RMODE_BASE
	add eax, 24
	mov ebx, [eax]
	mov dword [e_entry], ebx 
	add eax, 4
	mov ebx, [eax]
	mov dword [e_phoff], ebx 
	add ebx, IMAGE_RMODE_BASE
	mov dword [ph_base], ebx
	add eax, 14
	movzx ebx, word [eax]
	mov word [e_phentsize], bx 
	add eax, 2
	movzx ebx,word [eax]
	mov word [e_phnum], bx 
	mov ecx, ebx  ; put number of program headers in ecx

.copy_segment:
	push ecx
	mov eax, ecx
	sub eax, 1
	mul word [e_phentsize]
	add eax, [ph_base]
	add eax, 4
	mov ebx, [eax]
	mov dword [p_offset], ebx
	add ebx, IMAGE_RMODE_BASE
	mov dword [p_segment_base], ebx
	mov esi, ebx   ; source of segment
	add eax, 8
	mov ebx, [eax]
	mov dword [p_paddr], ebx
	mov edi, ebx ; destination of segment
	add eax, 4
	mov ebx, [eax]
	mov dword [p_filesz], ebx
	mov ecx, ebx
	add eax, 4
	mov ebx, [eax]
	mov dword [p_memsz], ebx
	rep movsb
	mov eax, dword [p_memsz]
	sub eax, dword [p_filesz]
	jz .no_padding
	mov ecx, eax
	mov al, 0
	rep stosb

.no_padding:
	pop ecx
	loop .copy_segment
.execute:
	mov eax, dword [e_entry]
	mov ebp, eax
	mov eax, 0x2badb002 ; multiboot specs say eax should be this
	mov ebx, 0
	mov edx, [imageSize]
	push dword boot_info
	call ebp
	add esp, 4
	cli
	hlt	

; Function copyImage is not used anymore
;copyImage:
;	mov eax, dword [imageSize]
;	movzx ebx, word [bpbBytesPerSector]
;	mul ebx
;	mov ebx, 4
;	div ebx
;	cld
;	mov ecx, eax
;	mov esi, IMAGE_RMODE_BASE
;	mov edi, IMAGE_PMODE_BASE
;	rep movsd
;	mov ebx, loadingKernel
;	call print32
;	hlt
;
;	jmp CODE_DESC:IMAGE_PMODE_BASE ; jump to kernel

