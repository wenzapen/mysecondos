#include <string.h>
#include "mmngr_phys.h"
#include "debugDisplay.h"

#define PMMNGR_BLOCKS_PER_BYTE 8
#define PMMNGR_BLOCK_SIZE  4096
#define PMMNGR_BLOCK_ALIGN  PMMNGR_BLOCK_SIZE

// external functions
extern void _pmmngr_paging_enable(int b);
extern int _pmmngr_is_paging();
extern void _pmmngr_load_PDBR(physical_addr addr);
extern physical_addr _pmmngr_get_PDBR();

//! size of physical memory
static uint32_t _mmngr_memory_size = 0;

//! number of blocks currently in use
static uint32_t _mmngr_used_blocks = 0;

//! maximum number of available memory blocks
static uint32_t _mmngr_max_blocks = 0;

//! memory map bit array. Each bit represents a memory block
static uint32_t* _mmngr_memory_map = 0;



//! set any bit(frame) within the memory map bi array
void mmap_set(int bit);

//! unset any bit(frame) within the memory map bit array
void mmap_unset(int bit);

//! test any bit(frame) within the memory map bit array
int mmap_test(int bit);

//! finds first free frame in the bit array and returns its index
int mmap_first_free();

//! finds first free "size" number of frames and returns its index
int mmap_first_free_s(size_t size);

void mmap_set(int bit) {
    _mmngr_memory_map[bit/32] |= ( 1 << (bit % 32));
}

void mmap_unset(int bit) {
    _mmngr_memory_map[bit/32] &= ~ ( 1 << (bit % 32));
}

int mmap_test(int bit) {
    return _mmngr_memory_map[bit/32] & ( 1 << (bit % 32));

}


int mmap_first_free() {
    for(uint32_t i=0; i<pmmngr_get_block_count()/32; i++) {
	if(_mmngr_memory_map[i] != 0xffffffff) {
	    for( int j=0; j<32; j++) {
		if( !(_mmngr_memory_map[i] & (1 << j)))
		    return i*32+j;
	    }
	}
    }
    return -1;
}

int mmap_first_free_s(size_t size) {
    if(size == 0) return -1;
    if(size == 1) return mmap_first_free();
    for(uint32_t i=0; i<pmmngr_get_block_count()/32; i++) {
        if(_mmngr_memory_map[i] != 0xffffffff) {
            for( int j=0; j<32; j++) {
                if( !(_mmngr_memory_map[i] & (1 << j))) {
		    int startingBit = i*32 + j;
		    uint32_t free = 0;
		    for(uint32_t count=0; count<=size;count++) {
			if(! mmap_test(startingBit+count))
			    free++;
			if(free==size)
			    return i*32+j;
		    } 
		}
            }       
        }
    }
    return -1;
}

//! all memory blocks are used by default
void pmmngr_init(size_t memSize, physical_addr bitmap) {
    _mmngr_memory_size = memSize;
    _mmngr_memory_map = (uint32_t *)bitmap;
    _mmngr_max_blocks = (pmmngr_get_memory_size()*1024)/PMMNGR_BLOCK_SIZE;
    _mmngr_used_blocks = _mmngr_max_blocks;

    memset(_mmngr_memory_map, 0xff, pmmngr_get_block_count()/PMMNGR_BLOCKS_PER_BYTE);

}

//! unset memory map if available
void pmmngr_init_region(physical_addr base, size_t size) {
    int align = base / PMMNGR_BLOCK_SIZE;
    int blocks = size / PMMNGR_BLOCK_SIZE;
    if((size % PMMNGR_BLOCK_SIZE) != 0)
	blocks++;

    for(; blocks>0; blocks--) {
	mmap_unset(align++);
	_mmngr_used_blocks--;
    }
//    mmap_set(0);  // first block is always set. This inusres allocs can't be 0

}

void pmmngr_deinit_region(physical_addr base, size_t size) {
    int align = base / PMMNGR_BLOCK_SIZE;
    int blocks = size / PMMNGR_BLOCK_SIZE;

    for(; blocks>=0; blocks--) {
	mmap_set(align++);
	_mmngr_used_blocks++;
    }
}

void* pmmngr_alloc_block() {
    if(pmmngr_get_free_block_count() <= 0) return 0;
    int frame = mmap_first_free();
    if(frame == -1) return 0;
    mmap_set(frame);
    physical_addr addr = frame * PMMNGR_BLOCK_SIZE;
    _mmngr_used_blocks++;
    return (void *)addr;
}

void pmmngr_free_block(void* p) {
    physical_addr addr = (physical_addr)p;
    int frame = addr / PMMNGR_BLOCK_SIZE;
    mmap_unset(frame);
    _mmngr_used_blocks--;
}

void* pmmngr_alloc_blocks(size_t size) {
    if(pmmngr_get_free_block_count() <= size) return 0;
    int frame = mmap_first_free_s(size);
    if(frame == -1) return 0;
    for(uint32_t i=0; i<size; i++) 
        mmap_set(frame+i);
    physical_addr addr = frame * PMMNGR_BLOCK_SIZE;
    _mmngr_used_blocks += size;

    return (void *)addr;

}

void pmmngr_free_blocks(void* p, size_t size) {
    physical_addr addr = (physical_addr)p;
    int frame = addr / PMMNGR_BLOCK_SIZE;
    for(uint32_t i=0; i<size; i++) 
	mmap_unset(frame+i);
    _mmngr_used_blocks -= size;

}

size_t pmmngr_get_memory_size() {
    return _mmngr_memory_size;
}

uint32_t pmmngr_get_block_count() {
    return _mmngr_max_blocks; 
}

uint32_t pmmngr_get_use_block_count() {
    return _mmngr_used_blocks;
}

uint32_t pmmngr_get_free_block_count() {
    return _mmngr_max_blocks - _mmngr_used_blocks;
}

uint32_t pmmngr_get_block_size() {
    return PMMNGR_BLOCK_SIZE;
}

//! if b==0, disable paging; otherwise, enable paging
void pmmngr_paging_enable(int b) {
    _pmmngr_paging_enable(b);
}

//! return 1, if paging is enabled; otherwise return 0
int pmmngr_is_paging() {
    return _pmmngr_is_paging();
}

void pmmngr_load_PDBR(physical_addr addr) {
    _pmmngr_load_PDBR(addr);
}

physical_addr pmmngr_get_PDBR() {
    return _pmmngr_get_PDBR();
}
