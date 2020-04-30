#include "idt.h"

#ifdef _DEBUG
#include "../kernel/debugDisplay.h"
#endif

struct idtr {
    uint16_t limit;
    uint32_t base;
}__attribute__((packed));

static struct idt_descriptor _idt[I86_MAX_INTERRUPTS];
static struct idtr _idtr;

static void idt_install();
static void i86_default_handler();

static void idt_install() {
    asm volatile("lidt [_idtr]");
}

static void i86_default_handler() {
    #ifdef _DEBUG
	debugClrScr(0x18);
	debugGotoXY(0,0);
	debugSetColor(0x1e);
	debugPuts("*** [i86 Hal] i86_default_handler: Unhandled Exception");
    #endif

	for(;;);
}

idt_descriptor* i86_get_ir(uint32_t i) {
    if(i > I86_MAX_INTERRUPTS) return 0;
    return &_idt[i];
}

int i86_install_ir(uint32_t i,uint16_t flags,uint16_t sel,I86_IRQ_HANDLER irq) {
    if(i > I86_MAX_INTERRUPTS) return 0;
    if(!irq) return 0;

    uint32_t uiBase = (uint32_t)irq;
    _idt[i].baseLo = uint16_t(uiBase & 0xffff);
    _idt[i].baseHi = uint16_t((uiBase >> 16) & 0xffff);
    _idt[i].reserved = 0;
    _idt[i].flags = uint8_t(flags);
    _idt[i].sel = sel;

    return 0;

}

int i86_idt_initialize(uint16_t codeSel) {
    _idtr.limit = sizeof(struct idt_descriptor)*I86_MAX_INTERRUPTS - 1;
    _idtr.base = (uint32_t)&_idt[0];

    memset((void*)&_idt[0], 0, sizeof(idt_descriptor)*I86_MAX_INTERRUPTS - 1);
    
    for(int i=0; i<I86_MAX_INTERRUPTS;i++)
	i86_install_ir(i, 0x8E, 0x8, (I86_IRQ_HANDLER)i86_default_handler);

    idt_install();
    return 0;

}
