%ifndef _MEMORY_H
%define _MEMORY_H

bits 16

struc memoryMapEntry
	.baseAddress resq 1
	.length resq 1
	.type resd 1
	.acpi_null resd 1
endstruc

;**************************************************
; get memory map from bios
; /in es:di->destination buffer for entries
; /ret bp=entry count
;**************************************************

biosGetMemoryMap:
	pushad
	xor ebx, ebx
	xor bp, bp ;number of entries store here
	mov edx, 'PAMS' ; SMAP
	mov eax, 0xe820
	mov ecx, 24 ; memory map entry struct is 24 byte
	int 0x15 ; get the first entry
	jc .error
	cmp eax, 'PAMS' ;bios returns 'PMAS' in eax
	jne .error
	test ebx, ebx   ; if ebx=0, then list is one entry long
	je .error
	jmp .start
.next_entry:
	mov edx, 'PAMS'
	mov ecx, 24
	mov eax, 0xe820
	int 0x15
.start:
	jcxz .skip_entry ; if actual returned byte is 0, skip the entry
.notext:
	mov ecx, [es:di+memoryMapEntry.length] ; get length(low dword)
	test ecx, ecx ; if length is 0 skip
	jne short .good_entry
	mov ecx, [es:di+memoryMapEntry.length+4] ; get length(upper dword)
	jecxz .skip_entry
.good_entry:
	inc bp
	add di, 24
.skip_entry:
	cmp ebx, 0
	jne .next_entry
	jmp .done
.error:
	stc
.done:
	popad
	ret

;*******************************************************
; get memory size for >64M configuration (32 bit)
; ret\ ax=KB btween 1MB and 16MB
; ret\ bx=number of 64K blocks above 16MB
; ret\ bx=0 and ax=-1 on error
;********************************************************
biosGetMemorySize64MB_32Bit:
	push ecx
	push edx
	xor ecx, ecx
	xor edx, edx
	mov eax, 0xe881
	int 0x15
	jc .error
	cmp ah, 0x86 ;unsupported function
	je .error
	cmp ah, 0x80
	je .error
	jcxz .use_ax ; bios may have stored it in ax,bx or cx,dx
	mov ax, cx
	mov bx, dx
.use_ax:
	pop edx
	pop ecx
	ret
.error:
	mov ax, -1
	mov bx, 0
	pop edx
	pop ecx
	ret
	
biosGetMemorySize64MB:
	push ecx
	push edx
	xor ecx, ecx
	xor edx, edx
	mov ax, 0xe801
	int 0x15
	jc .error
	cmp ah, 0x86 ;unsupported function
	je .error
	cmp ah, 0x80
	je .error
	jcxz .use_ax ; bios may have stored it in ax,bx or cx,dx
	mov ax, cx
	mov bx, dx
.use_ax:
	pop edx
	pop ecx
	ret
.error:
	mov ax, -1
	mov bx, 0
	pop edx
	pop ecx
	ret
	
;*********************************************************
; get amount of contiguous KB from address 0
; ret\ ax=KB size from address 0
;*********************************************************

biosGetMemorySize:
	int 0x12
	ret

;********************************************************
; get contiguous extended memory size
; ret\ ax=KB size above 1MN; ax=-1 on error
;********************************************************

biosGetExtendedMemorySize:
	mov ax, 0x88
	int 0x15
	jc .error
	test ax, ax
	je .error ; if size=0
	cmp ah, 0x86 ; unsupported function
	je .error
	cmp ah, 0x80 ; invalid command
	je .error
	ret
.error:
	mov ax, -1
	ret


%endif
