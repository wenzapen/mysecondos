%ifndef FAT12_H
%define FAT12_H


;************************************************
; loadRoot
;	- Load Root Directory Table to 0x7e00
;
;************************************************
loadRoot:
	pusha
	push es
; compute size of root directory and store in cx
	xor cx, cx
	xor dx, dx
	mov ax, 32
	mul WORD [bpbRootEntries]
	div WORD [bpbBytesPerSector] ; ax: quotient; dx: remainder
	xchg ax, cx 
; compute location of root directory and store in ax
	mov al, BYTE [bpbNumberOfFATs]
	mul WORD [bpbSectorsPerFAT]
	add ax, WORD [bpbReservedSectors]
	mov WORD [dataSector], ax
	add WORD [dataSector], cx
;read root directory into 0x7e00
	push word ROOT_SEG
	pop es
	mov bx, 0
	call readSectors
	pop es
	popa
	ret

;************************************************
; loadFAT
;	- load FAT to 0x7c00
; ES:DI => root directory table
;************************************************

loadFAT:
	pusha
	push es
;compute size of FAT and store in cx
	xor ax, ax
	mov al, BYTE [bpbNumberOfFATs]
	mul WORD [bpbSectorsPerFAT]
	mov cx, ax
;compute location of FAT and store in ax
	mov ax, WORD [bpbReservedSectors]
; read FAT into memory (overwrite our bootloader at 0x7c00)
	push word FAT_SEG
	pop es
	xor bx, bx
	call readSectors
	pop es
	popa
	ret

;****************************************************
; findFile
;	-search for filename in root table
;
; ds:si => file name
; ret ax =>file index number in directory table. -1 if error
;*****************************************************

findFile:
	push cx
	push dx
	push bx
	mov bx, si
; browse root directory for binary image
	mov cx, WORD [bpbRootEntries]
	mov di, ROOT_OFFSET
	cld
.loop:
	push cx
	mov cx, 11
	mov si, bx
	push di
	req cmpsb
	pop di
	je .found
	pop cx
	add di, 32
	loop .loop
.notFound:
	pop bx
	pop dx
	pop cx
	mov ax, -1
	ret
.found:
	pop ax   ;  pop cx into ax; ax contains the file entry
	pop bx
	pop dx
	pop cx
	ret

;*************************************************************
;
; loadFile
;	- load file
; es:si=> file to load
; ebx:bp => buffer to load file to
; ret/ax => -1 on error; 0 on success
; ret/cx => number of sectors read
;************************************************************

loadFile:
	xor ecx, ecx
	push ecx
.findFile:
	push bx
	push bp
	call findFile
	cmp ax, -1
	jne .loadImagePre
	pop bp
	pop bx
	pop ecx
	mov ax, -1
	ret
.loadImagePre:
	sub edi, ROOT_OFFSET
	sub eax, ROOT_OFFSET
; get starting cluster
	push word ROOT_SEG
	pop es
	mov dx, WORD [es:di + 0x1a]
	mov WORD [cluster], DX
	pop bx
	pop es
	push bx
	push es
	call loadFAT
.loadImage:
	;load the cluster
	mov ax, WORD [cluster]
	pop es
	pop bx
	call clusterLBA	
	xor cx, cx
	mov cl, BYTE [bpbSectorsPerCluster]
	call readSectors
	pop ecx
	inc ecx
	push ecx
	push bx
	push es
	mov ax, FAT_SEG
	mov es, ax
	xor bx, bx
	;get next cluster
	mov ax, WORD [cluster]
	mov cx, ax
	mov dx, ax
	shr dx, 0x1
	add cx, dx
	mov bx, 0
	add bx, cx
	mov dx, WORD [es:bx]
	test ax, 0x1
	jnz .ODD_CLUSTER

.EVEN_CLUSTER:
	add dx, 0000111111111111b
	jmp .done
.ODD_CLUSTER:	
	shr dx, 0x4
.done:
	mov WORD [cluster], dx
	cmp dx, 0xff0
	jb .loadImage
.success:
	pop es
	pop bx
	pop ecx
	xor ax, ax
	ret

%endif
