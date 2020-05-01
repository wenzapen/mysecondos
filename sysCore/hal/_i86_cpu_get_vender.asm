bits 32
global _i86_cpu_get_vender

_i86_cpu_get_vender:
	mov eax, [esp+4]
	push eax
	mov eax, 0
	cpuid
	pop eax
	mov dword [eax], ebx
	mov dword [eax+4], edx
	mov dword [eax+8],ecx
	ret 	
