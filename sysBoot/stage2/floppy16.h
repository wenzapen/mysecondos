%ifndef FLOPPY16_H
%define FLOPPY_16_H

bits 16

bpbOEM db "My OS   "
bpbBytesPerSector: dw 512
bpbSectorsPerCluster: db 1
bpbReservedSectors: dw 1
bpbNumberOfFATs: db 2
bpbRootEntries: dw 224
bpbTotalSectors: dw 2880
bpbMedia: db 0xf0
bpbSectorsPerFAT: dw 9
bpbSectorsPerTrack: dw 18
bpbHeadsPerCylinder: dw 2
bpbHiddenSectors: dd 0
bpbTotalSectorsBig: dd 0
bsDriveNumber: db 0
bsUnused: db 0
bsExtBootsSignature: db 0x29
bsSerialNumber: dd 0xa0a1a2a3
bsVolumeLabel: db "MOS FLOPPY "
bsFileSystem: db "FAT12   "

dataSector dw 0x0000
cluster dw 0x0000

absoluteSector db 0x00
absoluteHead db 0x00
absoluteTrack db 0x00

;********************************************
; Convert CHS to LBA
; LBA = (Cluster -2 ) * sectors per cluster
; store the result in ax
;********************************************

clusterLBA:
	sub ax, 0x2
	xor cx, cx
	mov cl, BYTE [bpbSectorsPerCluster]
	mul cx
	add ax, WORD [dataSector]
	ret

;**********************************************
; convert LBA to CHS
; ax: LBA to convert
;
; absolute sector = (LBA % sectorsPerTrack)+1
; absolute head = (LBA/sectorsPerTrack) mod numberOfHeads
; absolute track = LBA / ( sectorsPerTrack * numberOfHeads)
; DIV: dividend in dx:ax; quotient in ax; remainder in dx
;***********************************************

LBACHS:
	xor dx, dx
	div WORD [bpbSectorsPerTrack]
	inc dl	; remainder in dx ; quotient in ax
	mov BYTE [absoluteSector], dl
	xor dx, dx
	div WORD [bpbHeadsPerCylinder]
	mov BYTE [absoluteHead], dl
	mov BYTE [absoluteTrack], al
	ret
		
;***********************************************
;
; reads a series of sectors
; cx: number of sectors to read
; ax: starting sector
; es:ebx : buffer to read to
;************************************************

readSectors:
	.main: 
		mov di, 0x5 ; five retries for error
	.sectorloop:
		push ax
		push bx
		push cx
		call LBACHS
		mov ah, 0x02
		mov al, 0x01
		mov ch, BYTE [absoluteTrack]
		mov cl, BYTE [absoluteSector]
		mov dh, BYTE [absoluteHead]
		mov dl, BYTE [bsDriveNumber]
		int 0x13
		jnc .success
		xor ax, ax 
		int 0x13  ; BIOS reset disk
		dec di
		pop cx
		pop bx
		pop ax
		jnz .sectorloop
		int 0x18

	.success:
		pop cx
		pop bx
		pop ax
		add bx, WORD [bpbBytesPerSector]
		inc ax
		loop .main ; loop until cx = 0
		ret	

%endif
