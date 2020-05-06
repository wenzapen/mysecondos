#include <string.h>
#include "mmngr_virtual.h"
#include "mmngr_phys.h"

#define PTABLE_ADDR_SPACE_SIZE 0x400000
#define DTABLE_ADDR_SPACE_SIZE 0x100000000
#define PAGE_SIZE 4096


extern void _vmmngr_flush_tlb_entry(virtual_addr addr);

//! current directory table
pdirectory* _cur_directory = 0;

//! current page directory base register
physical_addr _cur_pdbr = 0;

pt_entry* vmmngr_ptable_lookup_entry(ptable* p, virtual_addr addr) {
    if(p)
	return &p->m_entries[PAGE_TABLE_INDEX(addr)];
    return 0;
}

pd_entry* vmmngr_pdirectory_lookup_entry(pdirectory* p, virtual_addr addr) {
    if(p)
	return &p->m_entries[PAGE_DIRECTORY_INDEX(addr)];
    return 0;
}

int vmmngr_switch_pdirectory(pdirectory* dir) {
    if(!dir)
	return 0;
    _cur_directory = dir;
    pmmngr_load_PDBR(_cur_pdbr);
    return 1;
}

void vmmngr_flush_tlb_entry(virtual_addr addr) {
    _vmmngr_flush_tlb_entry(addr);
}

pdirectory* vmmngr_get_directory() {
    return _cur_directory;
}

int vmmngr_alloc_page(pt_entry* e) {
    //! allocate a free physical frame to the page
    void* f = pmmngr_alloc_block();
    if(!f)
	return 0;
    pt_entry_set_frame(e, (physical_addr)p);
    pt_entry_add_attrib(e, I86_PET_PRESENT);

    return 1;
}