#include "debugDisplay.h"
#include <hal.h>
#include <bootinfo.h>
#include "mmngr_phys.h"

struct memory_region {
    uint32_t startLo;
    uint32_t startHi;
    uint32_t sizeLo;
    uint32_t sizeHi;
    uint32_t type;
    uint32_t acpi_3_0;
};

//! different memory regions(in memory_region.type)
char* strMemoryTypes[] = {
    "Available",    //memory_region.type==0
    "Reserved",
    "ACPI Reclaim",
    "ACPI NVS Memory"  //memory_region.type==3
};

int main(struct multiboot_info* bootinfo) {

//! get kernel size passed from boot loader by register edx
    uint32_t kernelSize = 0;
    asm volatile("xor %eax, %eax");
    asm volatile("mov %%dx, %%ax":"=a"(kernelSize));

    //! make demo look nice :)
    debugClrScr (0x13);
    debugGotoXY (0,0);
    
    debugSetColor (0x3F);
    debugPrintf ("                    ~[ Physical Memory Manager Demo ]~                          ");
    
    debugGotoXY (0,24);
    debugSetColor (0x3F);
    debugPrintf ("                                                                                ");
    
    debugGotoXY (0,2);
    debugSetColor (0x17);

    hal_initialize();

//! get memory size in KB
    uint32_t memSize = 1024+bootinfo->m_memoryLo+bootinfo->m_memoryHi*64;

// ! place memory bit map used by PMM at the end of the kernel in memory
    pmmngr_init(memSize, 0x100000+kernelSize*512);

    debugPrintf("pmm initialized with %d KB physical memory; memLo: %d memHi: %d\n\n", memSize, bootinfo->m_memoryLo, bootinfo->m_memoryHi);

    debugSetColor(0x19);
    debugPrintf("Physical Memory Map:\n");
    struct memory_region *region = (struct memory_region*)0x1000;

    for (int i=0; i<15; ++i) {
    
    	//! sanity check; if type is > 4 mark it reserved
    	if (region[i].type>4)
    		region[i].type=1;
    
    	//! if start address is 0, there is no more entries, break out
    	if (i>0 && region[i].startLo==0)
    		break;
    
    	//! display entry
    	debugPrintf ("region %i: start: 0x%x%x length (bytes): 0x%x%x type: %i (%s)\n", i, 
    		region[i].startHi, region[i].startLo,
    		region[i].sizeHi,region[i].sizeLo,
    		region[i].type, strMemoryTypes[region[i].type-1]);
    
    	//! if region is avilable memory, initialize the region for use
    	if (region[i].type==1)
    		pmmngr_init_region (region[i].startLo, region[i].sizeLo);
    }
    
    debugSetColor (0x17);
    
    debugPrintf ("\npmm regions initialized: %i allocation blocks; used or reserved blocks: %i\nfree blocks: %i\n",
    	pmmngr_get_block_count (),  pmmngr_get_use_block_count (), pmmngr_get_free_block_count () );
    
//! deinit the region the kernel is in as its in use
    pmmngr_deinit_region (0x100000, kernelSize*512);
    
    debugSetColor (0x17);
    
    debugPrintf ("\npmm regions initialized: %i allocation blocks; used or reserved blocks: %i\nfree blocks: %i\n",
    	pmmngr_get_block_count (),  pmmngr_get_use_block_count (), pmmngr_get_free_block_count () );
    
    //! allocating and deallocating memory examples...
    
    debugSetColor (0x12);
    
    uint32_t* p = (uint32_t*)pmmngr_alloc_block ();
    debugPrintf ("\np allocated at 0x%x", p);
    
    uint32_t* p2 = (uint32_t*)pmmngr_alloc_blocks (2);
    debugPrintf ("\nallocated 2 blocks for p2 at 0x%x", p2);
    
    pmmngr_free_block (p);
    p = (uint32_t*)pmmngr_alloc_block ();
    debugPrintf ("\nUnallocated p to free block 1. p is reallocated to 0x%x", p);
    
    pmmngr_free_block (p);
    pmmngr_free_blocks (p2, 2);

    return 0;

}
