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
    _cur_pdbr = (physical_addr)&dir->m_entries;
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
    pt_entry_set_frame(e, (physical_addr)f);
    pt_entry_add_attrib(e, I86_PTE_PRESENT);

    return 1;
}

void vmmngr_initialize() {
    ptable* table = (ptable*)pmmngr_alloc_block();
    if(!table) return;

    ptable* table2 = (ptable*)pmmngr_alloc_block();
    if(!table2) return;
    
    memset(table, 0, sizeof(ptable));
    memset(table2, 0, sizeof(ptable));

    //! identity map 0~4Mb
    for(int i=0,frame=0x0,virt=0x00000000;i<1024;i++,frame+=4096,virt+=4096) {
	pt_entry page = 0;
	pt_entry_add_attrib(&page, I86_PTE_PRESENT);
	pt_entry_set_frame(&page, frame);
	table2->m_entries[PAGE_TABLE_INDEX(virt)] = page;
    }
    //!  map 1Mb to 3Gb
    for(int i=0,frame=0x100000,virt=0xc0000000;i<1024;i++,frame+=4096,virt+=4096) {
	pt_entry page = 0;
	pt_entry_add_attrib(&page, I86_PTE_PRESENT);
	pt_entry_set_frame(&page, frame);
	table->m_entries[PAGE_TABLE_INDEX(virt)] = page;
    }

   //! create default directory table
    pdirectory* dir = (pdirectory*)pmmngr_alloc_block();
    if(!dir) return;
    memset(dir, 0, sizeof(pdirectory));

    pd_entry* entry = &dir->m_entries[PAGE_DIRECTORY_INDEX(0xc0000000)];
    pd_entry_add_attrib(entry, I86_PDE_PRESENT|I86_PDE_WRITABLE);
    pd_entry_set_frame(entry, (physical_addr)table);
    
    pd_entry* entry2 = &dir->m_entries[PAGE_DIRECTORY_INDEX(0x00000000)];
    pd_entry_add_attrib(entry2, I86_PDE_PRESENT|I86_PDE_WRITABLE);
    pd_entry_set_frame(entry2, (physical_addr)table2);

    vmmngr_switch_pdirectory(dir);
    pmmngr_paging_enable(1);
}
