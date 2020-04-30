#include "gdt.h"
#include <string.h>

struct gdtr {
    uint16_t m_limit;
    uint32_t m_base;
}__attribute((packed))__;

static struct gdt_descriptor _gdt[MAX_DESCRIPTORS];
static struct gdtr _gdtr;

static void gdt_install();

static void gdt_install() {
   asm volatile("lgdt [_gdtr]");
}

void gdt_set_descriptor(uint32_t i,uint64_t base,uint64_t limit,uint8_t access,uint8_t grand) {
    if(i > MAX_DESCRIPTORS) return;
    memset((void*)&_gdt[i], 0, sizeof(gdt_descriptor));

    _gdt[i].baseLo = uint16_t(base & 0xffff);
    _gdt[i].baseMid = uint8_t((base >> 16) & 0xff);
    _gdt[i].baseHi = uint8_t((base >> 24) & 0xff);
    _gdt[i].limit = uint16_t(limit & 0xffff);
    _gdt[i].flags = access;
    _gdt[i].grand = uint8_t((limit >> 16) & 0x0f);
    _gdt[i].grand |= grand & 0xf0;

}

gdt_descriptor* i86_gdt_get_descriptor(int i) {
    if(i > MAX_DESCRIPTORS) return 0;
    return &_gdt[i];

}

int i86_gdt_initialize() {
    _gdtr.m_limit = (sizeof(struct gdt_descriptor)*MAX_DESCRIPTORS) - 1;
    _gdtr.m_base = (uint32_t)&_gdt[0];

    gdt_set_descriptor(0,0,0,0,0);
    gdt_set_descriptor(1,0,0xffffffff,0x9a,0xcf);
    gdt_set_descriptor(2,0,0xffffffff,0x92,0xcf);
    gdt_set_descriptor(3,0,0xffffffff,0xfa,0xcf);
    gdt_set_descriptor(4,0,0xffffffff,0xf2,0xcf);
    gdt_install();
    return 0;
}
