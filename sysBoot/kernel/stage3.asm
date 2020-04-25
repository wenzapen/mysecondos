org 0x100000

bits 32

jmp stage3

%include "sysBoot/kernel/stdio.h"

msg db 0xa, 0xa, "            OS Development Series "
    db 0xa, 0xa, "          MOS 32 Bit Kernel Executing", 0xa, 0

stage3:
	mov ax, 0x10
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov esp, 0x90000

	call clrScr32
	mov ebx, msg
	call print32

	cli
	hlt
