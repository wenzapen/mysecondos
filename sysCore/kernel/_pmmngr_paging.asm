bits 32
global _pmmngr_paging_enable
global _pmmngr_is_paging
global _pmmngr_load_PDBR
global _pmmngr_get_PDBR

_pmmngr_paging_enable:
	mov eax, cr0
	cmp 0x0, 0x4(esp)
	jz ._disable
._enable:
	or eax, 0x80000000
	mov cr0, eax
	jmp done
._diable:
	and eax, 0x7fffffff
	mov cr0, eax 	
.done:
	ret

_pmmngr_is_paging:
	mov eax, cr0
	and eax, 0x80000000
	ret

_pmmngr_load_PDBR:
	mov eax, 0x4(esp)
	mov cr3, eax
	ret

_pmmngr_get_PDBR:
	mov eax, cr3
	ret
