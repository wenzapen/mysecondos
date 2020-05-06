#ifndef _MMNGR_VIRT_H
#define _MMNGR_VIRT_H

#include <stdint.h>
#include "vmmngr_pte.h"
#include "vmmngr_pde.h"

typedef uint32_t virtual_addr;

#define PAGES_PER_TABLE 1024
#define TABLES_PER_DIR  1024

#define PAGE_DIRECTORY_INDEX(x) (((x)>>22) & 0x3ff)
#define PAGE_TABLE_INDEX(x) (((x)>>12) & 0x3ff)
#define PAGE_GET_PHYSICAL_ADDRESS(x) (*x & ~0xfff)


typedef struct  {
    pt_entry m_entries[PAGES_PER_TABLE];
} ptable;

typedef struct  {
    pd_entry m_entries[TABLES_PER_DIR];
} pdirectory;

//! maps phys to virtual address
void mm_map_page(void *phys, void *virt);

void vmmngr_initialize();

//! allocates a page in physical memory
int vmmngr_alloc_page(pt_entry *);

void vmmngr_free_page(pt_entry *e);

int vmmngr_switch_pdirectory(pdirectory *);

pdirectory* vmmngr_get_directory();

void vmmngr_flush_tlb_entry(virtual_addr addr);

void vmmngr_ptable_clear(ptable *p);

uint32_t vmmngr_ptable_virt_to_index(virtual_addr addr);

pt_entry* vmmngr_ptable_lookup_entry(ptable* p, virtual_addr addr);

uint32_t vmmngr_pdirectory_virt_to_index(virtual_addr addr);

void vmmngr_pdirectory_clear(pdirectory* dir);

pd_entry* vmmngr_pdirectory_lookup_entry(pdirectory* p, virtual_addr addr);

#endif

