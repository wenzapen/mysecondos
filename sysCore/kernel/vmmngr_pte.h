#ifndef _VMMNGR_PTE_H
#define _VMMNGR_PTE_H

#include <stdint.h>
#include "mmngr_phys.h"

#define I86_PTE_PRESENT 0x1
#define I86_PTE_WRITABLE 0x2
#define I86_PTE_USER 0x4
#define I86_PTE_WRITETHROUGH 0x8
#define I86_PTE_NOT_CACHEABLE 0x10
#define I86_PTE_ACCESSED 0x20
#define I86_PTE_DIRTY 0x40
#define I86_PTE_PAT 0x80
#define I86_PTE_CPU_GLOBAL 0x100
#define I86_PTE_LV4_GLOBAL 0x200
#define I86_PTE_FRAME 0x7ffff000

typedef uint32_t pt_entry;

void pt_entry_add_attrib(pt_entry* e, uint32_t attrib);
void pt_entry_del_attrib(pt_entry* e, uint32_t attrib);
void pt_entry_set_frame(pt_entry*, physical_addr);
int pt_entry_is_present(pt_entry e);
int pt_entry_is_writable(pt_entry e);
//! get page table entry frame address
physical_addr pt_entry_pfn(pt_entry e);

#endif
