bits 32
global _pmmngr_paging_enable
global _pmmngr_is_paging
global _pmmngr_load_PDBR
global _pmmngr_get_PDBR

_pmmngr_paging_enable:
	mov eax, [0x4+esp]
	cmp eax, 0x0
	jz ._disable
._enable:
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
	jmp .done
._disable:
	mov eax, cr0
	and eax, 0x7fffffff
	mov cr0, eax 	
.done:
	ret

_pmmngr_is_paging:
	mov eax, cr0
	and eax, 0x80000000
	ret

_pmmngr_load_PDBR:
	mov eax, [0x4+esp]
	mov cr3, eax
	ret

_pmmngr_get_PDBR:
	mov eax, cr3
	ret

_vmmngr_flush_tlb_entry:
	mov eax, [0x4+esp]
	cli
	invlpg eax
	sti
	ret
