#include "cpu.h"
#include "gdt.h"
#include "idt.h"

extern void _i86_cpu_get_vender(void*);

int i86_cpu_initialize() {
    i86_gdt_initialize();
    i86_idt_initialize(0x8);
    return 0;
}

int i86_cpu_shutdown() {
    return 0;
}

char* i86_cpu_get_vender() {
    static char vender[32] = {0};
    _i86_cpu_get_vender(&vender);
    return vender;
}
