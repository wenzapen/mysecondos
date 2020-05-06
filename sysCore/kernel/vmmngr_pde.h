#ifndef _VMMNGR_PDE_H
#define _VMMNGR_PDE_H

#include <stdint.h>
#include "mmngr_phys.h"

#define I86_PDE_PRESENT 0x1
#define I86_PDE_WRITABLE 0x2
#define I86_PDE_USER 0x4
#define I86_PDE_PWT 0x8
#define I86_PDE_PCD 0x10
#define I86_PDE_ACCESSED 0x20
#define I86_PDE_DIRTY 0x40
#define I86_PDE_4MB 0x80
#define I86_PDE_CPU_GLOBAL 0x100
#define I86_PDE_LV4_GLOBAL 0x200
#define I86_PDE_FRAME 0x7ffff000

typedef uint32_t pd_entry;

void pd_entry_add_attrib(pd_entry* e, uint32_t attrib);
void pd_entry_del_attrib(pd_entry* e, uint32_t attrib);
void pd_entry_set_frame(pd_entry*, physical_addr);
int pd_entry_is_present(pd_entry e);
int pd_entry_is_user(pd_entry e);
int pd_entry_is_4mb(pd_entry e);
int pd_entry_is_writable(pd_entry e);
//! get page table entry frame address
physical_addr pd_entry_pfn(pd_entry e);
//! enable global pages
void pd_entry_enable_global(pd_entry e);

#endif
