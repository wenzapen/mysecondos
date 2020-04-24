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

