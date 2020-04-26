%ifndef A20_H
%define A20_H

bits 16

;Enables A20 line through keyboard controller

enableA20_kbrd:
	cli
	push ax
	mov al, 0xdd
	out 0x64, al
	pop ax
	ret

_enableA20:
	cli
	call wait_input
	mov al, 0xad
	out 0x64, al
	
	
	call wait_input
	mov al, 0xd0
	out 0x64, al

	call wait_output
	in al, 0x60
	push eax

	call wait_input
	mov al, 0xd1
	out 0x64, al

	call wait_input
	pop eax
	or al, 0x2
	out 0x60, al

	call wait_input
	mov al, 0xae
	out 0x64, al

	call wait_input
	sti
	ret

;wait until keyboard controller input buffer is empty , before write to it
wait_input:
	in al, 0x64
	test al, 0x2
	jnz wait_input
	ret


;wait until keyboard controller output buffer is full , before read from it
wait_output:
	in al, 0x64
	test al, 0x1
	jz wait_output
	ret


%endif

