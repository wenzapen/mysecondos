#include "idt.h"
#include <string.h>

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
    asm volatile("lidt %0": : "m"(_idtr));
//    asm volatile("sti");
}

static void i86_default_handler() {
    #ifdef _DEBUG
//	debugClrScr(0x18);
	debugGotoXY(0,0);
	debugSetColor(0x1e);
	debugPuts("*** [i86 Hal] i86_default_handler: Unhandled Exception");
    #endif

//    asm volatile ("iret");
	for(;;);
}

static void i86_int_handler_32() {
    #ifdef _DEBUG
//	debugClrScr(0x18);
//	debugGotoXY(0,0);
	debugSetColor(0x1e);
	debugPuts("*** [i86 Hal] i86_int_handler_32: handled Exception");
    #endif
    while(1);
    asm volatile ("iret");

}

struct idt_descriptor* i86_get_ir(uint32_t i) {
    if(i > I86_MAX_INTERRUPTS) return 0;
    return &_idt[i];
}

int i86_install_ir(uint32_t i,uint16_t flags,uint16_t sel,I86_IRQ_HANDLER irq) {
    if(i > I86_MAX_INTERRUPTS) return 0;
    if(!irq) return 0;

    uint32_t uiBase = (uint32_t)irq;
    _idt[i].baseLo = (uint16_t)(uiBase & 0xffff);
    _idt[i].baseHi = (uint16_t)((uiBase >> 16) & 0xffff);
    _idt[i].reserved = 0;
    _idt[i].flags = (uint8_t)(flags);
    _idt[i].sel = sel;

    return 0;

}

int i86_idt_initialize(uint16_t codeSel) {
    _idtr.limit = sizeof(struct idt_descriptor)*I86_MAX_INTERRUPTS - 1;
    _idtr.base = (uint32_t)&_idt[0];

    memset((void*)&_idt[0], 0, sizeof(struct idt_descriptor)*I86_MAX_INTERRUPTS - 1);
    
//    for(int i=0; i<I86_MAX_INTERRUPTS;i++)
//	i86_install_ir(i, 0x8E, 0x8, (I86_IRQ_HANDLER)i86_int_handler_32);
//	i86_install_ir(i, 0x8E, 0x8, (I86_IRQ_HANDLER)i86_default_handler);

//    i86_install_ir(32, 0x8E, 0x8, (I86_IRQ_HANDLER)i86_int_handler_32);
    i86_install_ir( 0, 0x8e, 0x8, (I86_IRQ_HANDLER)isr0);
    i86_install_ir( 1, 0x8e, 0x8, (I86_IRQ_HANDLER)isr1);
    i86_install_ir( 2, 0x8e, 0x8, (I86_IRQ_HANDLER)isr2);
    i86_install_ir( 3, 0x8e, 0x8, (I86_IRQ_HANDLER)isr3);
    i86_install_ir( 4, 0x8e, 0x8, (I86_IRQ_HANDLER)isr4);
    i86_install_ir( 5, 0x8e, 0x8, (I86_IRQ_HANDLER)isr5);
    i86_install_ir( 6, 0x8e, 0x8, (I86_IRQ_HANDLER)isr6);
    i86_install_ir( 7, 0x8e, 0x8, (I86_IRQ_HANDLER)isr7);
    i86_install_ir( 8, 0x8e, 0x8, (I86_IRQ_HANDLER)isr8);
    i86_install_ir( 9, 0x8e, 0x8, (I86_IRQ_HANDLER)isr9);
    i86_install_ir(10, 0x8e, 0x8, (I86_IRQ_HANDLER)isr10);
    i86_install_ir(11, 0x8e, 0x8, (I86_IRQ_HANDLER)isr11);
    i86_install_ir(12, 0x8e, 0x8, (I86_IRQ_HANDLER)isr12);
    i86_install_ir(13, 0x8e, 0x8, (I86_IRQ_HANDLER)isr13);
    i86_install_ir(14, 0x8e, 0x8, (I86_IRQ_HANDLER)isr14);
    i86_install_ir(15, 0x8e, 0x8, (I86_IRQ_HANDLER)isr15);
    i86_install_ir(16, 0x8e, 0x8, (I86_IRQ_HANDLER)isr16);
    i86_install_ir(17, 0x8e, 0x8, (I86_IRQ_HANDLER)isr17);
    i86_install_ir(18, 0x8e, 0x8, (I86_IRQ_HANDLER)isr18);
    i86_install_ir(19, 0x8e, 0x8, (I86_IRQ_HANDLER)isr19);
    i86_install_ir(20, 0x8e, 0x8, (I86_IRQ_HANDLER)isr20);
    i86_install_ir(21, 0x8e, 0x8, (I86_IRQ_HANDLER)isr21);
    i86_install_ir(22, 0x8e, 0x8, (I86_IRQ_HANDLER)isr22);
    i86_install_ir(23, 0x8e, 0x8, (I86_IRQ_HANDLER)isr23);
    i86_install_ir(24, 0x8e, 0x8, (I86_IRQ_HANDLER)isr24);
    i86_install_ir(25, 0x8e, 0x8, (I86_IRQ_HANDLER)isr25);
    i86_install_ir(26, 0x8e, 0x8, (I86_IRQ_HANDLER)isr26);
    i86_install_ir(27, 0x8e, 0x8, (I86_IRQ_HANDLER)isr27);
    i86_install_ir(28, 0x8e, 0x8, (I86_IRQ_HANDLER)isr28);
    i86_install_ir(29, 0x8e, 0x8, (I86_IRQ_HANDLER)isr29);
    i86_install_ir(30, 0x8e, 0x8, (I86_IRQ_HANDLER)isr30);
    i86_install_ir(31, 0x8e, 0x8, (I86_IRQ_HANDLER)isr31);
    i86_install_ir(32, 0x8e, 0x8, (I86_IRQ_HANDLER)irq0);
    i86_install_ir(33, 0x8e, 0x8, (I86_IRQ_HANDLER)irq1);
    i86_install_ir(34, 0x8e, 0x8, (I86_IRQ_HANDLER)irq2);
    i86_install_ir(35, 0x8e, 0x8, (I86_IRQ_HANDLER)irq3);
    i86_install_ir(36, 0x8e, 0x8, (I86_IRQ_HANDLER)irq4);
    i86_install_ir(37, 0x8e, 0x8, (I86_IRQ_HANDLER)irq5);
    i86_install_ir(38, 0x8e, 0x8, (I86_IRQ_HANDLER)irq6);
    i86_install_ir(39, 0x8e, 0x8, (I86_IRQ_HANDLER)irq7);
    i86_install_ir(40, 0x8e, 0x8, (I86_IRQ_HANDLER)irq8);
    i86_install_ir(41, 0x8e, 0x8, (I86_IRQ_HANDLER)irq9);
    i86_install_ir(43, 0x8e, 0x8, (I86_IRQ_HANDLER)irq11);
    i86_install_ir(44, 0x8e, 0x8, (I86_IRQ_HANDLER)irq12);
    i86_install_ir(45, 0x8e, 0x8, (I86_IRQ_HANDLER)irq13);
    i86_install_ir(46, 0x8e, 0x8, (I86_IRQ_HANDLER)irq14);
    i86_install_ir(47, 0x8e, 0x8, (I86_IRQ_HANDLER)irq15);

    idt_install();
    return 0;

}

