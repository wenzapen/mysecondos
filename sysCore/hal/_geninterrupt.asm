bits 32
global _geninterrupt

_geninterrupt:
	push eax
	xor eax, eax
	mov al, byte [esp+8]
	mov byte [.genint+1], al
	jmp .genint
	
.genint:
	int 0
	pop eax
	ret
