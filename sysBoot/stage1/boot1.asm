bits 16

start:
    jmp main 

bpbOEM: db "My OS "
bpbBytesPerSector: dw 512
bpbSectorsPerCluster: db 1
bpbReservedSectors: dw 1
bpbNumberOfFATs: db 2
bpbRootEntries: dw 224
bpbTotalSectors: dw 2880
bpbMedia: db 0xf8
bpbSectorsPerFAT: dw 9
bpbSectorsPerTrack: dw 18
bpbHeadsPerCylinder: dw 2
bpbHiddenSectors: dd 0
bpbTotalSectorsBig: dd 0
bsDriveNumber: db 0
bsUnsed: db 0
bsExtBootSignature: db 0x29
bsSerialNumber: dd 0xa0a1a2a3
bsVolumeLabel: db "MOS FLOPPY "
bsFileSystem: db "FAT12  "

print:
    lodsb
    or al, al
    jz printDone
    mov ah, 0xe
    int 0x10
    jmp print

printDone:
    ret

readSectors:
    .MAIN:
	mov di, 0x0005
    .SECTORLOOP:
	push ax
	push bx
	push cx
	call LBACHS
	mov ah, 0x2
	mov al, 0x01
	mov ch, BYTE [absoluteTrack]
	mov cl, BYTE [absoluteSector]
	mov dh, BYTE [absoluteHead]
	mov dl, BYTE [bsDriveNumber]
	int 0x13
	jnc .SUCCESS
	xor ax,ax
	int 0x13
	dec di
	pop cx
	pop bx
	pop ax
	jnz .SECTORLOOP
	int 0x18
    .SUCCESS:
	mov si, msgProgress
	call print
	pop cx
	pop bx
	pop ax
	add bx, WORD [bpbBytesPerSector]
	inc ax
	loop .MAIN
	ret

clusterLBA:
    sub ax, 0x2
    xor cx, cx
    mov cl, BYTE [bpbSectorsPerCluster]
    mul cx
    add ax, WORD [datasector]
    ret

LBACHS:
    xor dx, dx  ;sector number is stored in 'ax'
    div WORD [bpbSectorsPerTrack]
    inc dl
    mov BYTE [absoluteSector], dl
    xor dx, dx
    div WORD [bpbHeadsPerCylinder]
    mov BYTE [absoluteHead], dl
    mov BYTE [absoluteTrack], al
    ret

main:
    cli
    mov ax, 0x07c0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

;create stack
    mov ax, 0x0
    mov ss, ax
    mov sp, 0xffff
    sti

    mov si, msgLoading
    call print

LOAD_ROOT:
;compute size of root directory and store in 'cx'
    xor cx, cx
    xor dx, dx
    mov ax, 0x20 ;20 byte directory entry
    mul WORD [bpbRootEntries]
    div WORD [bpbBytesPerSector]
    xchg ax, cx

;compute location of root directory and store in 'ax'
    mov al, BYTE [bpbNumberOfFATs]
    mul WORD [bpbSectorsPerFAT]
    add ax, WORD [bpbReservedSectors]
    mov WORD [datasector], ax
    add WORD [datasector], cx

;read root directory into memory (7c00:0200)
    mov bx, 0x0200
    call readSectors

;find stage 2 image
;browse root directory for binary image
    mov cx, WORD [bpbRootEntries]
    mov di, 0x0200 ;locate first root entry
    .LOOP:
	push cx
	mov cx, 0xb ;eleven character name
	mov si, imageName
	push di
    rep cmpsb
	pop di
	je LOAD_FAT
	pop cx
	add di, 0x20
	loop .LOOP
	jmp FAILURE
LOAD_FAT:
;save starting cluster of boot image
    mov si, msgCRLF
    call print
    mov dx, WORD [di + 0x1A] ; the location of first cluster
    mov WORD [cluster], dx
;compute size of FAT and store in 'cx'
    xor ax, ax
    mov al, BYTE [bpbNumberOfFATs]
    mul WORD [bpbSectorsPerFAT]
    mov cx, ax
;compute location of FAT and store in 'ax'
    mov ax, WORD [bpbReservedSectors]
;read FAT into memory (7c00:0200)
    mov bx, 0x0200
    call readSectors
;read image file into memory(0050:0000)
    mov si, msgCRLF
    call print
    mov ax, 0x0050
    mov es, ax
    mov bx, 0x0
    push bx
;load stage 2
LOAD_IMAGE:
    mov ax, WORD [cluster]
    pop bx
    call clusterLBA
    xor cx, cx
    mov cl, BYTE [bpbSectorsPerCluster]
    call readSectors
    push bx
;compute next cluster
    mov ax, WORD [cluster]
    mov cx, ax
    mov dx, ax
    shr dx, 0x1
    add cx, dx
    mov bx, 0x0200
    add bx, cx
    mov dx, WORD [bx]
    test ax, 0x1
    jnz .ODD_CLUSTER

.EVEN_CLUSTER:
    and dx, 0xFFF
    jmp .DONE
.ODD_CLUSTER:
    shr dx, 0x4
.DONE:
    mov WORD [cluster], dx
    cmp dx, 0xFF0
    jb LOAD_IMAGE
DONE:
    mov si, msgCRLF
    call print
    push WORD 0x0050
    push WORD 0x0000
    retf
FAILURE:
    mov si, msgFailure
    call print
    mov ah, 0x00
    int 0x16
    int 0x19

absoluteSector: db 0x00
absoluteHead: db 0x00
absoluteTrack: db 0x00
datasector: dw 0x0000
cluster: dw 0x0000
imageName: db "stage2  bin"
msgLoading: db 0x0d, 0x0a, "Loading Boot Image ", 0x0d, 0x0a, 0x00
msgCRLF: db 0x0d, 0x0a, 0x00
msgProgress: db ".", 0x00
msgFailure: db 0x0d, 0x0a, "ERROR : press Any Key to Reboot", 0x0a, 0x00

times 510 - ($-$$) db 0
dw 0xaa55

