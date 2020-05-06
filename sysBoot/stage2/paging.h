%ifndef _PAGINT_H
%define _PAGING_H

bits 32

%define PAGE_DIR  0x9c000
%define PAGE_TABLE_0  0x9d000 ; 0th page table
%define PAGE_TABLE_768  0x9e000 ; 768th page table
%define PAGE_TABLE_ENTRIES 1024
%define PRIV 3

enablePaging:
	pusha
; identity map 0th page table(0~4Mb)
	mov eax, PAGE_TABLE_0
	mov ebx, 0x0 | PRIV
	mov ecx, PAGE_TABLE_ENTRIES
.loop:
	mov dword [eax], ebx
	add eax, 4
	add ebx, 4096
	loop .loop
; map 768th page table(1Mb to 3gb; 768th page table starting for virtual address; 0xc0000000)
	mov eax, PAGE_TABLE_768
	mov ebx, 0x100000 | PRIV
	mov ecx, PAGE_TABLE_ENTRIES
.loop2:
	mov dword [eax], ebx
	add eax, 4
	add ebx, 4096
	loop .loop2
; set up the entries in the directory table
	mov eax, PAGE_TABLE_0 | PRIV
	mov dword [PAGE_DIR], eax
	mov eax, PAGE_TABLE_768 | PRIV
	mov dword [PAGE_DIR+768*4], eax




%endif
