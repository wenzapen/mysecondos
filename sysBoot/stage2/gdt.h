%ifndef GDT_H
%define GDT_H

;**********************************************************
;	installGDT()
;		-Install GDT tables
;**********************************************************

bits 16
installGDT:
	cli
	pusha
	lgdt [toc]
	sti
	popa
	ret

gdt_data:
	dd 0	;null descriptor
	dd 0
;gdt code:
	dw 0xffff
	dw 0
	db 0
	db 10011010b
	db 11001111b
	db 0
;gdt data:
	dw 0xffff
	dw 0
	db 0
	db 10010010b
	db 11001111b
	db 0
end_of_gdt:
toc:
	dw end_of_gdt - gdt_data -1	;limit(size of GDT)
	dd gdt_data			;base of GDT

%define NULL_DESC 0
%define CODE_DESC 0x8
%define DATA_DESC 0x10


%endif
